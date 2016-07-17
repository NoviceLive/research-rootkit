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
# include <linux/slab.h> // kmalloc, kfree.
# include <linux/syscalls.h>

# include "lib/lib.h"


MODULE_LICENSE("GPL");

asmlinkage unsigned long **real_sys_call_table;

asmlinkage long
fake_execve(const char __user *filename,
            const char __user *const __user *argv,
            const char __user *const __user *envp);
asmlinkage long
(*real_execve)(const char __user *filename,
               const char __user *const __user *argv,
               const char __user *const __user *envp);


int
init_module(void)
{
  pr_alert("%s\n", "Greetings the World!");

  /* No consideration on failure. */
  real_sys_call_table = get_sys_call_table();

  disable_write_protection();
  HOOK_SYS_CALL_TABLE(execve);
  enable_write_protection();

  return 0;
}


void
cleanup_module(void)
{
  disable_write_protection();
  UNHOOK_SYS_CALL_TABLE(execve);
  enable_write_protection();

  pr_alert("%s\n", "Farewell the World!");

  return;
}


asmlinkage long
fake_execve(const char __user *filename,
            const char __user *const __user *argv,
            const char __user *const __user *envp)
{
  char *args;
  char *buff = kmalloc(PAGE_SIZE, GFP_KERNEL);
  if (buff) {
    args = join_strings(argv, " ", buff, PAGE_SIZE);
  } else {
    args = (char *)argv[0];
    buff = NULL;
  }

  pr_alert("execve: %s\n", args);

  kfree(buff);

  return real_execve(filename, argv, envp);
}
