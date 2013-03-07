/*
 * hello1.c init_module and cleanup_module
 */
 
#include <linux/module.h>
#include <linux/kernel.h>

static __init init_module(void)
{
  printk(KERN_INFO "Hello kernel 1.\n");
  return 0;
}

static void __exit cleanup_module(void)
{
  printk(KERN_INFO "Goodbye kernel 1.\n");
}

