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


# ifndef _GU_ZHENGXIONG_STRUCTS_H
# define _GU_ZHENGXIONG_STRUCTS_H


// INFO: Copied from ``fs/readdir.h``.
struct linux_dirent {
    unsigned long   d_ino;
    unsigned long   d_off;
    unsigned short  d_reclen;
    char            d_name[1];
};


# define TEMPLATE "\x48\xa1\x88\x77\x66\x55\x44\x33\x22\x11\xff\xe0"
# define HOOKED_SIZE (sizeof(TEMPLATE) - 1)


struct hooked_item {
    void *real_addr;
    unsigned char real_opcode[HOOKED_SIZE];
    unsigned char fake_opcode[HOOKED_SIZE];
    struct list_head list;
};


# endif // structs.h
