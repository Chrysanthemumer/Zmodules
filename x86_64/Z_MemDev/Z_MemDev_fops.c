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
 
static int Z_MemDev_open(struct inode *inode, struct file *filp)
{
  printk(KERN_INFO "Z_MemDev:  open() - Start -\n");
  printk(KERN_INFO "Z_MemDev:  open()  - End -\n");
  return 0;
}

static int Z_MemDev_close(struct inode *inode, struct file *filp)
{
  printk(KERN_INFO "Z_MemDev: release() - Start -\n");
  printk(KERN_INFO "Z_MemDev: release()  - End -\n");
  return 0;
}

static ssize_t Z_MemDev_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
  printk(KERN_INFO "Z_MemDev:  read() - Start -\n");
  printk(KERN_INFO "Z_MemDev:  read()  - End -\n");
  return 0;
}

static ssize_t Z_MemDev_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
  printk(KERN_INFO "Z_MemDev: write() - Start -\n");
  printk(KERN_INFO "Z_MemDev: write()  - End -\n");
  return len;
}
 
 
 
 /*
 * Version History:
 *
 * 0.1: initialization version.
 */
