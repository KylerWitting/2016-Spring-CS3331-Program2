// ----------------------------------------------------------- 
// NAME : Kyler Witting                         User ID: kmwittin 
// DUE DATE : 02/26/2016                                       
// PROGRAM ASSIGNMENT 2                                        
// FILE NAME : main.c             
// PROGRAM PURPOSE :                                           
// 
// 1. Program main.c  reads three input arrays a[ ], x[ ] and y[ ] into one or more shared memory
// segments. Let the number of elements of a[ ], x[ ] and y[ ] be k, m and n, respectively.
// 
// 2. main.c  prints out the input arrays.
// 
// 3. main.c  creates a child process to run program qsort.c  using the execvp()  system call and
// passes the assigned Left , Right  and other needed information to program qsort.c . Initially,
// Left  and Right  are 0 and k­1, respectively.
// 
// 4. main.c  creates a second child process to run program merge.c  using the execvp()  system call
// and passes the needed command line arguments to merge.c .
// 
// 5. Then, main.c  waits for both child processes to complete, prints out the results, and terminates itself.
//
// ----------------------------------------------------------- 

#include <stdio.h>      //Needed for sprintf
#include <string.h>     //Needed for strlen
#include <sys/types.h>  //Has all system data types
#include <unistd.h>     //Declares standard symbolic constants and types
#include <sys/wait.h>   //The wait command 
#include <sys/shm.h>    //For Shared Memory 

int printArray( int array[], int m, int type ); 

int main( ) {
    //execvp
    char prog[7];
    char *argv2[5];
    char temp1[10], temp2[10], temp3[10], temp4[10];

    //fork
    pid_t child;
    int status;

    //shared mem
    int *qArr, *xArr, *yArr, *outArr;
    int qshmid, xshmid, yshmid, outshmid;
    key_t qKey, xKey, yKey, outKey;

    //scanning var
    int temp, i;

    //input var
    int k, m, n; 

    //misc
    char buffer[256];


    //Qsort Shared Memory 
    scanf( "%d", &k); 

    qKey = ftok("./", 'q');
    sprintf( buffer, "*** MAIN: qsort shared memory key = %d\n", qKey ); 
    write( 1, buffer, strlen( buffer ) );

    qshmid = shmget( qKey, k*sizeof(int), 0666 |  IPC_CREAT  | IPC_EXCL );
    sprintf( buffer, "*** MAIN: qsort shared memory created\n" ); 
    write( 1, buffer, strlen( buffer ) );

    qArr = (int *)shmat(qshmid, NULL, 0);
    sprintf( buffer, "*** MAIN: qsort shared memory attached and is ready to use\n\n" ); 
    write( 1, buffer, strlen( buffer ) );

    for( i = 0; i < k; i++ ) { 
        scanf( "%d", &temp );
        qArr[i] = temp; 
    }


    //X Array Shared Memory 
    scanf( "%d", &m); 

    xKey = ftok("./", 'x');
    sprintf( buffer, "*** MAIN: Merge x Array shared memory key = %d\n", xKey ); 
    write( 1, buffer, strlen( buffer ) );

    xshmid = shmget( xKey, m*sizeof(int), 0666 |  IPC_CREAT  | IPC_EXCL );
    sprintf( buffer, "*** MAIN: Merge x Array shared memory created\n" ); 
    write( 1, buffer, strlen( buffer ) );

    xArr = (int *)shmat(xshmid, NULL, 0);
    sprintf( buffer, "*** MAIN: Merge x Array shared memory attached and is ready to use\n\n" ); 
    write( 1, buffer, strlen( buffer ) );

    for( i = 0; i < m; i++ ) { 
        scanf( "%d", &temp );
        xArr[i] = temp; 
    }

    //Y Array Shared Memory 
    scanf( "%d", &n); 

    yKey = ftok("./", 'y');
    sprintf( buffer, "*** MAIN: Merge y Array shared memory key = %d\n", yKey ); 
    write( 1, buffer, strlen( buffer ) );

    yshmid = shmget( yKey, (n)*sizeof(int), 0666 |  IPC_CREAT  | IPC_EXCL );
    sprintf( buffer, "*** MAIN: Merge y Array shared memory created\n" ); 
    write( 1, buffer, strlen( buffer ) );

    yArr = (int *)shmat(yshmid, NULL, 0);
    sprintf( buffer, "*** MAIN: Merge y Array shared memory attached and is ready to use\n\n" ); 
    write( 1, buffer, strlen( buffer ) );

    for( i = 0; i < n; i++ ) { 
        scanf( "%d", &temp );
        yArr[i] = temp; 
    }


    //Output Array Shared Memory 

    outKey = ftok("./", 'o');
    sprintf( buffer, "*** MAIN: Merge Output Array shared memory key = %d\n", outKey ); 
    write( 1, buffer, strlen( buffer ) );

    outshmid = shmget( outKey, (m+n)*sizeof(int), 0666 |  IPC_CREAT  | IPC_EXCL );
    sprintf( buffer, "*** MAIN: Merge Output Array shared memory created\n" ); 
    write( 1, buffer, strlen( buffer ) );

    outArr = (int *)shmat(outshmid, NULL, 0);
    sprintf( buffer, "*** MAIN: Merge Output Array shared memory attached and is ready to use\n\n" ); 
    write( 1, buffer, strlen( buffer ) );

    //Prints the given arrays 
    printArray( qArr, k, 0 );

    printArray( xArr, m, 1 );

    printArray( yArr, n, 2 );

    //Qsort fork into execvp 
    if ( ( child = fork() ) == 0 ) {
        sprintf( buffer, "*** MAIN: about to spawn the process for qsort\n" );
        write( 1, buffer, strlen( buffer ) );  

        sprintf( prog, "./qsort" );
        argv2[0] =   prog      ;
        sprintf( temp1, "%d", k );
        sprintf( temp2, "%d", 0 );
        sprintf( temp3, "%d", k-1 );
        char *argv2[] = { prog, temp1, temp2, temp3, '\0'};

        execvp( prog, argv2 );
        sprintf( buffer, "***MAIN: Q Sort Execvp has Failed.\n" );
        write( 1, buffer, strlen( buffer ) );
        return -1; 
    }
    if( child < 0 ) {
        sprintf( buffer, "*** MAIN: qsort fork() failed.\n" );
        write( 2, buffer, strlen( buffer ) );
    }

    //Binary Merge into execvp 
    if ( ( child = fork() ) == 0 ) {
        //merge goes here
        sprintf( buffer, "*** MAIN: about to spawn the process for merge\n" );
        write( 1, buffer, strlen( buffer ) );  
        sprintf( prog, "./merge" );
        argv2[0] =   prog      ;
        sprintf( temp1, "%d", m );
        sprintf( temp2, "%d", n );
        sprintf( temp3, "%d", m+n );
        char *argv2[] = { prog, temp1, temp2, temp3, '\0' };

        execvp( prog, argv2 );
        sprintf( buffer, "***MAIN: Merge Execvp has Failed.\n" );
        write( 1, buffer, strlen( buffer ) );
        return -1; 
    }
    if( child < 0 ) {
        sprintf( buffer, "*** MAIN: Binary Merge fork() failed.\n" );
        write( 2, buffer, strlen( buffer ) );
    }



    //Waits on the children processes to complete before detaching and removing shared memory
    //And then exiting the program
    wait( &status );
    wait( &status );
    printArray( qArr, k, 3 );
    printArray( outArr, m+n, 4 );


    //Removing the shared memory 
    shmdt((void *) qArr );
    sprintf( buffer, "*** MAIN: qsort shared memory successfully detached\n" );
    write( 1, buffer, strlen( buffer ) );

    shmctl( qshmid, IPC_RMID, NULL);
    sprintf( buffer, "*** MAIN: qsort shared memory successfully removed\n\n" );
    write( 1, buffer, strlen( buffer ) );


    shmdt((void *) xArr);
    sprintf( buffer, "*** MAIN: Merge x Array shared memory successfully detached\n" );
    write( 1, buffer, strlen( buffer ) );

    shmctl( xshmid, IPC_RMID, NULL);
    sprintf( buffer, "*** MAIN: Merge x Array shared memory successfully removed\n\n" );
    write( 1, buffer, strlen( buffer ) );


    shmdt((void *) yArr);
    sprintf( buffer, "*** MAIN: Merge y Array shared memory successfully detached\n" );
    write( 1, buffer, strlen( buffer ) );

    shmctl( yshmid, IPC_RMID, NULL);
    sprintf( buffer, "*** MAIN: Merge y Array shared memory successfully removed\n\n" );
    write( 1, buffer, strlen( buffer ) );

    shmdt((void *) outArr);
    sprintf( buffer, "*** MAIN: Merge Output Array shared memory successfully detached\n" );
    write( 1, buffer, strlen( buffer ) );

    shmctl( outshmid, IPC_RMID, NULL);
    sprintf( buffer, "*** MAIN: Merge Output Array shared memory successfully removed\n" );
    write( 1, buffer, strlen( buffer ) );

    return 0;
}

// ----------------------------------------------------------- 
// FUNCTION  Prints the Array : (printArray)                          
//    Prints the array given
// PARAMETER USAGE :                                           
//    m : The number of integers in the array
//    array: the array to print               
//    type: 0 for input qsort array
//          1 for input x array
//             2 for input y array
//             3 for sorted qsort array
//             4 for sorted merged array
// FUNCTION CALLED :                                           
//    None
// ----------------------------------------------------------- 
int printArray( int array[], int m, int type ) {
    int i = 0, lineCount = 0;
    char buffer[(m*5)+50];

    if( type == 0 )
        sprintf( buffer, "Input array for qsort has %d elements:\n", m );
    if( type == 1 )
        sprintf( buffer, "Input array x[] for merge has %d elements:\n", m );
    if( type == 2 )  
        sprintf( buffer, "Input array y[] for merge has %d elements:\n", m );
    if( type == 3 )  
        sprintf( buffer, "*** MAIN: sorted array by qsort:\n" );
    if( type == 4 ) 
        sprintf( buffer, "*** MAIN: merged array:\n" );

    sprintf( buffer + strlen( buffer ), "  " );
    for( i = 0; i < m; i++ ) {
        //Keeps each line under 80
        if( lineCount > 17 ) { 
            sprintf( buffer + strlen( buffer ), "\n  " );
            lineCount = 0;    
        }

        sprintf( buffer + strlen( buffer ), "%4d", array[i] );

        lineCount+=1;
    }

    //Prints the final buffer into stdout
    sprintf( buffer + strlen( buffer ), "\n\n" );
    write( 1, buffer, strlen( buffer ) );
}
