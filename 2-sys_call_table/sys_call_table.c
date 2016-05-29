# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/syscalls.h>


MODULE_LICENSE("GPL");

asmlinkage unsigned long long **sys_call_table;


unsigned long long **
get_sys_call_table(void)
{
  unsigned long long **entry = (unsigned long long **)PAGE_OFFSET;

  /* Conquer or die. */
  for (;; entry += 1) {
    if (entry[__NR_close] == (unsigned long long *)sys_close) {
        return entry;
      }
  }

  /* Won't happen. */
  return NULL;
}


int
init_module(void)
{
  printk(KERN_ALERT "%s\n", "Greetings the World!");

  sys_call_table = get_sys_call_table();

  printk(KERN_ALERT "PAGE_OFFSET = %lx\n", PAGE_OFFSET);
  printk(KERN_ALERT "sys_call_table = %p\n", sys_call_table);
  printk(KERN_ALERT "sys_call_table - PAGE_OFFSET = %llu MiB\n",
         ((unsigned long long)sys_call_table -
          (unsigned long long)PAGE_OFFSET) / 1024 / 1024);

  return 0;
}


void
cleanup_module(void)
{
  printk(KERN_ALERT "%s\n", "Farewell the World!");

  return;
}
