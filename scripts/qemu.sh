#!/bin/sh

qemu-system-x86_64 -drive file=avuOS.img,cache=directsync,format=raw,id=disk,if=ide