// ----------------------------------------------------------- 
// NAME : Kyler Witting                         User ID: kmwittin 
// DUE DATE : 02/26/2016                                       
// PROGRAM ASSIGNMENT 2                                        
// FILE NAME : qsort.c             
// PROGRAM PURPOSE :                                           
// 1. When qsort.c  runs, it receives the left and right section indices Left  and Right  and other
// information from its command line arguments.
//
// 2. Then, it partitions the array section a[Left..Right]  into two at element a[M] . 
// See your data structures and/or algorithms textbooks for this partitioning procedure. 
// After the partition is obtained two child processes are created, each of which 
// runs qsort.c  using system call execvp() . Thefirst child receives Left  
// and M ­1, while the second receives M +1 and Right . The parent then waits
// until both child processes complete their job.
// 
// 3. After this, program qsort.c  exits. 
//
// ----------------------------------------------------------- 

#include <stdio.h>      //Needed for sprintf
#include <string.h>     //Needed for strlen
#include <sys/types.h>  //Has all system data types
#include <unistd.h>     //Declares standard symbolic constants and types
#include <sys/wait.h>   //The wait command 
#include <sys/shm.h>    //For Shared Memory

int quicksort(  int k,       int left, int right );
int printArray( int array[], int l, int r ); 

int main( int argc, char **argv ) {
    //Shared mem
    int *array, shmid;
    key_t key;

    //cmd var
    int left, right, k;

    //execvp calls 
    char prog[7], *argv2[5], temp1[10], temp2[10], temp3[10];
    
    //fork var
    pid_t child;
    int status;

    //qsort vars
    int i, j, pivot, temp;
    
    //sprintf buffer 
    char buffer[256];

    //cmd arg set up
    k     = atoi( argv[1] );
    left  = atoi( argv[2] );
    right = atoi( argv[3] );


    //Shared Mem Set up
    key = ftok("./", 'q');
    shmid = shmget( key, k*sizeof(int), 0666 );
    if (shmid < 0) {
        sprintf(buffer, "   ###Shmget Error. Now exiting.\n");
        write( 1, buffer, strlen( buffer ) );
        return -1;
    }
    array = ( int * )shmat( shmid, NULL, 0 );

    //Actual Q sort finally 
    //Find pivot and put left/right into temp variables
    temp = left + (right - left ) / 2;
    pivot = array[ temp ];
    i = left; 
    j = right; 
    sprintf( buffer, "   ### Q-PROC(%d): : pivot element is array[%d] = %d\n", getpid(), temp, pivot );
    write( 1, buffer, strlen( buffer ) );
    printArray( array, i, j );
    //quicksort algo 
    while( i <= j ) {
        while( array[i] < pivot )
            i++;
        while( array[j] > pivot )
            j--;

        if( i <= j ) { 
            temp = array[i];
            array[i] = array[j];
            array[j] = temp;

            i++;
            j--;
        }
    }
   
   //"Recursive" calls using execvp 2 times 
    if( left < j ) {
        if ( ( child = fork() ) == 0 ) {
            sprintf( prog, "./qsort" );
            argv2[0] =   prog;
            sprintf( temp1, "%d", k );
            sprintf( temp2, "%d", left );
            sprintf( temp3, "%d", j );
            char *argv2[] = { prog, temp1, temp2, temp3, '\0'};

            execvp( prog, argv2 );
            sprintf( buffer, "   ###MAIN: Q Sort Execvp has Failed.\n" );
            write( 1, buffer, strlen( buffer ) );
            return -1; 
        }
        if( child < 0 ) {
            sprintf( buffer, "   ###MAIN: qsort fork() failed.\n" );
            write( 2, buffer, strlen( buffer ) );
        } 
    }
    if( i < right ) {
        if ( ( child = fork() ) == 0 ) {
            sprintf( prog, "./qsort" );
            argv2[0] =   prog;
            sprintf( temp1, "%d", k );
            sprintf( temp2, "%d", i );
            sprintf( temp3, "%d", right );
            char *argv2[] = { prog, temp1, temp2, temp3, '\0'};

            execvp( prog, argv2 );
            sprintf( buffer, "   ###MAIN: Q Sort Execvp has Failed.\n" );
            write( 1, buffer, strlen( buffer ) );
            return -1; 
        }
        if( child < 0 ) {
            sprintf( buffer, "   ###MAIN: qsort fork() failed.\n" );
            write( 2, buffer, strlen( buffer ) );
        } 
    }
    
    //waits for new processes to finish before exiting
    wait(&status);
    wait(&status);
    
    sprintf( buffer, "   ### Q-PROC(%d): exits\n", getpid() );
    write( 1, buffer, strlen( buffer ) );

    return 1;
}

// ----------------------------------------------------------- 
// FUNCTION  Prints the Array : (printArray)                          
//    Prints the array given
// PARAMETER USAGE :                                           
//    l : where to start on the left side of the array
//    r : where to end printing the array
//    array: the array to print               
// FUNCTION CALLED :                                           
//    None
// -----------------------------------------------------------
int printArray( int array[], int l, int r ) {
    int i = l, lineCount = 0;
    char buffer[(r-l)+100];

    sprintf( buffer, "   ### Q-PROC(%d): entering with array[%d..%d]\n   ", getpid(), l, r );

    sprintf( buffer + strlen( buffer ), "  " );
    for( i = l; i < r; i++ ) {
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
