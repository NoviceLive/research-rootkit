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
