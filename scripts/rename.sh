#!/usr/bin/env bash


SED='sed --in-place'


# find -name '*.c' -or -name '*.h' | while read -r _source; do
#     ${SED} 's/disable_write_protection/disable_wp/g' "${_source}" &&
#     ${SED} 's/enable_write_protection/enable_wp/g' "${_source}"
# done


# find -name '*.c' -or -name '*.h' | while read -r _source; do
#     ${SED} 's/get_sys_call_table/get_sct/g' "${_source}"
# done
