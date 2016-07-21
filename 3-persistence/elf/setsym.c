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
# include <stdio.h> // fopen, fprintf, printf.
# include <string.h> // perror.
# include <errno.h>
# include <limits.h>
// Get PRIx64.
# include <inttypes.h>

# include <elf.h>

# include "uelf.h"


int
main(int argc, char **argv)
{
    if (argc < 3 || argc > 4) {
        fprintf(stderr, "%s\n", "Invalid Arguments!");
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r+");
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

    int total = symtab->sh_size / symtab->sh_entsize;
    for (int count = 0; count < total; count += 1) {
        if (strcmp(strs + syms[count].st_name, argv[2]) == 0) {
            if (argc == 4) {
                Elf64_Sym sym = syms[count];
                char *endp;
                errno = 0;
                unsigned long long val = strtoull(argv[3], &endp, 0);
                if ((errno == ERANGE && val == ULLONG_MAX) ||
                    (errno != 0 && val == 0)) {
                    perror("strtoull");
                    return EXIT_FAILURE;
                }
                if (endp == argv[3]) {
                    fprintf(stderr, "%s\n", "No Valid Number!");
                    return EXIT_FAILURE;
                }
                sym.st_value = val;

                long delta = count * symtab->sh_entsize;
                fseek(fp, symtab->sh_offset + delta, SEEK_SET);
                if (fwrite(&sym, 1, sizeof sym, fp) != sizeof sym) {
                    fprintf(stderr, "%s\n", "Incomplete Sym Write!");
                    return EXIT_FAILURE;
                } else {
                    fprintf(stderr, "%s\n", "Writing complete.");
                }
            } else {
                printf("0x%"PRIx64"\n", syms[count].st_value);
            }
        }
    }

    free(header);
    free(sec_header_tab);
    free(shstrtab);
    free(syms);
    free(strs);
    return EXIT_SUCCESS;
}
