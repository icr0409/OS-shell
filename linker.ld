ENTRY(_start)

SECTIONS
{
    . = 2M;

    _kernel_start = .;

    .text BLOCK(4K) : ALIGN(4K)
    {
        _text_start = .;
        *(.multiboot)
        *(.text)
        _text_end = .;
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        _rodata_start = .;
        *(.rodata)
        _rodata_end = .;
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        _data_start = .;
        *(.data)
        _rodata_end = .;
    }

    .bss BLOCK(4K) : ALIGN(4K)
    {
        _bss_start = .;
        *(COMMON)
        *(.bss)
        _bss_end = .;
    }

    _kernel_end = .;
}