# include <linux/module.h>
# include <linux/kernel.h>


MODULE_LICENSE("GPL");


int
init_module(void)
{
  printk(KERN_ALERT "%s\n", "Greetings the World!");

  return 0;
}


void
cleanup_module(void)
{
  printk(KERN_ALERT "%s\n", "Farewell the World!");

  return;
}
