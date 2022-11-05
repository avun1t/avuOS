#!/bin/sh

qemu-system-x86_64 -drive file=avuOS.img,cache=directsync,format=raw -device ich9-ahci