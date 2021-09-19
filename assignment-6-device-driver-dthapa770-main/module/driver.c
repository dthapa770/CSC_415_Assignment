/**************************************************************
* Class:  CSC-415-01 Summer 2021
* Name:Dinesh Thapa
* Student ID:920879242
* GitHub ID:dthapa770
* Project: Assignment 6 – Device-Driver
*
* File: driver.c
*
* Description:a skeleton device driver which an be loaded and run in Linux,
*             capable of recognizing palindrome numbers.           
* Template reference:http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
**************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioctl.h>

#define DEVICE_NAME "palindrome_dev"
#define CLASS_NAME "palindrome_mod"
#define WR_VALUE _IOW('a', 'a', int)
#define RD_VALUE _IOR('a', 'b', int)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dinesh Thapa");
MODULE_DESCRIPTION("Linux Device Driver");

static int majorNumber;          //Stores the device number -- determined automatically
static char message[256] = {0};  //Memory for the string that is passed from userspace
static short size_of_message;    //Used to remember the size of the string stored
static int value = 0;            //
static int result = 0;
static struct class *palindrome_dev_class = NULL;   //The device-driver class struct pointer
static struct device *palindrome_dev = NULL;        //The device-driver device struct pointer

// Prototype functions for the palindrome driver
static int dev_open(struct inode *, struct file *);
static int dev_release(struct inode *, struct file *);
static ssize_t dev_read(struct file *, char *, size_t, loff_t *);
static ssize_t dev_write(struct file *, const char *, size_t, loff_t *);
static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
static int dev_palindrome(int number);

//panlindrome devices will implement open, read, write, ioctl and release calls
static struct file_operations fops =
    {
        .open = dev_open,
        .read = dev_read,
        .write = dev_write,
        .unlocked_ioctl = dev_ioctl,
        .release = dev_release,
};

//static keyword restrict visibility of function to within C class
// Initializes device module with magic number and registers class
//returns 0 is initilatzation is seccessful
static int __init dev_mod_init(void)
{
   printk(KERN_INFO "palindrome_dev_mod: Initializing the palindrome_dev_mod LKM\n");

   //dynamically allocate a major number for the device
   majorNumber = register_chrdev(0, DEVICE_NAME, &fops);
   if (majorNumber < 0)
   {
      printk(KERN_ALERT "palindrome_dev_mod: Failed to register a major number\n");
      return majorNumber;
   }
   printk(KERN_INFO "palindrome_dev_mod: Registered correctly with major number %d\n", majorNumber);

   //register the device class
   palindrome_dev_class = class_create(THIS_MODULE, CLASS_NAME);
   if (IS_ERR(palindrome_dev_class))
   { 
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "palindrome_dev_mod: Failed to register device class\n");
      return PTR_ERR(palindrome_dev_class);
   }
   printk(KERN_INFO "palindrome_dev_mod: Device class registered correctly\n");

   //register the device driver
   palindrome_dev = device_create(palindrome_dev_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
   if (IS_ERR(palindrome_dev))
   {                                 
      class_destroy(palindrome_dev_class); 
      unregister_chrdev(majorNumber, DEVICE_NAME);
      printk(KERN_ALERT "palindrome_dev_mod: Failed to create the device\n");
      return PTR_ERR(palindrome_dev);
   }
   printk(KERN_INFO "palindrome_dev_mod: Device class created correctly\n");
   
   return 0;
}

// LKM cleanup function
static void __exit palindrome_dev_mod_exit(void)
{
   device_destroy(palindrome_dev_class, MKDEV(majorNumber, 0)); //remove device
   class_unregister(palindrome_dev_class);                      //unregister device class                     
   class_destroy(palindrome_dev_class);                         //remove device class                       
   unregister_chrdev(majorNumber, DEVICE_NAME);                 //unregister major number          
   printk(KERN_INFO "palindrome_dev_mod: Goodbye from the LKM\n");
}

// Called when device is opened
//inodep is pointer to inode object
//filep is pointer to file object
static int dev_open(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO "palindrome_dev_mod: Device opened\n");
   return 0;
}

// called when device is being read from user space
static ssize_t dev_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int error_count = 0;
   //send the buffer string to the user and captures any errors.
   error_count = copy_to_user(buffer, message, size_of_message);
   if (error_count == 0)          // if true then have success
   { 
      printk(KERN_INFO "palindrome_dev_mod: Sent %d characters to the user\n", size_of_message);
      return (size_of_message = 0); // clear the position to the start and return 0
   }
   else
   {
      printk(KERN_INFO "palindrome_dev_mod: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;               // Failed -- return a bad address message (i.e. -14)
   }
}

// called when device is being written from user space
static ssize_t dev_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   sprintf(message, "%s", buffer);
   size_of_message = strlen(message);
   printk(KERN_INFO "palindrome_dev_mod: Received %zu characters from the user\n", len);
   return len;
}

// IOCTL function, on write receives from user, on read sends to user
static long dev_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
   switch (cmd)
   {
   case WR_VALUE:
      copy_from_user(&value, (int32_t*) arg, sizeof(value));
      printk(KERN_INFO "palindrome_dev_mod: Received value %d from the user\n", value);
      break;
   case RD_VALUE:
      result = dev_palindrome(value);
      copy_to_user((int32_t*) arg, &result, sizeof(result));
      printk(KERN_INFO "palindrome_dev_mod: Sent palindrome result %d to the user\n", result);
      break;
   }

   return 0;
}

// function that determines palindrome number, implemented in IOCTL function
static int dev_palindrome(int number)
{
   // declare variables
  int temp, remainder, rev=0;
  // copy of original number
  temp = number;
  // loop to repeat
  while( number!=0 )
  {
     // find last digit
     remainder = number % 10;
     // calculate reverse
     rev = rev*10 + remainder;
     // remove last digit
     number /= 10;
  }
  if ( rev == temp ) return 1;
  else return 0;
 
}

// called whenever the device is closed/released by the userspace program
static int  dev_release(struct inode *inodep, struct file *filep)
{
   printk(KERN_INFO "palindrome_dev_mod: Device successfully closed\n");
   return 0;
}

module_init(dev_mod_init);
module_exit(palindrome_dev_mod_exit);
