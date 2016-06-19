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


# include <linux/syscalls.h>


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
