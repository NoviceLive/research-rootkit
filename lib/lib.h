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


# define HOOK_SYS_CALL_TABLE(name)                     \
  do {                                                 \
    real_##name = (void *)sys_call_table[__NR_##name]; \
    sys_call_table[__NR_##name] = (void *)fake_##name; \
  } while (0)

# define UNHOOK_SYS_CALL_TABLE(name)                \
  sys_call_table[__NR_##name] = (void *)real_##name


unsigned long **
get_sys_call_table(void);


void
disable_write_protection(void);
void
enable_write_protection(void);


char *
strjoin(const char *const *strings, const char *delim,
        char *buff, size_t count);


# endif
