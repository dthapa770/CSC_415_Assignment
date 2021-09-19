
/**************************************************************
* Class:  CSC-415-01 Summer 2021
* Name: Dinesh Thapa
* Student ID: 920879242
* GitHub UserID:dthapa770
* Project: Assignment 2 – Stucture in Memory and Buffering
*
* File: Thapa_Dinesh_HW2_main.c
*
* Description:
     In this Assignment ,main function receive command l
     line argments then,fill the personalInfo object with it
     and write it then start receeveing and committing
     strings untill it hits NULL
**************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "assignment2.h"

int main(int argc, char *argv[]){
	const char *next;
	char buffer[BLOCK_SIZE];

	/*allocating memory to object of type personalInfo*/
	personalInfo *myInfo=(personalInfo *)malloc(sizeof(personalInfo));

	/*if memmory  allocation failed*/
	if(myInfo==NULL){
		printf("error occured while instantiaing  personl info\n");
	}
    
	/*To assign values in struct -> is used , here we assign firstname, lastname 
          which are provided by the command line argument and assign  values to 
          variables student id and level*/
	myInfo->firstName=argv[1];
	myInfo->lastName=argv[2];
	myInfo->studentID=920879242;
	myInfo->level=SENIOR;

	/* bit wise or the mask of all languaes and add to it to personal info */
	myInfo->languages =KNOWLEDGE_OF_C | KNOWLEDGE_OF_JAVA | KNOWLEDGE_OF_JAVASCRIPT;

	/*copying meesage from command line arg to personalINfo*/
	strcpy(myInfo->message,argv[3]);

	/*writing personal info*/
	int r=writePersonalInfo (myInfo); 	
	if(r!=0){
		printf("error occured while writting personl info\n");
		return r;
	}

	int len=0;
	int i=0;

	/*get the first string*/
	next= getNext();

	/*keep looping untill NULL is recieved, get the length of the received string,
      fill the block buffer with recieved bytes then a single character is copied
      at a time. Check if buffer is filled, if filled then commit buffer, empty
      the buffer and set the index to 0 */
	while(next!=NULL){	
		len=strlen(next);		
		for(int k=0;k<len;k++){			
			buffer[i++]=next[k];			
			if(i==256){				
				commitBlock(buffer);				
				strcpy(buffer,"");
				i=0;
			}
		}
		/*get next string*/
		next=getNext();
		
	}

	/*if NULL recieved and last filled buffer was not 
	commited as it was not full filled*/
	if(i!=0){
		commitBlock(buffer);
	}

	/*calling check to print the hexadecimal characters*/
	r=checkIt();
	
	/*freeing memory*/
	free(myInfo);
	
	return r;
								
    
}
