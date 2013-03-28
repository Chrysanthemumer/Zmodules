/*
 * hello1.c init_module and cleanup_module
 */
 
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>

#include <linux/sched.h>
//MODULE_AUTHOR("Sizhou.Wang");

static int hello_init(void)
{
  static char *whom = "world";
  //module_param(whom, charp, S_IRUGO);  
  printk(KERN_ALERT "Hello kernel 1.\n");
  //Print the current running process pid
  printk(KERN_INFO "The process is \"%s\" (pid %i)\n", current->comm, current->pid);
  printk(KERN_INFO "Whom is \"%s\" .", whom);
  return 0;
}

static void hello_exit(void)
{
  printk(KERN_ALERT "Goodbye kernel 1.\n");
}

module_init(hello_init);
module_exit(hello_exit);

