/**************************************************************
* Class:  CSC-415-03 Summer 2021
* Name:Dinesh Thapa
* Student ID:920879242
* GitHub ID:dthapa770
* Project: Assignment 4 – Word Blast
*
* File: Thapa_Dinesh_HW4_main.c
*
* Description: To read War and Peace  and it is to count and 
*              tally each of the words that are 6 or more 
*              characters long.
*
**************************************************************/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define MAXCOUNTWORDS 2500 
#define TOPWORDS 10  
// You may find this Useful
char * delim = "\"\'.“”‘’?:;-,—*($%)! \t\n\x0A\r";

int count = 0;     
int setFlag = 0;  
int excessBytes = 0;  
int fileDescriptor, blockSize;
pthread_mutex_t lock;

//Struct to hold word and it's number of occurence
typedef struct wordsData {
    char * word;
    int count;
} wordsData;

//struct object to hold array of words
struct wordsData wordArray[MAXCOUNTWORDS];

// this function is used by threads to process file in blocks
void *wordProcess(void *ptr) {  
    int result;
    char * buffer;
    char * token;
    // allocatee buffer with excess bytes
    buffer = malloc(blockSize+excessBytes);

    //Read will attempt to read upto blockSize+excessBytes
    //from file descriptor into the buffer
    read(fileDescriptor, buffer, blockSize+excessBytes);

    // loop through buffer and parse strings into sequence of tokens
    while (token = strtok_r(buffer, delim, &buffer)) {  
        // check if token is greater or equal to  6 character,
        if ((int)strlen(token) >= 6) {  
            //loop through wordarray to check token 
            for (int i = 0; i < MAXCOUNTWORDS; i++) {
                //strcmp will comapare word in wordarray with token
                //if matched sucessfully, returns 0
                result = strcasecmp(wordArray[i].word, token);
                //if match result is 0 and increase count
                if (result == 0) {
                    //avoids race condition, critical section
                    pthread_mutex_lock(&lock);   
                    wordArray[i].count++;
                    // End critical section
                    pthread_mutex_unlock(&lock); 
                    break;
                } 
            }
            // if no match found
            if (result != 0) {
                if (count < MAXCOUNTWORDS) {
                    // avoids race condition, critical section
                    pthread_mutex_lock(&lock); 
                    //insert token to word array           
                    strcpy(wordArray[count].word, token);
                    wordArray[count].count++;
                    // End critical section
                    pthread_mutex_unlock(&lock);  
                    //increment count        
                    count++;
                }
            }
        }
    }
}



int main (int argc, char *argv[])
    {
    //***TO DO***  Look at arguments, open file, divide by threads
    //             Allocate and Initialize and storage structuresult

     int r1, r2, fileSize;
    // thread safe atoi changes string args to integer
    int numOfThreads = atoi(argv[2]);  
    
    // if flag =0, initialize array and set flag to 1
    if (setFlag == 0) {
         for (int i = 0; i < MAXCOUNTWORDS; i++) {
        wordArray[i].word = malloc(TOPWORDS);
        wordArray[i].count = 0;
         }
        setFlag = 1;
    }
    
    // initialize mutex lock and check for errors
    if (r1 = pthread_mutex_init(&lock, NULL)) {
        printf("ERROR: Mutex init failed [%d]\n", r1);
    }
     
    // open file from given argument
    fileDescriptor = open(argv[1], O_RDONLY);
    // lseek will return file size
    fileSize = lseek(fileDescriptor, 0, SEEK_END);
    // he lseek will set file position back to start
    lseek(fileDescriptor, 0, SEEK_SET);
    //block size depends on number of threads
    blockSize = fileSize / numOfThreads;

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Time stamp start
    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_REALTIME, &startTime);
    //**************************************************************
    // *** TO DO ***  start your thread processing
    //                wait for the threads to finish
     // Declare pthread with number of threads
    pthread_t thread[numOfThreads];

    for (int i = 0; i < numOfThreads; i++) {
        //checking for last threads to adjust excess bytes with leftovers
        if (i == numOfThreads-1) {
            excessBytes = fileSize % numOfThreads;
        }
        // start new thread in calling process with some error checking
        if (r2 = pthread_create(&thread[i], NULL, wordProcess, (void*) &i)) {
            printf("ERROR: Thread creation failed [%d]\n", r2);
            exit(EXIT_FAILURE);
        }
    }

    // blocks the execution of calling threads
    //until target threads terminates
    // acts like wait()
    for (int i = 0; i < numOfThreads; i++) {
        pthread_join(thread[i], NULL);
    }

    // ***TO DO *** Process TOP 10 and display
    //temp variable
    wordsData temp;

    // loop will sort in descending order
    for (int i = 0; i < MAXCOUNTWORDS; i++) {
        for (int j = i+1; j < MAXCOUNTWORDS; j++) {
            if (wordArray[i].count < wordArray[j].count) {
                temp = wordArray[i];
                wordArray[i] = wordArray[j];
                wordArray[j] = temp;
            }
        }
    }

    printf("\n\n");
    printf("Word Frequency Count on %s with %d threads\n", argv[1], numOfThreads);
    printf("Printing top 10 words 6 characters or more.\n");

    // ***TO DO *** Process TOP 10 and display
      for (int i = 0; i < 10; i++) {
        printf("Number %d is %s with a count of %d\n", i+1, wordArray[i].word, wordArray[i].count);
    }

    //**************************************************************
    // DO NOT CHANGE THIS BLOCK
    //Clock output
    clock_gettime(CLOCK_REALTIME, &endTime);
    time_t sec = endTime.tv_sec - startTime.tv_sec;
    long n_sec = endTime.tv_nsec - startTime.tv_nsec;
    if (endTime.tv_nsec < startTime.tv_nsec)
        {
        --sec;
        n_sec = n_sec + 1000000000L;
        }

    printf("Total Time was %ld.%09ld seconds\n", sec, n_sec);
    //**************************************************************


    // ***TO DO *** cleanup
     // do necessary closing of the file
    close(fileDescriptor);
    // must destroy lock
    pthread_mutex_destroy(&lock);
    // Free  buffers
    for (int i = 0; i < MAXCOUNTWORDS; i++) {
        free(wordArray[i].word);
    }
    return 0;
    }
