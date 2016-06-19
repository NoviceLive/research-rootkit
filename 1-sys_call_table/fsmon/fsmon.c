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

# include "lib/lib.h"


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


int
init_module(void)
{
  printk(KERN_ALERT "%s\n", "Greetings the World!");

  /* No consideration on failure. */
  sys_call_table = get_sys_call_table();

  disable_write_protection();
  HOOK_SYS_CALL_TABLE(open);
  HOOK_SYS_CALL_TABLE(unlink);
  HOOK_SYS_CALL_TABLE(unlinkat);
  enable_write_protection();

  return 0;
}


void
cleanup_module(void)
{
  disable_write_protection();
  UNHOOK_SYS_CALL_TABLE(open);
  UNHOOK_SYS_CALL_TABLE(unlink);
  UNHOOK_SYS_CALL_TABLE(unlinkat);
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
