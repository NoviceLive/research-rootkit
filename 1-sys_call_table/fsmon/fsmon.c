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

unsigned long **sct;

#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
asmlinkage long fake_open(const char __user *filename, int flags, umode_t mode);
asmlinkage long fake_openat(int dfd, const char __user *filename, int flags, umode_t mode);
asmlinkage long fake_unlink(const char __user *pathname);
asmlinkage long fake_unlinkat(int dfd, const char __user * pathname, int flag);
asmlinkage long (*real_open)(const char __user *filename, int flags, umode_t mode);
asmlinkage long (*real_openat)(int dfd, const char __user *filename, int flags, umode_t mode);
asmlinkage long (*real_unlink)(const char __user *pathname);
asmlinkage lon (*real_unlinkat)(int dfd, const char __user * pathname, int flag);
#else
asmlinkage long fake_open(struct pt_regs *regs);
asmlinkage long fake_openat(struct pt_regs *regs);
asmlinkage long fake_unlink(struct pt_regs *regs);
asmlinkage long fake_unlinkat(struct pt_regs *regs);
asmlinkage long (*real_open)(struct pt_regs *regs);
asmlinkage long (*real_openat)(struct pt_regs *regs);
asmlinkage long (*real_unlink)(struct pt_regs *regs);
asmlinkage long (*real_unlinkat)(struct pt_regs *regs);
#endif


int
init_module(void)
{
    fm_alert("%s\n", "Greetings the World!");

    /* No consideration on failure. */
    sct = get_sct();

    disable_wp();
    HOOK_SCT(sct, open);
    HOOK_SCT(sct, openat);
    HOOK_SCT(sct, unlink);
    HOOK_SCT(sct, unlinkat);
    enable_wp();

    return 0;
}


void
cleanup_module(void)
{
    disable_wp();
    UNHOOK_SCT(sct, open);
    UNHOOK_SCT(sct, openat);
    UNHOOK_SCT(sct, unlink);
    UNHOOK_SCT(sct, unlinkat);
    enable_wp();

    fm_alert("%s\n", "Farewell the World!");

    return;
}


asmlinkage long
fake_open(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
          const char __user *filename, int flags, umode_t mode
#else
                 struct pt_regs *regs
#endif
          )
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
    MKVAR(const char __user *, filename, regs->di);
    MKVAR(int, flags, regs->si);
    /* MKVAR(umode_t, mode, regs->dx); */
#endif
    if ((flags & O_CREAT) && strcmp(filename, "/dev/null") != 0) {
        fm_alert("open: %s\n", filename);
    }

    return real_open(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
                     filename, flags, mode
#else
                     regs
#endif
                     );
}


asmlinkage long
fake_openat(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
            int dfd, const char __user *filename, int flags, umode_t mode
#else
                 struct pt_regs *regs
#endif
          )
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
    // di
    MKVAR(const char __user *, filename, regs->si);
    MKVAR(int, flags, regs->dx);
#endif
    if ((flags & O_CREAT) && strcmp(filename, "/dev/null") != 0) {
        fm_alert("openat: %s\n", filename);
    }

    return real_openat(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
                     filename, flags, mode
#else
                     regs
#endif
                     );
}


asmlinkage long
fake_unlink(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
            const char __user *pathname
#else
                 struct pt_regs *regs
#endif
            )
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
    MKVAR(const char __user *, pathname, regs->di);
#endif
    fm_alert("unlink: %s\n", pathname);

    return real_unlink(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
                       pathname
#else
                     regs
#endif
                       );
}


asmlinkage long
fake_unlinkat(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
              int dfd, const char __user * pathname, int flag
#else
                 struct pt_regs *regs
#endif
              )
{
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
    /* MKVAR(int, dfd, regs->di); */
    MKVAR(const char __user *, pathname, regs->si);
    /* MKVAR(int, flag, regs->dx); */
#endif
    fm_alert("unlinkat: %s\n", pathname);

    return real_unlinkat(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
                         dfd, pathname, flag
#else
                     regs
#endif
                         );
}
