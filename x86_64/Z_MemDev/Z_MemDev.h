/*
 * Z_MemDev.h -- header for Memory Devices development modules
 *
 * version 0.1
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
typedef struct Z_MemDev_list {
	void **data;
	struct Z_MemDev_list *next;
}Z_MemDev_list;

typedef struct Z_MemDev_dev {
	struct Z_MemDev_list *list;  
	int size;                   /* the current quantum size */
	int length;                 /* the current array size */
	unsigned long total_size;       
	//unsigned int access_key;  
	//struct semaphore sem;     
	struct cdev cdev;	          /* Char device structure		*/
}Z_MemDev_dev;

extern int MemDev_list_size;
extern int MemDev_list_length;














/*
 * Version History:
 *
 * 0.1: initialization version.
 */
 
 #endif
