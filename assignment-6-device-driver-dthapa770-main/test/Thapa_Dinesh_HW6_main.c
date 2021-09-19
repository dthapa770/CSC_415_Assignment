/**************************************************************
* Class:  CSC-415-01 Summer 2021
* Name:Dinesh Thapa
* Student ID:920879242
* GitHub ID:dthapa770
* Project: Assignment 6 – Device-Driver
*
* File: Thapa_Dinesh_HW06.c
*
* Description:a user application that utilizes my device driver
*                       
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define BUFFER_LENGTH 256
#define RD_VALUE _IOR('a', 'b', int)
#define WR_VALUE _IOW('a', 'a', int)

static char stringRecieved[BUFFER_LENGTH];  // The receive buffer from the LKM


int main()
{
   int ret, fd;
   char inputString[BUFFER_LENGTH];
   int value, number;

   printf("--------Start Program--------\n");

   // Open the device with read/write access
   fd = open("/dev/palindrome_dev", O_RDWR);
   if (fd < 0)
   {
      perror("ERROR: Failed to open the device.");
      return errno;
   }

   printf("Enter the integer to check : ");
   scanf("%s", inputString);                   //The receive buffer from the LKM

   // Send the string to LKM
   ret = write(fd, inputString, strlen(inputString));
   if (ret < 0)
   {
      perror("ERROR: Failed to write to the device.");
      return errno;
   }

   // Read the response from LKM
   ret = read(fd, stringRecieved, BUFFER_LENGTH);
   if (ret < 0)
   {
      perror("ERROR: Failed to read from the device.");
      return errno;
   }

   printf("The value you entered: %s\n", stringRecieved);

   // Convert the string to an int
   number = atoi(stringRecieved);

   // Checking for error
   if (number == 0)
   {
      printf("ERROR: Invalid entry of '%s'.\n", stringRecieved);
      printf("--------End Program--------\n");
      close(fd);
      return 0;
   }

   // Send int to kernel mod using ioctl()
   ret = ioctl(fd, WR_VALUE, &number);
   if (ret < 0)
   {
      perror("ERROR: Failed to write to the device.");
      return errno;
   }

   // Read  value from kernel mod using ioctl()
   ret = ioctl(fd, RD_VALUE, &value);
   if (ret < 0)
   {
      perror("ERROR: Failed to read from the device.");
      return errno;
   }

   //printf("------%d\n",value);
   if ( value == 1){
      printf("The %d number  is  palindrome \n", number);
   } else {
      printf("The %d number  is not palindrome \n", number);
   }
  
   printf("--------End Program--------\n");
   close(fd);

   return 0;
}