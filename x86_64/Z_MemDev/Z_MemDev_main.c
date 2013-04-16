/*
 * Z_MemDev.c -- Memory Devices develpment module
                        with least functions
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

#include <linux/init.h>
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>

#include <asm-generic/errno.h>

#include "Z_MemDev.h"

int MemDev_major = Z_MEMDEV_MAJOR;
int MemDev_minor = Z_MEMDEV_MINOR;
int MemDev_num = Z_MEMDEV_NUM;

int MemDev_list_size = MEMDEV_LIST_SIZE;
int MemDev_list_length = MEMDEV_LIST_LENGTH;

struct Z_MemDev_dev * MemDev_devices = NULL;

MODULE_AUTHOR("Sizhou Wang");
MODULE_LICENSE("Dual BSD/GPL");

struct file_operations MemDev_fops = {
  .owner   = THIS_MODULE,
  //.llseek  = ,
  .read    = Z_MemDev_read,
  .write   = Z_MemDev_write,
  .open    = Z_MemDev_open,
  .release = Z_MemDev_release,
};
/*****************************************************************************/
/*   Function: Z_MemDev_trim                                                 */
/*Description: Empty out a MemDev device. (Destory and init Z_MemDev_list    */
/*             related information within one single Z_MemDev_dev structure.)*/
/*     return: int                                                           */
/*              0 - success                                                  */
/*             -1 - Fail, input is null                                      */
/*****************************************************************************/
int Z_MemDev_trim(struct Z_MemDev_dev *dev)
{
  struct Z_MemDev_list *list_current;
  struct Z_MemDev_list *list_next;
  int i;
  if(dev == NULL){
    return -1;
  }
  for(list_current = dev->list; list_current; list_current = list_next){
    if(list_current->data){
      for(i = 0; i < dev->length; i++){
        kfree(list_current->data[i]); //2 dimensional array
      }
      kfree(list_current->data);
    }
    list_next = list_current->next;
    kfree(list_current);
  }
  dev->total_size = 0;
  dev->length = MemDev_list_length;
  dev->size = MemDev_list_size;
  dev->list = NULL;
  
  return 0;
}

/*****************************************************************************/
/*   Function: Z_MemDev_cleanup_module                                       */
/*Description: Module Cleanup function. The function can be called during    */
/*             initialization failure.                                       */
/*     return: void                                                          */
/*****************************************************************************/
static void __exit Z_MemDev_cleanup_module(void)
{
  int i;
  int devno = MKDEV(MemDev_major, MemDev_minor);
  
  printk(KERN_INFO "Z_MemDev: Cleanup() - Start -\n");
  
  /* 1. Release the Each Device Structure Memory */
  if(MemDev_devices){
    for(i = 0; i < MemDev_num; i++){
      Z_MemDev_trim(MemDev_devices + i);
      cdev_del(&MemDev_devices[i].st_cdev);
    }
    kfree(MemDev_devices);
  }
  /* 2. Unregister Devices */
	unregister_chrdev_region(devno, MemDev_num);
	
	printk(KERN_INFO "Z_MemDev: Cleanup()  - End -\n");
}

/*****************************************************************************/
/*   Function: Z_MemDev_setup_cdev                                           */
/*Description: Setup one char device during "init_module"                    */
/*             All resources limited to this function shall be destoryed     */
/*             after initialization.                                         */
/*     return: void                                                          */
/*****************************************************************************/
static void __init Z_MemDev_setup_cdev(struct Z_MemDev_dev *dev, int index)
{
  int err;
  int devno = MKDEV(MemDev_major, MemDev_minor + index);
  printk(KERN_INFO "Z_MemDev: setup() - Running -\n");
  cdev_init(&dev->st_cdev, &MemDev_fops);
  dev->st_cdev.owner = THIS_MODULE;
  err = cdev_add(&dev->st_cdev, devno, 1);
  if(err < 0){
    printk(KERN_NOTICE "Z_MemDev: Error %d adding device [%d]\n", err, devno);
  }
}
/*****************************************************************************/
/*   Function: Z_MemDev_init_module                                          */
/*Description: Module Initialization function.                               */
/*             All resources limited to this function shall be destoryed     */
/*             after initialization.                                         */
/*     return: void                                                          */
/*****************************************************************************/
static int __init Z_MemDev_init_module(void)
{
  int err, i;
  dev_t dev = 0;
  
  printk(KERN_INFO "Z_MemDev: init() - Start -\n");
  
  /* 1. Register Devices */
  err = alloc_chrdev_region(&dev, 0, MemDev_num, "Z_MemDev");
  MemDev_major = MAJOR(dev);
  if(err < 0){
    printk(KERN_WARNING "Z_MemDev: Cannot register major! [%d]\n", MemDev_major);
    return err;
  }
  /* 2. Allocate the Device Structure */
  MemDev_devices = kmalloc(MemDev_num * sizeof(struct Z_MemDev_dev), GFP_KERNEL);
  if(MemDev_devices == NULL){
    printk(KERN_WARNING "Z_MemDev: Cannot kmalloc during init_module\n");
    err = -ENOMEM;
    //Z_MemDev_cleanup_module();
    return err;  
  }
  memset(MemDev_devices, 0, MemDev_num * sizeof(struct Z_MemDev_dev));
  /* 3. Initialize Each Device (MemDev_num) */
  for(i = 0; i < MemDev_num; i++){
    MemDev_devices[i].length = MemDev_list_length;
    MemDev_devices[i].size   = MemDev_list_size;
    MemDev_devices[i].list   = NULL;
    MemDev_devices[i].total_size   = 0;
    //Mutex here
    Z_MemDev_setup_cdev(&MemDev_devices[i], i);
    
  }
  
  printk(KERN_INFO "Z_MemDev: init()  - End -\n");
  return 0; 
}

 
module_init(Z_MemDev_init_module);
module_exit(Z_MemDev_cleanup_module);

/*
 * Version History:
 *
 * 0.1: initialization version.
 * 1.0: file_operations version.
 */
