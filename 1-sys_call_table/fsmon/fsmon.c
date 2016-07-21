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
# include <linux/syscalls.h>

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");

asmlinkage unsigned long **real_sys_call_table;

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
  fm_alert("%s\n", "Greetings the World!");

  /* No consideration on failure. */
  real_sys_call_table = get_sct();

  disable_wp();
  HOOK_SYS_CALL_TABLE(open);
  HOOK_SYS_CALL_TABLE(unlink);
  HOOK_SYS_CALL_TABLE(unlinkat);
  enable_wp();

  return 0;
}


void
cleanup_module(void)
{
  disable_wp();
  UNHOOK_SYS_CALL_TABLE(open);
  UNHOOK_SYS_CALL_TABLE(unlink);
  UNHOOK_SYS_CALL_TABLE(unlinkat);
  enable_wp();

  fm_alert("%s\n", "Farewell the World!");

  return;
}


asmlinkage long
fake_open(const char __user *filename, int flags, umode_t mode)
{
  if ((flags & O_CREAT) && strcmp(filename, "/dev/null") != 0) {
    fm_alert("open: %s\n", filename);
  }

  return real_open(filename, flags, mode);
}


asmlinkage long
fake_unlink(const char __user *pathname)
{
  fm_alert("unlink: %s\n", pathname);

  return real_unlink(pathname);
}


asmlinkage long
fake_unlinkat(int dfd, const char __user * pathname, int flag)
{
  fm_alert("unlinkat: %s\n", pathname);

  return real_unlinkat(dfd, pathname, flag);
}
