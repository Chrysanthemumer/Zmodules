/*
 * Z_MemDev_single.c -- Memory Devices develpment module
                        with least functions
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

#include <linux/init.h>
#include <linux/modules.h>

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/string.h>

#include <asm-generic/errno.h>

int MemDev_major = Z_MEMDEV_MAJOR;
int MemDev_minor = Z_MEMDEV_MINOR;
int MemDev_single_num = Z_MEMDEV_SINGLE_NUM;

struct Z_MemDev_dev * MemDev_devices = NULL;
/*****************************************************************************/
/*   Function: Z_MemDev_init_module                                          */
/*Description: Module Initialization function. All resources limited to this */
/*             function shall be destoryed after initialization.             */
/*     return:                                                               */
/*****************************************************************************/
static int __init Z_MemDev_init_module(void)
{
  int err;
  dev_t dev = 0;
  
  /* 1. Register Device MAJOR */
  err = alloc_chrdev_region(&dev, 0, MemDev_single_num, "Z_MemDev");
  MemDev_major = MAJOR(dev);
  if(err == 0){
    printk(KERN_WARNING "Z_MemDev: Cannot register major! [%d]\n", MemDev_major);
    return err;
  }
  /* 2. Allocate the Device Structure */
  MemDev_devices = Kmalloc(MemDev_single_num * sizeof(struct Z_MemDev_dev), GFP_KERNEL);
  if(MemDev_devices == NULL){
    err = -ENOMEM;
    goto fail;
  }
  memset(MemDev_devices, 0, MemDev_single_num * sizeof(struct Z_MemDev_dev));
  
  
}
/*****************************************************************************/
/*                                                                           */
/*****************************************************************************/
static void __exit Z_MemDev_cleanup_module(void)
{

}
 
module_init(Z_MemDev_init_module);
module_exit(Z_MemDev_cleanup_module);

/*
 * Version History:
 *
 * 0.1: initialization version.
 */
