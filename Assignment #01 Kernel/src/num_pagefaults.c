/*********************************************************************************
NAME : RAHUL SHINDE
ASSIGNMENT NO : 01
ASSIGNMENT : WRITING A KERNEL MODULE
REFERENCES : 
https://courses.cs.washington.edu/courses/cse551/07sp/programming/assign2/gribble_module.c
http://www.crashcourse.ca/introduction-linux-kernel-programming/lesson-11-adding-proc-files-your-modules-part-1
http://www.crashcourse.ca/introduction-linux-kernel-programming/lesson-4-writing-and-running-your-first-kernel-module
*********************************************************************************/

#include <linux/module.h>    // Used for all kernel modules
#include <linux/kernel.h>    // Used for KERN_INFO
#include <linux/init.h>      // Used for __init and __exit macros
#include <linux/proc_fs.h>   // Used for writing to proc
#include <linux/seq_file.h>  // Used for using sequence files
#include <linux/mm.h>        // Used for vm stats


static unsigned long stats_buffer[NR_VM_EVENT_ITEMS];	// It will store information passed to all_vm_events
  
static int show_pagefault(struct seq_file *m, void *v)	// It is used to print total pagefaults on the console
{
    all_vm_events(stats_buffer);
    seq_printf(m, "******Kernel Module Assignment****** \nNumber of pagefaults :\t");	 
    seq_printf(m, "%lu\n", stats_buffer[PGFAULT]);
    return 0;
}

static int open_pagefault(struct inode *inode, struct file *file)	//This function called when proc file is opened	
{
    return single_open(file,show_pagefault, NULL);
} 

static const struct file_operations pf_fops = 
{
    .open     = open_pagefault, 
    .read     = seq_read,        
    .release  = single_release,  
};

static int __init init_pagefault(void)	// This function is called when module is installed
{
    proc_create("num_pagefaults",0,NULL,&pf_fops);   
    return 0;
}

static void __exit exit_pagefault(void)	// This function is called when module is removed
{
    remove_proc_entry("num_pagefaults",NULL);
    printk(KERN_INFO"\nmodule removed");
}

module_init(init_pagefault);
module_exit(exit_pagefault);

MODULE_LICENSE("GPL");
