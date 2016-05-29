#!/usr/bin/env bash


grep sys_close /proc/kallsyms --color=always
grep sys_open /proc/kallsyms --color=always
