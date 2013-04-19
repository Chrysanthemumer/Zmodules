/*
 * Z_MemDev_pfops.c -- procfs utilities for MemDev
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

/* 
 * Dependency:
 * Kernel Header Version 3.8
 * 1. proc_create_data();
 */


#include <linux/proc_fs.h>
#include <linux/seq_file.h>
 
#include "Z_MemDev.h"
#ifdef MEMDEV_PROC
/*****************************************************************************/
/*   Function: Z_MemDev_proc_seq_start                                       */
/*Description: Implementation of seq_operation->start.                       */
/*     return: void * - a pointer pass to next                               */
/*             NULL   - end of loop                                          */
/*****************************************************************************/
static void *Z_MemDev_proc_seq_start(struct seq_file *s, loff_t *pos)
{
  if((int)*pos >= MemDev_num)
    return NULL;
  else
    return MemDev_devices + (int)*pos;
}/* END */

/*****************************************************************************/
/*   Function: Z_MemDev_proc_seq_next                                        */
/*Description: Implementation of seq_operation->next .                       */
/*     return: void * - a pointer pass to next                               */
/*             NULL   - end of loop                                          */
/*****************************************************************************/
static void *Z_MemDev_proc_seq_next(struct seq_file *s, void *v, loff_t *pos)
{
  (*pos)++;
  if((int)*pos >= MemDev_num) 
    return NULL;
  else
    return MemDev_devices + (int)*pos;
}/* END */

/*****************************************************************************/
/*   Function: Z_MemDev_proc_seq_start                                       */
/*Description: Implementation of seq_operation->start.                       */
/*     return: void * - a pointer pass to next                               */
/*             NULL   - end of loop                                          */
/*****************************************************************************/
static void Z_MemDev_proc_seq_stop(struct seq_file *s, void *v)
{
}/* END */

/*****************************************************************************/
/*   Function: Z_MemDev_proc_seq_show                                        */
/*Description: Implementation of seq_operation->show.                        */
/*     return: int - 0 success                                               */
/*****************************************************************************/
static int Z_MemDev_proc_seq_show(struct seq_file *s, void *v)
{
  struct Z_MemDev_dev *dev = (struct Z_MemDev_dev *)v;
  struct Z_MemDev_list *element = NULL;
  int j;

  seq_printf(s, "\nDevice %i: size %i, length %i, total %li\n",
               (int)(dev-MemDev_devices), dev->size, dev->length, dev->total_size);
  /* <1> loop in data list of one device */
  for (element = dev->list; element; element = element->next) {
    seq_printf(s, "  element at %p, data at %p\n",
                         element, element->data);
    /* <2> Just get the last element in one device */
    if(element && !element->next) {
      for(j = 0; j <= dev->length; j++) {
        if(element->data[j]) {
          seq_printf(s, "    % 4i: %8p\n", j, element->data[j]);
        } else {
          break;
        }
      } /* end <2> */
    }
  } /* end <1> */
  
  return 0;  
}/* END */

static struct seq_operations MemDev_seq_fops = {
	.start = Z_MemDev_proc_seq_start,
	.next  = Z_MemDev_proc_seq_next,
	.stop  = Z_MemDev_proc_seq_stop,
	.show  = Z_MemDev_proc_seq_show
};

/*****************************************************************************/
/*   Function: Z_MemDev_proc_open                                            */
/*Description: This function connects '_memory' file operation open with     */
/*             seq_operations.                                               */
/*             It is the implementation for the file operations open of      */
/*             '_memory' entry.                                              */
/*     return: int - 0 success                                               */
/*****************************************************************************/
int Z_MemDev_proc_open(struct inode *inode, struct file *filp)
{
  return seq_open(filp, &MemDev_seq_fops);
}/* END */


/*****************************************************************************/
/*   Function: Z_MemDev_proc_read_proc                                       */
/*Description: This function is the implementation for the read_proc function*/
/*             of '_memory' entry.                                           */
/*     return: void                                                          */
/*****************************************************************************/
static int Z_MemDev_proc_read_proc(char *page, char **start, off_t off,
                            int count, int *eof, void *data)
{
  int limit = count - 80;
  int data_read = 0;
  int i, j;
  struct Z_MemDev_list *element;
  
  /* <1> loop in number of devices */
  for (i = 0; i < MemDev_num; i++) {
    data_read += sprintf(page+data_read, "\nDevice %i: size %i, length %i, total %li\n",
                        i, MemDev_devices[i].size, MemDev_devices[i].length,
                        MemDev_devices[i].total_size);
    /* <2> loop in data list of one device */
    for (element = MemDev_devices[i].list; element && data_read <= limit;
         element = element->next) {
      data_read += sprintf(page+data_read, "  element at %p, data at %p\n",
                           element, element->data);
      /* <3> Just get the last element in one device */
      if(element && !element->next) {
        for(j = 0; j <= MemDev_devices[i].length; j++) {
          if(element->data[j]) {
            data_read += sprintf(page+data_read, "    % 4i: %8p\n", j, element->data[j]);
          } else {
            break;
          }
        }
      } /* end <3> */
    } /* end <2> */
  } /* end <1> */
  
  *eof = 1;
  return data_read;
}

/*****************************************************************************/
/*   Function: Z_MemDev_proc_create                                          */
/*Description: Create proc file system directory or file for MemDev. This    */
/*             function is called at module init time. two directory is to be*/
/*             created, the '_memory' space which turn out to be a read only */
/*             space, and the '_seq' which uses seq_file to pull infomation  */
/*             from module.                                                  */
/*     return: void                                                          */
/*****************************************************************************/
void Z_MemDev_proc_create(void)
{
  /* 1. Create proc entry for '_memory' */
  create_proc_read_entry("Z_MemDev_memory", 0, NULL, Z_MemDev_proc_read_proc,
                         NULL);
  /* 2. Create proc entry for '_seq' with proc_fops */
  proc_create_data("Z_MemDev_seq", 0, NULL, &MemDev_pfops, NULL);
  
  
}/* END */

/*****************************************************************************/
/*   Function: Z_MemDev_proc_remove                                          */
/*Description: Remove proc file system directory or file for MemDev. This    */
/*             function is called at module cleanup time.                    */
/*     return: void                                                          */
/*****************************************************************************/
void Z_MemDev_proc_remove(void)
{
  remove_proc_entry("Z_MemDev_memory", NULL);
  remove_proc_entry("Z_MemDev_seq", NULL);
}/* END */
 
 
#endif /* MEMDEV_PROC */
 
  /*
 * Version History:
 *
 * 0.1: initialization version. (Apr 04, 2013)
 * 1.0: proc version with switch (MEMDEV_PROC).
 */
