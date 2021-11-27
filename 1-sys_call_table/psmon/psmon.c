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

unsigned long **sct;

asmlinkage long
fake_execve(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
            const char __user *filename,
            const char __user *const __user *argv,
            const char __user *const __user *envp
#else
            struct pt_regs *regs
#endif
            );
asmlinkage long
(*real_execve)(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
               const char __user *filename,
               const char __user *const __user *argv,
               const char __user *const __user *envp
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
    HOOK_SCT(sct, execve);
    enable_wp();

    return 0;
}


void
cleanup_module(void)
{
    disable_wp();
    UNHOOK_SCT(sct, execve);
    enable_wp();

    fm_alert("%s\n", "Farewell the World!");

    return;
}


asmlinkage long
fake_execve(
#if LINUX_VERSION_CODE < KERNEL_VERSION(4, 17, 0)
            const char __user *filename,
            const char __user *const __user *argv,
            const char __user *const __user *envp
#else
            struct pt_regs *regs
#endif
            )
{
    long ret = 0;
    char *args;
    char *buff;
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
    /* MKVAR(const char __user *, filename, regs->di); */
    MKVAR(const char __user *const __user *, argv, regs->si);
    /* MKVAR(const char __user *const __user *, envp, regs->dx); */
#endif

    buff = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (buff) {
        args = join_strings(argv, " ", buff, PAGE_SIZE);
    } else {
        fm_alert("kmalloc failed!!!");
        args = (char *)argv[0];
        buff = NULL;
    }

    fm_alert("execve: %s\n", args);

    kfree(buff);

    ret = real_execve(
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 17, 0)
                      regs
#else
                      filename, argv, envp
#endif
                      );

    return ret;
}
