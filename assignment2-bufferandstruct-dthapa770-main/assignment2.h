
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


#define KNOWLEDGE_OF_C				1
#define KNOWLEDGE_OF_JAVA			2
#define KNOWLEDGE_OF_JAVASCRIPT 		4
#define KNOWLEDGE_OF_PYTHON			8
#define KNOWLEDGE_OF_CPLUSPLUS		16
#define KNOWLEDGE_OF_PASCAL			32
#define KNOWLEDGE_OF_FORTRAN		64
#define KNOWLEDGE_OF_RUBY			128
#define KNOWLEDGE_OF_ADA			256
#define KNOWLEDGE_OF_LISP			512
#define KNOWLEDGE_OF_SQL			1024
#define KNOWLEDGE_OF_HTML			2048
#define KNOWLEDGE_OF_SWIFT			4096
#define KNOWLEDGE_OF_PROLOG			8192
#define KNOWLEDGE_OF_C_SHARP		16384
#define KNOWLEDGE_OF_PL1			32768
#define KNOWLEDGE_OF_INTEL_ASSEMBLER	65536
#define KNOWLEDGE_OF_IBM_ASSEMBLER	131072
#define KNOWLEDGE_OF_MIPS_ASSEMBLER	262144
#define KNOWLEDGE_OF_ARM_ASSEMBLER	524288
#define KNOWLEDGE_OF_COBOL			1048576
#define KNOWLEDGE_OF_APL			2097152
#define KNOWLEDGE_OF_R				4194304
#define KNOWLEDGE_OF_OBJECTIVE_C	8388608
#define KNOWLEDGE_OF_BASIC			16777216
#define KNOWLEDGE_OF_PHP			33554432
#define KNOWLEDGE_OF_GO				67108864

typedef struct personalInfo 
    {
    char * firstName;
    char * lastName;
    int studentID;
    enum gradelevel {FRESHMAN, SOPHMORE, JUNIOR, SENIOR, GRAD, INSTRUCTOR} level;
    int languages; // See #defines for the bitmap values
    char message[100];
    } personalInfo;
    
#define BLOCK_SIZE					256

int writePersonalInfo (personalInfo * pi);  //Write your personal info structure
const char * getNext(void);					//Get the next line to buffer write
void commitBlock (char * buffer);			//Flush out your 256 byte Buffer
int checkIt (void);							//Called at the end of your program to check the results


