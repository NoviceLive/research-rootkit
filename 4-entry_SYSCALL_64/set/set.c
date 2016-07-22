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


# ifndef CPP
# include <linux/module.h>
# include <linux/kernel.h>
// proc_create, proc_remove.
# include <linux/proc_fs.h>
// __NR_syscall_max.
# include <asm-generic/asm-offsets.h>
# endif // CPP

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");

# define NAME "entry"

struct proc_dir_entry *entry;

ssize_t
write_handler(struct file * filp, const char __user *buff,
              size_t count, loff_t *offp);

struct file_operations proc_fops = {
    .write = write_handler
};

// kernel text mapping, from phys 0
// ffffffff80000000 - ffffffffa0000000 (=512 MB)

// module mapping space
// ffffffffa0000000 - ffffffffff5fffff (=1526 MB)

u64 fake_sct[__NR_syscall_max] = { 0 };


int
init_module(void)
{
    fm_alert("%s\n", "Greetings the World!");

    fm_alert("__NR_syscall_max: %d", __NR_syscall_max);

    entry = proc_create(NAME, S_IRUGO | S_IWUGO, NULL, &proc_fops);

    return 0;
}


void
cleanup_module(void)
{
    proc_remove(entry);

    fm_alert("%s\n", "Farewell the World!");

    return;
}


// INFO: This is for debugging using gdb via kgdb,
// and with this we can break upon interesting code conveniently.
ssize_t
write_handler(struct file * filp, const char __user *buff,
              size_t count, loff_t *offp)
{
    u64 *real_sct;

    real_sct = (u64 *)get_sct_via_sys_close();
    if (real_sct == NULL) {
        fm_alert("%s\n", "get_sct failed.");
        return count;
    }
    fm_alert("real sys_call_table: %p\n", real_sct);
    fm_alert("fake sys_call_table: %p\n", fake_sct);

    memcpy(fake_sct, real_sct, sizeof fake_sct);

    set_lstar_sct((u32)(unsigned long)fake_sct);

    return count;
}


// 5f:       ff 14 c5 a0 01 60 81
// call   QWORD PTR [rax*8-0x7e9ffe60]
// 816001a0
