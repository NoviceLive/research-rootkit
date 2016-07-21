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
# include <string.h> // memcmp, perror.
// Get PRIx64.
# include <inttypes.h>

# include <elf.h>


int
main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "%s\n", "Invalid Arguments!");
        return EXIT_FAILURE;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    unsigned char e_ident[EI_NIDENT];
    if (fread(e_ident, 1, EI_NIDENT, fp) != EI_NIDENT) {
        fprintf(stderr, "%s\n", "Incomplete ELF Identification!");
        return EXIT_FAILURE;
    }

    if (memcmp(e_ident, ELFMAG, SELFMAG) != 0) {
        fprintf(stderr, "%s\n", "Bad ELF Magic Number!");
        return EXIT_FAILURE;
    }

    if (e_ident[EI_CLASS] != ELFCLASS64 ||
        e_ident[EI_DATA] != ELFDATA2LSB) {
        fprintf(stderr, "%s\n", "We Only Support ELF64 LE!");
        return EXIT_FAILURE;
    }


    Elf64_Ehdr header;
    fseek(fp, 0, SEEK_SET);
    if (fread(&header, 1, sizeof header, fp) != sizeof header) {
        fprintf(stderr, "%s\n", "Incomplete ELF Header!");
        return EXIT_FAILURE;
    }

    size_t size = header.e_shnum * header.e_shentsize;
    Elf64_Shdr *section_header_table = malloc(size);
    if (section_header_table == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    fseek (fp, header.e_shoff, SEEK_SET);
    if (fread(section_header_table, 1, size, fp) != size) {
        fprintf(stderr, "%s\n", "Incomplete Section Header Table!");
        return EXIT_FAILURE;
    }

    Elf64_Shdr shstrtab = section_header_table[header.e_shstrndx];
    size = shstrtab.sh_size;
    char *string_table = malloc(size);
    if (string_table == NULL) {
        perror("malloc");
        return EXIT_FAILURE;
    }

    fseek (fp, shstrtab.sh_offset, SEEK_SET);
    if (fread(string_table, 1, size, fp) != size) {
        fprintf(stderr, "%s\n",
                "Incomplete Section Header String Table!");
        return EXIT_FAILURE;
    }

    printf("%s\n", "number offset     size   entsize   name");
    for (unsigned num = 0; num < header.e_shnum; num += 1) {
        Elf64_Shdr section_header = section_header_table[num];
        char *name = string_table + section_header.sh_name;
        printf("%4u %"PRIx64" %"PRIx64" %"PRIx64" %s\n",
               num, section_header.sh_offset,
               section_header.sh_size, section_header.sh_entsize,
               name);
    }


    free(section_header_table);
    free(string_table);
    return EXIT_SUCCESS;
}
