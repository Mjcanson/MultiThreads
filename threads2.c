/************************************************************** * Class: CSC-415-0# Spring 2020
* Name: Michael John Canson
* Student ID:920601003
* Project:Assignment 2 - Word Blast
* File: threads2.c
*
* Description: 
*
**************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t lock; 

static int testNum = 0;

typedef struct Thread_Args {
    long start;
    long end;
    char FileName[20];
}threadArgs;

void* FileReader(void * targs) {
        pthread_mutex_lock(&lock); 

        printf(" ======== FileReader called: %d ========== \n",testNum++);

        printf("Hello World!!! \n");

    // // char buffer[] = 0;
    // char delims[] = " ,-[]#/.;:!?\t\n\a\r";
    // int i = 0;
    // int length;

    threadArgs* args = (threadArgs*) targs;
    
    long s = (long ) args->start;
    printf("s: %ld\n", s); //TEST
    
    long e = (long ) args->end;
    printf("e: %ld\n", e); //TEST

    char* Fname = (char *) args->FileName;
    printf("Fname: %s\n", Fname); //TEST

    pthread_mutex_unlock(&lock);


    // printf("========================\n");

    // free(args);

    // char * buffer = 0;
    // FILE * f = fopen(Fname,"r");
    // if(f == NULL) {
    //     printf("Error opening file\n");
    //     exit(1);
    // }

    // if(f) {
    //     printf("im here \n");
    //     fseek (f, 0, SEEK_END);
    //     // printf("Fseek: %ld\n", ftell(f));

    //     length = ftell(f);

    //     fseek (f, 0, SEEK_SET);
    //     // printf("Fseek2: %ld\n", ftell(f));

    //     buffer = malloc (length);  // shouldnt each thread have its own array in stack?

        
    //     if (buffer) {

    //         fread (buffer, s + 1, 20, f); //TODO MAKE SURE YOU GO OVER S+1
    //     }

    //     fclose (f);
    //     printf("File closed\n");

    // }

    // if (buffer) {
    //             printf("here \n");

    //     // start to process your data / extract strings here...
    //     char *ptr = strtok(buffer,delims);

    //     while (ptr != NULL) {
    //         printf("THREAD[%d] |> %s|\n", i++ , ptr);
    //         ptr = strtok(NULL, delims);
    //     }
        
    // }
    // printf("exited ebuffer. %d \n",i);

    return 0;
}

//Takes text file name and number of desired thread(s) to work on file
void ThreadMaker(char *FileName, long threadCount) {

    int nThreads = threadCount;
    printf("nThreads: %d\n",nThreads);
    pthread_t threads[nThreads - 1]; //TODO maybe threads doesnt need to be an array

     threadArgs *tArgs;

    // threadArgs tArgs;
    // strcpy(tArgs.FileName,FileName); //set Filename
    //     printf("FileName %s\n", tArgs.FileName); //TEST
    long length = 0;

    FILE * f = fopen(FileName,"r");

    if(f) {
        fseek (f, 0, SEEK_END);
        length = ftell(f); //total lenght of file
        printf("lenght: %ld\n" , length);

        long pLenght = length / nThreads; //how much of the file each thread should work on

        printf("pLenght: %ld\n" , pLenght);
        printf("========================\n");
        
        fseek (f, 0, SEEK_SET);
        pthread_mutex_init(&lock,NULL);

        for(int i = 0; i < nThreads; i++) {

            tArgs = malloc(sizeof(threadArgs));
                    printf("============BEGIN============\n");
            
            strcpy(tArgs->FileName,FileName); //set Filename
            printf("FileName %s\n", tArgs->FileName); //TEST

            tArgs->start =(long) i * pLenght;
            printf("Start[%d]: %ld\n", i , tArgs->start);

            tArgs->end = (long)((i * pLenght) + pLenght);
            printf("End[%d]: %ld\n", i , tArgs->end); // causing seg fault error once it gets to this
            
            //TODO deal with seg fault error 

            // pthread_mutex_init(&lock,NULL);

           if( pthread_create(&threads[i],NULL,(void *) &FileReader,(void *) tArgs) < 0) {
               printf("error creating thread!\n");
           }

            //  pthread_join(threads[i], NULL); 


            printf("THREAD[%d] CREATED\n", i); //TEST
                                printf("==============END==========\n");

            
            // free(tArgs); // deallocate 
        }

        for(int i = 0; i < nThreads; i++) {

            if(pthread_join(threads[i], NULL) == 0) {
                printf("Thread[%d] successfuly joined.\n", i);
            } 

        
        }

        

    fclose (f);
    printf("file Closed!\n");
    } 
}


int main(int argc, char** argv) {
    struct timespec startTime;
    struct timespec endTime;
    clock_gettime(CLOCK_REALTIME, &startTime);


    char line[] = "WarAndPeace.txt"; //test
     ThreadMaker(line,4);

    //YOUR MAIN CODE IN MAIN HERE
    //TEST FILE READING
  
    clock_gettime(CLOCK_REALTIME, &endTime);
    
    time_t sec = endTime.tv_sec -startTime.tv_sec;
    long n_sec = endTime.tv_nsec -startTime.tv_nsec;

    if (endTime.tv_nsec < startTime.tv_nsec) {
        --sec;
        n_sec = n_sec + 1000000000L;
    }
    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);

    return 0;
}