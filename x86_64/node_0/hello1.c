/*
 * hello1.c init_module and cleanup_module
 */
 
#include <linux/module.h>
#include <linux/init.h>

static hello_init(void)
{
  printk(KERN_ALERT "Hello kernel 1.\n");
  return 0;
}

static void hello_exit(void)
{
  printk(KERN_ALERT "Goodbye kernel 1.\n");
}

module_init(hello_init);
module_exit(hello_exit);
