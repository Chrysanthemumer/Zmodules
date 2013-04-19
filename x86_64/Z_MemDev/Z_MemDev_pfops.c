/*
 * Z_MemDev_pfops.c -- procfs utilities for MemDev
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

/* 
 * Dependency:
 * Kernel Header Version 3.8
 * 1. proc_create_data();
 */
 
#include <linux/proc_fs.h>
 
#include "Z_MemDev.h"



struct file_operations MemDev_pfops = {
  .owner   = THIS_MODULE,
}

/*****************************************************************************/
/*   Function: Z_MemDev_proc_read_proc                                       */
/*Description: This function is the implementation for the read_proc function*/
/*             of '_memory' entry.                                           */
/*     return: void                                                          */
/*****************************************************************************/
int Z_MemDev_proc_read_proc(char *page, char **start, off_t off,
                            int count, int *eof, void *data)
{
  int limit = count - 80;
  int data_read = 0;
  int i, j;
  struct Z_MemDev_list *element;
  
  /* <1> loop in number of devices */
  for (i = 0; i < MemDev_num; i++) {
    data_read += sprintf(page+data_read, "\nDevice %i: size %i, length %i, total %li\n",
                        i, MemDev_devices[i]->size, MemDev_devices[i]->length,
                        MemDev_devices[i]->total_size);
    /* <2> loop in data list of one device */
    for (element = MemDev_devices[i]->list; element && data_read <= limit;
         element = element->next) {
      data_read += sprintf(page+data_read, "  element at %p, data at %p\n",
                           element, element->data);
      /* <3> Just get the last element in one device */
      if(element && !element->next) {
        for(j = 0; j <= element->length; j++) {
          if(element->data[j] {
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
  struct proc_dir_entry *entry;
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
 
 
 
 
  /*
 * Version History:
 *
 * 0.1: initialization version. (Apr 04, 2013)
 */
