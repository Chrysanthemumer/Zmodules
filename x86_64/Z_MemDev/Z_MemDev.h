/*
 * Z_MemDev.h -- header for Memory Devices development modules
 *
 * version 1.0
 * 
 * Copyright (C) 2013 Sizhou Wang
 * Copyright (C) 2010 IIT
 * 
 * The source code in this file can be freely used, copied, adapted and 
 * redistributed in source or binary form, so long as an acknowledgement
 * appears in derived source files. The citation should list that the code 
 * comes from 'Sizhou Wang, Ilinois Institute of Technology'.
 * No warrenty is attached.
 */
 
#include <linux/cdev.h>
 
#ifndef _Z_MEMDEV_H_
#define _Z_MEMDEV_H_

#define Z_MEMDEV_MAJOR 0
#define Z_MEMDEV_MINOR 0
#define Z_MEMDEV_NUM 4

#ifndef MEMDEV_LIST_SIZE
#define MEMDEV_LIST_SIZE 4000
#endif

#ifndef MEMDEV_LIST_LENGTH
#define MEMDEV_LIST_LENGTH 1000
#endif

#ifndef Z_MEMDEV_LIST
#define Z_MEMDEV_LIST
typedef struct Z_MemDev_list {
	void **data;
	struct Z_MemDev_list *next;
}Z_MemDev_list;
#endif

#ifndef Z_MEMDEV_DEV
#define Z_MEMDEV_DEV
typedef struct Z_MemDev_dev {
	struct Z_MemDev_list *list;  
	int size;                   /* the current 2-dim array member size (2nd tier)*/
	int length;                 /* the current 2-dim array size (1st tier)*/
	unsigned long total_size;       
	//unsigned int access_key;  
	//struct semaphore sem;     
	struct cdev st_cdev;	          /* Char device structure		*/
}Z_MemDev_dev;
#endif

extern int MemDev_list_size;
extern int MemDev_list_length;
extern int MemDev_num;

extern struct Z_MemDev_dev * MemDev_devices;

// File Operations
int Z_MemDev_open(struct inode *inode, struct file *filp);
int Z_MemDev_release(struct inode *inode, struct file *filp);
ssize_t Z_MemDev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos);
ssize_t Z_MemDev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos);

//Helpers
int Z_MemDev_trim(struct Z_MemDev_dev *dev);

//proc
void Z_MemDev_proc_create(void);
void Z_MemDev_proc_remove(void)


/****************/
/* DEBUG REGION */
/****************/




/*
 * Version History:
 *
 * 0.1: initialization version.
 * 1.0: file_operations version.
 * 2.0: proc version.
 */
 
 #endif
