/************************************************************** * Class:
 *CSC-415-0# Spring 2020 Name: Michael John Canson Student ID:920601003
 * Project:Assignment 2 - Word Blast
 * File: threads2.c
 *
 * Description:
 *
 **************************************************************/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t lock;
static int testNum = 0;
static int test = 0;

typedef struct Thread_Args {
    long start;
    long end;
    int T;
    char FileName[20];
} threadArgs;

typedef struct WordNode {
    char Word[20];
    int count;
    struct WordNode* next; 

} wNode;

wNode* head = NULL;

int getFrequencyOf(char* word) {
    
    // pthread_mutex_lock(&lock);

    int frequency = 0;

    wNode* temp = head;

    while(temp != NULL ) {
                // printf("here\n"); 

        if(temp->Word == word) {
                pthread_mutex_lock(&lock);

            frequency++;
                pthread_mutex_unlock(&lock);
            // printf("word: %s freq: %d\n",word,frequency);
        }
        temp = temp->next;
    }

    // pthread_mutex_unlock(&lock);

    return frequency;
}

void addToThreadBag (char* word) {

    wNode *node = malloc (sizeof(wNode));

    // pthread_mutex_lock(&lock);
    strcpy(node->Word,word);
        node->next = NULL;
  
    node->next = head; // adds new node infront
    head = node;  
    //  printf("new node copied word: %s \n", node->Word);
        // printf("node[%s]->next: %s\n",node->Word,node->next->Word);
}





// void addToWordBag(char* word) {


// }

void *FileReader(void *targs) {

    // pthread_mutex_lock(&lock);

    char delims[] = " “,()-[]#/.;:!?\"\t\n\a\r";
    int length;
    // pthread_mutex_lock(&lock);

    threadArgs *args = (threadArgs *)targs;

    long s = (long)args->start;

    long e = (long)args->end;

    char *Fname = (char *)args->FileName;

    int T = (int) args->T;
    // printf("Fname: %s\n", Fname);  // TEST


    char *buffer = 0;
    FILE *f = fopen(Fname, "r");
    // pthread_mutex_unlock(&lock);

    if (f == NULL) {
        printf("Error opening file\n");
        exit(1);
    }

    if (f) {
       
        long size = e - s;

        fseek(f, s, SEEK_SET);

        buffer = malloc(size);

        if (buffer) {
            fread(buffer, 1 , size, f);
        }

        fclose(f);
    }

    if (buffer) {

        char *rest = buffer;
        char *ptr = strtok_r(rest, delims, &rest);

        while (ptr != NULL) {

            if(strlen(ptr) >= 6 ) {
                    addToThreadBag(ptr);
                    // printf("HERE!@\n");
                    // if(strcasecmp(ptr,"pierre")==0) {
                    // printf("THREAD[%d] |> %s|\n", testNum++, ptr);
                    // }
            }

            ptr = strtok_r(rest, delims, &rest);
        }

        free(rest);
    }

    // pthread_mutex_unlock(&lock);

    return 0;
}

// Takes text file name and number of desired thread(s) to work on file
void ThreadMaker(char *FileName, long threadCount) {
    
    int nThreads = threadCount;
    printf("nThreads: %d\n", nThreads);  // number of threads

    pthread_t
        threads[nThreads];  // TODO maybe threads doesnt need to be an array

    threadArgs *tArgs;
    long length = 0;

    FILE *f = fopen(FileName, "r");
    if(f == NULL) {
        printf("error!!!");
    }

    if (f) {
        fseek(f, 0, SEEK_END);
        length = ftell(f);          

        long pLenght =length / nThreads;  
        fseek(f, 0, SEEK_SET);
        pthread_mutex_init(&lock, NULL);

        for (int i = 0; i < nThreads; i++) {

            tArgs = malloc(sizeof(threadArgs));

            strcpy(tArgs->FileName, FileName);

            tArgs->start = (long)i * pLenght;
            // printf("Start[%d]: %ld\n", i, tArgs->start);

            tArgs->end = (long)((i * pLenght) + pLenght);
            
            tArgs->T = (int)i; //TODO DELETE BEFORE SUBMITTING

            if (pthread_create(&threads[i], NULL, (void *)&FileReader,
                               (void *)tArgs) < 0) {

                printf("error creating thread!\n");
            }
            
        }

        for (int i = 0; i < nThreads; i++) {

            if (pthread_join(threads[i], NULL) == 0) {

            }
        }
    
        fclose(f);
    }
}

int main(int argc, char **argv) {
    struct timespec startTime;
    struct timespec endTime;
    clock_gettime(CLOCK_REALTIME, &startTime);

    char UserEntry[1024];
    char* args[500];
    char* nThreads;

        // printf("Enter FileName (name.txt) and number of threads: ");
        
        // if(fgets(UserEntry, sizeof(UserEntry), stdin)==NULL) {

        //  exit(EXIT_SUCCESS);

        // }

        // UserEntry[strlen(UserEntry) - 1] = '\0';

        // int i = 0;
        // args[0] = strtok(UserEntry, " ");
        // while (args[i] != NULL) {
        //     args[++i] = strtok(NULL, " ");
        // }
        //if( args[1] == "") {
        //      args[1] = "1"; 
        // }
        // int n = atoi(args[1]);

        // printf("%d\n",n);
        // printf("%s\n",args[0]);

    // ThreadMaker(args[0], n);

        ThreadMaker("WarAndPeace.txt",3);
    // ThreadMaker("example2.txt", 1);

    wNode* ptr = head;
    int i = 0;
    while(ptr != NULL) {
        i++;
        printf("node: %s\n" ,ptr->Word );
        ptr = ptr->next;
    }
     printf("Items: %d\n" ,i);

    // ThreadMaker("example.txt", 4);

    // YOUR MAIN CODE IN MAIN HERE
    // TEST FILE READING

    clock_gettime(CLOCK_REALTIME, &endTime);

    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;

    if (endTime.tv_nsec < startTime.tv_nsec) {
        --sec;
        n_sec = n_sec + 1000000000L;
    }
    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);

    return 0;
}