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

# include "lib.h"


MODULE_LICENSE("GPL");

asmlinkage unsigned long **real_sys_call_table;


int
init_module(void)
{
    printk(KERN_ALERT "%s\n", "Greetings the World!");

    real_sys_call_table = get_sys_call_table();

    printk(KERN_ALERT "PAGE_OFFSET = %lx\n", PAGE_OFFSET);
    printk(KERN_ALERT "sys_call_table = %p\n", real_sys_call_table);
    printk(KERN_ALERT "sys_call_table - PAGE_OFFSET = %lu MiB\n",
           ((unsigned long)real_sys_call_table -
            (unsigned long)PAGE_OFFSET) / 1024 / 1024);

    return 0;
}


void
cleanup_module(void)
{
    printk(KERN_ALERT "%s\n", "Farewell the World!");

    return;
}
