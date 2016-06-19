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


/* unsigned long ** */
/* get_sys_call_table() */
/* { */
/* } */


unsigned long **
get_sys_call_table(void)
{
  unsigned long **entry = (unsigned long **)PAGE_OFFSET;

  for (;(unsigned long)entry < ULONG_MAX; entry += 1) {
    if (entry[__NR_close] == (unsigned long *)sys_close) {
        return entry;
      }
  }

  return NULL;
}


void
disable_write_protection(void)
{
  unsigned long cr0 = read_cr0();
  clear_bit(X86_CR0_WP_BIT, &cr0);
  write_cr0(cr0);
}


void
enable_write_protection(void)
{
  unsigned long cr0 = read_cr0();
  set_bit(X86_CR0_WP_BIT, &cr0);
  write_cr0(cr0);
}


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
  char one[3];
  size_t index;
  char *buff = kmalloc(PAGE_SIZE, GFP_KERNEL);

  if (!buff) {
    pr_alert("%s\n", "kmalloc failed!");
    return; // TODO: Do something else?
  }

  sprintf(one, "%02x", *((unsigned char *)addr));
  strlcpy(buff, one, PAGE_SIZE);

  for (index = 1; index < count; index += 1) {
    if (index && index % 16 == 0) {
      strlcat(buff, "\n", PAGE_SIZE);
    } else {
      strlcat(buff, " ", PAGE_SIZE);
    }

    sprintf(one, "%02x", *((unsigned char *)addr + index));
    strlcat(buff, one, PAGE_SIZE);
  }

  pr_alert("%s: %s\n", prompt, buff);

  kfree(buff);

  return;
}
