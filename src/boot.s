MBOOT_MAGIC equ 0x1BADB002
MBOOT_FLAGS equ 0x00000000
MBOOT_CHECKSUM equ -(MBOOT_MAGIC + MBOOT_FLAGS)

[BITS 32]

section .text
    dd MBOOT_MAGIC
    dd MBOOT_FLAGS
    dd MBOOT_CHECKSUM

global _start
extern main

_start:
    cli
    call main
    hlt 
