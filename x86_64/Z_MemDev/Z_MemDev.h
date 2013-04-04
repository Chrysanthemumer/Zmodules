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
#define Z_MEMDEV_SINGLE_NUM 4

typedef struct data_list {
	void **data_unit;
	struct data_list *next;
}data_list;

typedef struct Z_MemDev_dev {
	struct data_list *data;  
	int list_length;              /* the current quantum size */
	int unit_size;                 /* the current array size */
	unsigned long size;       
	//unsigned int access_key;  
	//struct semaphore sem;     
	struct cdev cdev;	        /* Char device structure		*/
}Z_MemDev_dev;
















/*
 * Version History:
 *
 * 0.1: initialization version.
 */
 
 #endif
