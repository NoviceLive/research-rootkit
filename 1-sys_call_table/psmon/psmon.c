// Copyright 2016 Gu Zhengxiong <rectigu@gmail.com>
//
// This file is part of LibZeroEvil.
//
// LibZeroEvil is free software:
// you can redistribute it and/or modify it
// under the terms of the GNU General Public License
// as published by the Free Software Foundation,
// either version 3 of the License,
// or (at your option) any later version.
//
// LibZeroEvil is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with LibZeroEvil.
// If not, see <http://www.gnu.org/licenses/>.


# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/slab.h> // kmalloc, kfree.
# include <linux/syscalls.h>

# include "zeroevil/zeroevil.h"


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
  fm_alert("%s\n", "Greetings the World!");

  /* No consideration on failure. */
  real_sys_call_table = get_sct();

  disable_wp();
  HOOK_SYS_CALL_TABLE(execve);
  enable_wp();

  return 0;
}


void
cleanup_module(void)
{
  disable_wp();
  UNHOOK_SYS_CALL_TABLE(execve);
  enable_wp();

  fm_alert("%s\n", "Farewell the World!");

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

  fm_alert("execve: %s\n", args);

  kfree(buff);

  return real_execve(filename, argv, envp);
}
