# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/syscalls.h>


MODULE_LICENSE("GPL");

asmlinkage unsigned long **sys_call_table;


unsigned long **
get_sys_call_table(void)
{
  unsigned long **entry = (unsigned long **)PAGE_OFFSET;

  for (;(unsigned long)entry < ULONG_MAX; entry += 1) {
    if (entry[__NR_close] == (unsigned long *)sys_close) {
        return entry;
      }
  }

  return NULL;
}


int
init_module(void)
{
  printk(KERN_ALERT "%s\n", "Greetings the World!");

  sys_call_table = get_sys_call_table();

  printk(KERN_ALERT "PAGE_OFFSET = %lx\n", PAGE_OFFSET);
  printk(KERN_ALERT "sys_call_table = %p\n", sys_call_table);
  printk(KERN_ALERT "sys_call_table - PAGE_OFFSET = %lu MiB\n",
         ((unsigned long)sys_call_table -
          (unsigned long)PAGE_OFFSET) / 1024 / 1024);

  return 0;
}


void
cleanup_module(void)
{
  printk(KERN_ALERT "%s\n", "Farewell the World!");

  return;
}
