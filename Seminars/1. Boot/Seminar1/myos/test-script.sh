#!/bin/bash
nasm -f elf32 kernel.asm
ld -m elf_i386 -o kernel.bin -T linker.ld multiboot_header.o start.o myos.o
gcc -m32 -c myos.c
mv kernel.bin ./cdrom/boot
mv myos.o ./cdrom/boot
grub-mkrescue -o cdrom.iso cdrom
qemu-system-x86_64 -cdrom cdrom.iso
