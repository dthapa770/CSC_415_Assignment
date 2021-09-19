/**************************************************************
* Class:  CSC-415-0# Summer 2021
* Name:Dinesh Thapa
* Student ID: 920879242
* GitHub Name:dthapa770
* Project: Thapa_Dinesh_HW01, Assignment 1 – Command Line Arguments
*
* File: Thapa_Dinesh_HW01.c
*
* Description: The program should display the number of arguments from the command line and list each one on the console.
*
**************************************************************/
#include <stdio.h>

int main( int argc, char *argv[] ){
   //displays numbers of argument
    printf("\nThere were %d arguments in the command line.\n",argc);

    //for loop used to print passed argument
    for( int i=0; i<argc ; i++){
        printf("Argument %02d: %s\n",i,argv[i]);
    }
    return 0;
}
