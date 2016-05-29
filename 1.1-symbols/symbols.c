# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/syscalls.h>


MODULE_LICENSE("GPL");


int
init_module(void)
{
  printk(KERN_ALERT "%s\n", "Greetings the World!");

  printk(KERN_ALERT "sys_close = %p\n", sys_close);
  printk(KERN_ALERT "sys_open = %p\n", sys_open);
  printk(KERN_ALERT "sys_read = %p\n", sys_read);
  printk(KERN_ALERT "sys_write = %p\n", sys_write);

  return 0;
}


void
cleanup_module(void)
{
  printk(KERN_ALERT "%s\n", "Farewell the World!");

  return;
}
