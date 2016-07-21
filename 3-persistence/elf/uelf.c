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


# include <stdlib.h>
# include <stdio.h>
# include <string.h>

# include <elf.h>

# include "uelf.h"


int
check_elf_ident(FILE *stream)
{
    int ret = 0;
    unsigned char e_ident[EI_NIDENT];
    long offset = ftell(stream);

    if (fread(e_ident, 1, EI_NIDENT, stream) != EI_NIDENT) {
        fprintf(stderr, "%s\n", "Incomplete ELF Identification!");
        goto restore_file_offset_and_return_error;
    }

    if (memcmp(e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "%s\n", "Bad ELF Magic Number!");
        goto restore_file_offset_and_return_error;
    }

    if (e_ident[EI_CLASS] != ELFCLASS64 ||
        e_ident[EI_DATA] != ELFDATA2LSB) {
        fprintf(stderr, "%s\n", "We Only Support ELF64 LE!");
        goto restore_file_offset_and_return_error;
    }

    fseek(stream, offset, SEEK_SET);
    return 0;

 restore_file_offset_and_return_error:
    fseek(stream, offset, SEEK_SET);
    return 1;
}


Elf64_Ehdr *
get_elf_header(FILE *stream)
{
    Elf64_Ehdr *header = malloc(sizeof *header);
    if (header == NULL) {
        perror("malloc");
        return NULL;
    }

    long offset = ftell(stream);
    if (fread(header, 1, sizeof *header, stream) != sizeof *header) {
        fprintf(stderr, "%s\n", "Incomplete ELF Header!");
        header = NULL;
    }

    fseek(stream, offset, SEEK_SET);
    return header;
}


Elf64_Shdr *
get_sec_header_tab(FILE *stream, Elf64_Ehdr *header)
{
    size_t size = header->e_shnum * header->e_shentsize;
    long offset = ftell(stream);

    Elf64_Shdr *sec_header_tab = malloc(size);
    if (sec_header_tab == NULL) {
        perror("malloc");
        goto restore_file_offset_and_return_error;
    }

    fseek (stream, header->e_shoff, SEEK_CUR);
    if (fread(sec_header_tab, 1, size, stream) != size) {
        fprintf(stderr, "%s\n", "Incomplete Section Header Table!");
        goto restore_file_offset_and_return_error;
    }

    fseek(stream, offset, SEEK_SET);
    return sec_header_tab;

 restore_file_offset_and_return_error:
    fseek(stream, offset, SEEK_SET);
    return NULL;
}


char *
get_shstrtab(FILE *stream,
             Elf64_Shdr *str_tab_header)
{
    size_t size = str_tab_header->sh_size;
    long offset = ftell(stream);

    char *str_tab = malloc(size);
    if (str_tab == NULL) {
        perror("malloc");
        goto restore_file_offset_and_return_error;
    }

    fseek (stream, str_tab_header->sh_offset, SEEK_CUR);
    if (fread(str_tab, 1, size, stream) != size) {
        fprintf(stderr, "%s\n",
                "Incomplete Section Header String Table!");
        goto restore_file_offset_and_return_error;
    }

    fseek(stream, offset, SEEK_SET);
    return str_tab;

 restore_file_offset_and_return_error:
    fseek(stream, offset, SEEK_SET);
    return NULL;
}


Elf64_Shdr *
get_section_by_name(const char *name,
                    Elf64_Ehdr *header,
                    Elf64_Shdr *sec_header_tab,
                    char *str_tab)
{
    for (unsigned num = 0; num < header->e_shnum; num += 1) {
        Elf64_Shdr *sec_header = sec_header_tab + num;
        char *sec_name = str_tab + sec_header->sh_name;
        if (strcmp(sec_name, name) == 0) {
            return sec_header;
        }
    }

    return NULL;
}
