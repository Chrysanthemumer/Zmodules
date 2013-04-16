/*
 * Z_MemDev_fops.c -- File Operations for MemDev.
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

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/fcntl.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#include "Z_MemDev.h"

/*****************************************************************************/
/*   Function: Z_MemDev_open                                                 */
/*Description: 'file_operations' open function for MemDev. It helps find     */
/*             Z_MemDev_dev structure every time the device is being opend   */
/*             for file operation.                                           */
/*     return: int                                                           */
/*              0 - success                                                  */
/*             other - errno.h                                               */
/*****************************************************************************/
int Z_MemDev_open(struct inode *inode, struct file *filp)
{
  struct Z_MemDev_dev *dev;
  int err;
  
  printk(KERN_INFO "Z_MemDev: fops.open() - Start -\n");
    
  dev = container_of(inode->i_cdev, struct Z_MemDev_dev, st_cdev);
  filp->private_data = dev;
  
  if((filp->f_flags & O_ACCMODE) == O_WRONLY) {
    err = Z_MemDev_trim(dev);
    if(err != 0) printk(KERN_NOTICE "Z_MemDev: Error getting ready for WRITE_ONLY\n");
  }    
  
  printk(KERN_INFO "Z_MemDev: fops.open()  - End -\n");
  return 0;
}
/*****************************************************************************/
/*   Function: Z_MemDev_release                                              */
/*Description: 'file_operations' release function for MemDev.                */
/*     return: int                                                           */
/*              0 - success                                                  */
/*****************************************************************************/
int Z_MemDev_release(struct inode *inode, struct file *filp)
{
  printk(KERN_INFO "Z_MemDev: fops.release() - Start -\n");
  printk(KERN_INFO "Z_MemDev: fops.release()  - End -\n");
  return 0;
}

/*****************************************************************************/
/*   Function: Z_MemDev_follow                                               */
/*Description: Helps 'read' and 'write' locate the element in the link-list. */
/*     return: struct Z_MemDev_list *                                        */
/*             The element pointer is being returned.                        */
/*****************************************************************************/
struct Z_MemDev_list *Z_MemDev_followlist(struct Z_MemDev_dev *dev, int n)
{
  struct Z_MemDev_list *list = dev->list;
  /* 1. Check and Allocate the first element */
  if(!list) {
    dev->list = kmalloc(sizeof(struct Z_MemDev_list), GFP_KERNEL);
    list = dev->list;
    memset(list, 0, sizeof(struct Z_MemDev_list));
  }
  /* 2. follow the list to the n-th element */
  while(n--) {
    if(!list->next) {
      list->next = kmalloc(sizeof(struct Z_MemDev_list), GFP_KERNEL);
      memset(list, 0, sizeof(struct Z_MemDev_list));
    }
    list = list->next;
    continue;
  }
  
  return list;
}

/*****************************************************************************/
/*   Function: Z_MemDev_read                                                 */
/*Description: Copy less than one MemDev_list.data[] member at f_pos to user */
/*             space.                                                        */
/*     return: size_t being copied.                                          */
/*****************************************************************************/
ssize_t Z_MemDev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
  struct Z_MemDev_dev *dev = filp->private_data;
  struct Z_MemDev_list *element = NULL;
  int elementsize = dev->size * dev->length;
  int quotient, remainder;
  int sub_quotient, sub_remainder;
  
  printk(KERN_INFO "Z_MemDev: fops.read() - Start -\n");
  /* 1. f_pos reaches the end of MemDev */
  if((int)*f_pos >= dev->total_size) {
    printk(KERN_INFO "Z_MemDev: read(): f_ops=[%d]\n", (int)*f_pos);
    printk(KERN_INFO "Z_MemDev: read(): total_size=[%d]\n", dev->total_size);
    printk(KERN_INFO "Z_MemDev: read(): EOF reached, none read.\n");
    return 0;
  }
  /* 2. Update count value according to f_pos and total size */
  if(*f_pos + count > dev->total_size) {
    count = dev->total_size - *f_pos;
  }
  /* 3. Calculate and get the list element */
  quotient  = (long)*f_pos / elementsize;
  remainder = (long)*f_pos % elementsize;
  sub_quotient  = remainder / dev->size;
  sub_remainder = remainder / dev->size;
  element = Z_MemDev_followlist(dev, quotient);
  if(!element || !element->data || !element->data[sub_remainder]) {
    printk(KERN_NOTICE "Z_MemDev: Error in read():followlist()\n");
    return 0;
  }
  /* 4. Read MAXIMUM one array member size of data; */
  /*    Update count the second time.               */
  if(count > dev->size - sub_remainder){
    count = dev->size - sub_remainder;
  }
  if(copy_to_user(buf, element->data[sub_quotient]+sub_remainder, count)) {
    printk(KERN_NOTICE "Z_MemDev: Error in read():copy_to_user()\n");
    return -EFAULT;
  }
  *f_pos += count;
  printk(KERN_INFO "Z_MemDev: fops.read() copied [%d]\n", (unsigned int)count);
  
  printk(KERN_INFO "Z_MemDev: fops.read()  - End -\n");
  return count;
}

/*****************************************************************************/
/*   Function: Z_MemDev_write                                                */
/*Description: Copy less than one MemDev_list.data[] member at f_pos from    */
/*             user space.                                                   */
/*     return: size_t being copied.                                          */
/*****************************************************************************/
ssize_t Z_MemDev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  struct Z_MemDev_dev *dev = filp->private_data;
  struct Z_MemDev_list *element = NULL;
  int elementsize = dev->size * dev->length;
  int quotient, remainder;
  int sub_quotient, sub_remainder;

  printk(KERN_INFO "Z_MemDev: fops.write() - Start -\n");
  printk(KERN_INFO "Z_MemDev: write(): f_ops=[%d]\n", (int)*f_pos);
  /* 1. Calculate and get the list element */
  quotient  = (long)*f_pos / elementsize;
  remainder = (long)*f_pos % elementsize;
  sub_quotient  = remainder / dev->size;
  sub_remainder = remainder / dev->size;
  element = Z_MemDev_followlist(dev, quotient);
  if(!element) {
    printk(KERN_NOTICE "Z_MemDev: Error in write():followlist()\n");
    return 0;
  }
  /* 2. Initialize memory if not initiailzed yet */
  if(!element->data) {
    element->data = kmalloc(dev->length * sizeof(char *), GFP_KERNEL);
    if(!element->data) {
      printk(KERN_NOTICE "Z_MemDev: Error in write():fail to allocate list element memory\n");
      return 0;
    }
    memset(element->data, 0, dev->length * sizeof(char *));
  }
  if(!element->data[sub_quotient]) {
    element->data[sub_quotient] = kmalloc(dev->size, GFP_KERNEL);
    if(!element->data[sub_quotient]) {
      printk(KERN_NOTICE "Z_MemDev: Error in write():fail to allocate element member memory\n");
      return 0;
    }
  }
  /* 3. Copy up-to the end of this memory from user */
  if(count > dev->size - sub_remainder){
    count = dev->size - sub_remainder;
  }
  if(copy_from_user(element->data[sub_quotient]+sub_remainder, buf, count)) {
    printk(KERN_NOTICE "Z_MemDev: Error in read():copy_to_user()\n");
    return -EFAULT;
  }
  *f_pos += count;
  printk(KERN_INFO "Z_MemDev: fops.write() copied [%d]\n", (unsigned int)count);
  /* 4. Update total_size */
  if(dev->total_size < *f_pos) {
    dev->total_size = *f_pos;
  }
  
  
  printk(KERN_INFO "Z_MemDev: fops.write()  - End -\n");
  return count;
}
 
 
 
 /*
 * Version History:
 *
 * 0.1: initialization version.
 * 1.0: file_operations version.
 */
