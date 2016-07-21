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


# ifndef _GU_ZHENGXIONG_UELF_H
# define _GU_ZHENGXIONG_UELF_H


int
check_elf_ident(FILE *stream);
Elf64_Ehdr *
get_elf_header(FILE *stream);
Elf64_Shdr *
get_sec_header_tab(FILE *stream, Elf64_Ehdr *header);
char *
get_shstrtab(FILE *stream,
             Elf64_Shdr *str_tab_header);
Elf64_Shdr *
get_section_by_name(const char *name,
                    Elf64_Ehdr *header,
                    Elf64_Shdr *sec_header_tab,
                    char *str_tab);


# endif
