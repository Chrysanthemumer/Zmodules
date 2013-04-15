/*
 * Z_MemDev_fops.c -- File Operations for MemDev.
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

#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/fcntl.h>

#include "Z_MemDev.h"

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

int Z_MemDev_release(struct inode *inode, struct file *filp)
{
  printk(KERN_INFO "Z_MemDev: fops.release() - Start -\n");
  printk(KERN_INFO "Z_MemDev: fops.release()  - End -\n");
  return 0;
}

ssize_t Z_MemDev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
  printk(KERN_INFO "Z_MemDev: fops.read() - Start -\n");
  printk(KERN_INFO "Z_MemDev: fops.read()  - End -\n");
  return 0;
}

ssize_t Z_MemDev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  printk(KERN_INFO "Z_MemDev: fops.write() - Start -\n");
  printk(KERN_INFO "Z_MemDev: fops.write()  - End -\n");
  return count;
}
 
 
 
 /*
 * Version History:
 *
 * 0.1: initialization version.
 */
