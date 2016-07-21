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
// fopen, fprintf, printf.
# include <stdio.h>
// perror.
# include <string.h>
// Get PRIx64, PRIu64.
# include <inttypes.h>

# include <elf.h>

# include "uelf.h"


int
main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "%s\n", "Invalid Arguments!");
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    if (check_elf_ident(fp) != 0) {
        return EXIT_FAILURE;
    }

    Elf64_Ehdr *header = get_elf_header(fp);
    if (header == NULL) {
        return EXIT_FAILURE;
    }

    Elf64_Shdr *sec_header_tab = get_sec_header_tab(fp, header);
    if (sec_header_tab == NULL) {
        return EXIT_FAILURE;
    }

    Elf64_Shdr *str_tab_header = sec_header_tab + header->e_shstrndx;
    char *shstrtab = get_shstrtab(fp,
                                  str_tab_header);
    if (shstrtab == NULL) {
        return EXIT_FAILURE;
    }

    Elf64_Shdr *symtab = get_section_by_name(".symtab",
                                             header,
                                             sec_header_tab,
                                             shstrtab);
    Elf64_Shdr *strtab = get_section_by_name(".strtab",
                                             header,
                                             sec_header_tab,
                                             shstrtab);
    if (symtab == NULL || strtab == NULL) {
        return EXIT_FAILURE;
    }

    Elf64_Sym *syms = malloc(symtab->sh_size);
    if (syms == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    fseek(fp, symtab->sh_offset, SEEK_SET);
    if (fread(syms, 1, symtab->sh_size, fp) != symtab->sh_size) {
        fprintf(stderr, "%s\n", "Incomplete Symbol Table!");
        return EXIT_FAILURE;
    }

    char *strs = malloc(strtab->sh_size);
    if (strs == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    fseek(fp, strtab->sh_offset, SEEK_SET);
    if (fread(strs, 1, strtab->sh_size, fp) != strtab->sh_size) {
        fprintf(stderr, "%s\n", "Incomplete String Table!");
        return EXIT_FAILURE;
    }

    printf("%s\n",
           "num    index    size    value  info other  name");
    int total = symtab->sh_size / symtab->sh_entsize;
    for (int count = 0; count < total; count += 1) {
        printf("%2u %2x %"PRIu64" %"PRIx64" %4x %4x %s\n",
               count,
               syms[count].st_shndx,
               syms[count].st_size,
               syms[count].st_value,
               syms[count].st_info,
               syms[count].st_other,
               strs + syms[count].st_name);
    }

    free(header);
    free(sec_header_tab);
    free(shstrtab);
    free(syms);
    free(strs);
    return EXIT_SUCCESS;
}
