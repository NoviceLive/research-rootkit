#!/usr/bin/env bash


printf '%s\n' 'movq $0x1122334455667788, %rax; jmpq *%rax' |
    as &&
    objdump -d a.out -Mintel &&
    rm -rf a.out

# 0:   48 b8 88 77 66 55 44    movabs rax,0x1122334455667788
# 7:   33 22 11
# a:   ff e0                   jmp    rax
#
# 48b88877665544332211ffe0
# \x48\xb8\x88\x77\x66\x55\x44\x33\x22\x11\xff\xe0


printf '%s\n' 'pushl $0x11223344; ret' |
    as -32 &&
    objdump -d a.out -Mintel &&
    rm -rf a.out

# 0:   68 44 33 22 11          push   0x11223344
# 5:   c3                      ret
#
# 6844332211c3
# \x68\x44\x33\x22\x11\xc3
