// ----------------------------------------------------------- 
// NAME : Kyler Witting                         User ID: kmwittin 
// DUE DATE : 02/26/2016                                       
// PROGRAM ASSIGNMENT 2                                        
// FILE NAME : merge.c
// PROGRAM PURPOSE :                                           
// 1. When merge.c  runs, it receives whatever command line arguments.
// 
// 2. Then, it creates m+n child processes, each of which is 
// assigned to an element of x[ ] or an element of y[ ].
// 
// 3. Each child process carries out a modified binary 
// search as discussed earlier to determine the final
// location of the assigned element in the sorted array.
// 
// 4. When the final location is found, the process 
// writes its assigned value into that location of the output
// array.
// 
// 5. merge.c  waits until all of its child processes 
// exit, and terminates itself.
//
// ----------------------------------------------------------- 

#include <stdio.h>      //Needed for sprintf
#include <string.h>     //Needed for strlen
#include <sys/types.h>  //Has all system data types
#include <unistd.h>     //Declares standard symbolic constants and types
#include <sys/wait.h>   //The wait command 
#include <sys/shm.h>    //For Shared Memory
int main( int argc, char **argv ) {
    //Shared mem
    int *xArr, *yArr, *outArr, shmid;
    key_t key;

    //cmd var
    int xSize, ySize, totalSize;

    //fork var
    pid_t child;
    int status;

    //Binary Merge Var
    int i = 0, j = -1, k = 0, temp = 0;
    int xORy = 0;           //0 for x and 1 for y

    //sprintf buffer 
    char buffer[512];

    //cmd arg set up
    xSize     = atoi( argv[1] );
    ySize     = atoi( argv[2] );
    totalSize = atoi( argv[3] );



    //Shared Mem Set up
    key = ftok("./", 'x');
    shmid = shmget( key, xSize*sizeof(int), 0666 );
    if (shmid < 0) {
        sprintf(buffer, "      $$$Shmget Error. Now exiting.\n");
        write( 1, buffer, strlen( buffer ) );
        return -1;
    }
    xArr = ( int * )shmat( shmid, NULL, 0 );
    //TODO: Error Checking 

    key = ftok("./", 'y');
    shmid = shmget( key, ( ySize )*sizeof(int), 0666 );
    if (shmid < 0) {
        sprintf(buffer, "      $$$Shmget Error. Now exiting.\n");
        write( 1, buffer, strlen( buffer ) );
        return -1;
    }
    yArr = ( int * )shmat( shmid, NULL, 0 );
    //TODO: Error Checking 

    key = ftok("./", 'o');
    shmid = shmget( key, ( totalSize )*sizeof(int), 0666 );
    if (shmid < 0) {
        sprintf(buffer, "      $$$Shmget Error. Now exiting.\n");
        write( 1, buffer, strlen( buffer ) );
        return -1;
    }
    outArr = ( int * )shmat( shmid, NULL, 0 );
    //TODO: Error Checking 

    //Creates a child for each of the elements of x and y array 
    //Error checking if the fork fails 
    for( i = 0; i < totalSize; i++ ) { 
        j++;
        if( j >= xSize ) {
            j = 0;
            xORy = 1;
        }

        if ( ( child = fork() ) == 0 ) {
            //For putting x array into output array 
            if( !xORy ) { 
                temp = xArr[j];
                sprintf( buffer, "      $$$ M-PROC(%d): handling x[%d] = %d\n", getpid(), j, temp );
                write( 1, buffer, strlen( buffer ) );

                //Put x into spot j
                if( temp < yArr[0] ){
                    sprintf( buffer, "      $$$ M-PROC(%d): x[%d] = %d is found to be smaller than y[%d] = %d\n", getpid(), j, temp, 0, yArr[0] );
                    write( 1, buffer, strlen( buffer ) );
                    sprintf( buffer, "      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", getpid(), j, temp, j );
                    write( 1, buffer, strlen( buffer ) );
                    outArr[j] = temp;    
                    return 1;
                }
                //put x into spot j+(ySize-1)
                if( temp > yArr[ySize-1] ){
                    sprintf( buffer, "      $$$ M-PROC(%d): x[%d] = %d is found to be larger than y[%d] = %d\n", getpid(), j, temp, ySize-1, yArr[ySize-1] );
                    write( 1, buffer, strlen( buffer ) );
                    sprintf( buffer, "      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n", getpid(), j, temp, j+(ySize-1) );
                    write( 1, buffer, strlen( buffer ) );
                    outArr[j+ySize] = temp;
                    return 1;
                }
                //put x into spot j+k
                for( k = 0; k < totalSize; k++ ){ 
                    if( temp < yArr[k] && temp > yArr[k-1] ){

                        sprintf( buffer, "      $$$ M-PROC(%d): x[%d] = %d is found between y[%d] = %d and y[%d] = %d\n", getpid(), j, temp, k-1, yArr[k-1], k, yArr[k] );
                        write( 1, buffer, strlen( buffer ) );
                        sprintf( buffer, "      $$$ M-PROC(%d): about to write x[%d] = %d into position %d of the output array\n\n", getpid(), j, temp, j+k );
                        write( 1, buffer, strlen( buffer ) );
                        outArr[j+k] = temp;
                        return 1;
                    }

                }
            }
            //For putting y array into the output array 
            else { 
                temp = yArr[j];
                sprintf( buffer, "      $$$ M-PROC(%d): handling y[%d] = %d\n", getpid(), j, temp );
                write( 1, buffer, strlen( buffer ) );

                //Put y into spot j
                if( temp < xArr[0] ){
                    sprintf( buffer, "      $$$ M-PROC(%d): y[%d] = %d is found to be smaller than x[%d] = %d\n", getpid(), j, temp, 0, xArr[0] );
                    write( 1, buffer, strlen( buffer ) );
                    sprintf( buffer, "      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n", getpid(), j, temp, j );
                    write( 1, buffer, strlen( buffer ) );
                    outArr[j] = temp;    
                    return 1;
                }
                //put y into spot j+(xSize-1)
                if( temp > xArr[xSize-1] ){
                    sprintf( buffer, "      $$$ M-PROC(%d): y[%d] = %d is found to be larger than x[%d] = %d\n", getpid(), j, temp, xSize-1, xArr[xSize-1] );
                    write( 1, buffer, strlen( buffer ) );
                    sprintf( buffer, "      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n", getpid(), j, temp, j+(xSize-1) );
                    write( 1, buffer, strlen( buffer ) );
                    outArr[j+xSize] = temp;
                    return 1;

                }
                //put y into spot j+k
                for( k = 0; k < totalSize; k++ ){ 
                    if( temp < xArr[k] && temp > xArr[k-1] ){

                        sprintf( buffer, "      $$$ M-PROC(%d): y[%d] = %d is found between x[%d] = %d and x[%d] = %d\n", getpid(), j, temp, k-1, xArr[k-1], k, xArr[k] );
                        write( 1, buffer, strlen( buffer ) );
                        sprintf( buffer, "      $$$ M-PROC(%d): about to write y[%d] = %d into position %d of the output array\n\n", getpid(), j, temp, j+k );
                        write( 1, buffer, strlen( buffer ) );
                        outArr[j+k] = temp;
                        return 1;
                    }

                }
            }
            return 1;
        }
        //Error checking for fork()
        if( child < 0 ) {
            sprintf( buffer, "      $$$MAIN: merge fork() failed.\n" );
            write( 2, buffer, strlen( buffer ) );
            return -1;
        } 
    }

    //Parent waits for all children to complete 
    //Then exits and closes itself 
    for( i = 0; i < totalSize; i++ ) { 
        wait(&status);
    }
    return 1;
}
