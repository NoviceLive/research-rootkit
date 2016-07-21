#!/usr/bin/env bash


find -name Makefile -or -name makefile | while read -r _file; do
    _dir="$(dirname "${_file}")"
    (cd "${_dir}" && make "${@}" && printf "PASS: %s\n" "${_dir}")
done
