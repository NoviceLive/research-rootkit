#!/usr/bin/env bash


NAME='zeroevil'

SED='sed --in-place'
LINK='ln --symbolic --relative --force --no-target-directory'


find -name Makefile | while read -r _link; do
    ${LINK} ${NAME} "$(dirname "${_link}")/${NAME}"
done


# find -name Makefile | while read -r _file; do
#     ${SED} 's/EXTRA_CFLAGS = "\-I${PWD}\/lib"//g' "${_file}" &&
#         ${SED} "s/lib\/lib.o/${NAME}\/${NAME}.o/g" "${_file}"
# done


# find -name '*.c' -or -name '*.h' | while read -r _source; do
#     ${SED} "s/lib\/lib.h/${NAME}\/${NAME}.h/g" ${_source} &&
#         ${SED} "s/\"lib.h\"/\"${NAME}\/${NAME}.h\"/g" ${_source}
# done
