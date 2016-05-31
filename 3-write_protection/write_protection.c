# include <linux/module.h>
# include <linux/kernel.h>


MODULE_LICENSE("GPL");


void
disable_write_protection(void)
{
  unsigned long cr0 = read_cr0();
  clear_bit(X86_CR0_WP_BIT, &cr0);
  write_cr0(cr0);
}


void
enable_write_protection(void)
{
  unsigned long cr0 = read_cr0();
  set_bit(X86_CR0_WP_BIT, &cr0);
  write_cr0(cr0);
}


int
init_module(void)
{
  unsigned long cr0;

  printk(KERN_ALERT "%s\n", "Greetings the World!");

  cr0 = read_cr0();
  printk(KERN_ALERT "Old: %d\n", test_bit(X86_CR0_WP_BIT, &cr0));

  disable_write_protection();
  cr0 = read_cr0();
  printk(KERN_ALERT "New: %d\n", test_bit(X86_CR0_WP_BIT, &cr0));

  enable_write_protection();
  cr0 = read_cr0();
  printk(KERN_ALERT "Now: %d\n", test_bit(X86_CR0_WP_BIT, &cr0));

  return 0;
}


void
cleanup_module(void)
{
  printk(KERN_ALERT "%s\n", "Farewell the World!");

  return;
}
