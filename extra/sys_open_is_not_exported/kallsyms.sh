#!/usr/bin/env bash


grep -e '\bsys_open\b' -e '\bsys_read\b' \
     -e '\bsys_write\b' -e '\bsys_close\b' \
     /proc/kallsyms --color=always | less
