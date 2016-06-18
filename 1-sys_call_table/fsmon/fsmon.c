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
# include <linux/syscalls.h>


MODULE_LICENSE("GPL");

asmlinkage unsigned long **sys_call_table;

asmlinkage long
fake_open(const char __user *filename, int flags, umode_t mode);
asmlinkage long
fake_unlink(const char __user *pathname);
asmlinkage long
fake_unlinkat(int dfd, const char __user * pathname, int flag);
asmlinkage long
(*real_open)(const char __user *filename, int flags, umode_t mode);
asmlinkage long
(*real_unlink)(const char __user *pathname);
asmlinkage long
(*real_unlinkat)(int dfd, const char __user * pathname, int flag);


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
  printk(KERN_ALERT "%s\n", "Greetings the World!");

  /* No consideration on failure. */
  sys_call_table = get_sys_call_table();

  disable_write_protection();
  real_open = (void *)sys_call_table[__NR_open];
  sys_call_table[__NR_open] = (unsigned long*)fake_open;
  real_unlink = (void *)sys_call_table[__NR_unlink];
  sys_call_table[__NR_unlink] = (unsigned long*)fake_unlink;
  real_unlinkat = (void *)sys_call_table[__NR_unlinkat];
  sys_call_table[__NR_unlinkat] = (unsigned long*)fake_unlinkat;
  enable_write_protection();

  return 0;
}


void
cleanup_module(void)
{
  disable_write_protection();
  sys_call_table[__NR_open] = (unsigned long*)real_open;
  sys_call_table[__NR_unlink] = (unsigned long*)real_unlink;
  sys_call_table[__NR_unlinkat] = (unsigned long*)real_unlinkat;
  enable_write_protection();

  printk(KERN_ALERT "%s\n", "Farewell the World!");

  return;
}


asmlinkage long
fake_open(const char __user *filename, int flags, umode_t mode)
{
  if ((flags & O_CREAT) && strcmp(filename, "/dev/null") != 0) {
    printk(KERN_ALERT "open: %s\n", filename);
  }

  return real_open(filename, flags, mode);
}


asmlinkage long
fake_unlink(const char __user *pathname)
{
  printk(KERN_ALERT "unlink: %s\n", pathname);

  return real_unlink(pathname);
}


asmlinkage long
fake_unlinkat(int dfd, const char __user * pathname, int flag)
{
  printk(KERN_ALERT "unlinkat: %s\n", pathname);

  return real_unlinkat(dfd, pathname, flag);
}
