/**************************************************************
* Class:  CSC-415-03 Summer 2021
* Name:Dinesh Thapa
* Student ID:920879242
* GitHub ID:dthapa770
* Project: Assignment 3 – Simple Shell
*
* File: Thapa_Dinesh_HW3_main.c
*
* Description: To build a simple shell using fork and execvp.
*
**************************************************************/
#include <stdio.h> 
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h> 

#define BUFFER_SIZE 512 
char *buffer; 

char *tokens[BUFFER_SIZE / 2]; 

 void readCmd( char* Buffer){

  int i = 0;
  if( buffer ==NULL){
    printf("Empty Buffer : error(%s)", strerror(errno));
    exit(1);
  }
 /*gets a string from the command line input. if checks for EOF*/
  if (fgets (buffer, BUFFER_SIZE, stdin)){
 /* exits when ctrl+D, initially thought it was the EOF we need.
    kept it for extra feature */
  if (feof(stdin)){
    printf("\n");
    exit(0);
  }
 
  /*first call to strtok string parsed must be str,
  i.e  buffer,returns null terminator at the end of string. */
  tokens[0] = strtok(buffer, " \t\r\n\a");
  
 
  if( tokens[0] == NULL){
    printf("Error\n");
    return;
  }
   /*successfull compare returns 0.*/
   if( strcmp(tokens[0],"exit") == 0){
    exit(0);
   }
  /* fill argument array after breaking up line of character in prompt shell.
  In each subsequent call that should parse the same string, str must be NULL. */
  while(tokens[i] != NULL ) {
    i++;
    tokens[i] = strtok(NULL, " \t\r\n\a");
  }
 
}
else {
printf("EOF reached. Thank You For Using Simple Shell.\n");
exit(0);
}
}

int main(int argc, char* argv[]) {

buffer = malloc(BUFFER_SIZE);

while(1){

    if( argv[1] != NULL ){
     printf("%s",argv[1]);
     }
     else {
     printf(">");
     }
 
   readCmd(buffer);
  
   int stat;
   /*calling new child process*/
   pid_t res = fork();

/*if fails to fork, -1 is returned in the parent*/
if ( res<0){
  printf("Error in Fork %d",errno);
  exit(errno);
}
/*if child is created successfully, 0 is returned in child*/
   if( res  == 0){
     /* execvp will replace current process with new process
     int execvp (const char *file, char *const argv[]);
     file is file beinf executed, argv is the null terminated array pointers.
     the list of argument must be terminated by null pointer.*/
     execvp(tokens[0], tokens);
     /* This line will not print if execvp runs successfull */   
              printf("Unsuccessfull child process. Error: %d\n", errno);
              exit(0);                  
              }
   else {
            /*run by parents.
				    wait will make sure that child dies first. */
            wait(&stat);
            /*WIFEXITED will check if the child process died successfully*/
            if (WIFEXITED(stat)) { 
            printf("Child %d finished  with status: %d\n", res, WEXITSTATUS(stat));
            }
        }
}
    free(buffer); //free the buffer
    return 0; 
}
