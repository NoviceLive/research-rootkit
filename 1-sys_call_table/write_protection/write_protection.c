/*
Copyright 2016 Gu Zhengxiong <rectigu@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/


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
