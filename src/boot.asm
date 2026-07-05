MB_MAGIC equ 0x1BADB002
MB_FLAGS equ 0x00000000
MB_CHECKSUM equ -(MB_MAGIC + MB_FLAGS)

[BITS 32]

section .text
    dd MB_MAGIC
    dd MB_FLAGS
    dd MB_CHECKSUM

global _start
extern kmain

_start:
    cli
    call kmain
    hlt 
