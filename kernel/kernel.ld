ENTRY(start)

SECTIONS {
    . = 0xC0100000;

    PROVIDE(_KERNEL_START = .);

    .text : AT(ADDR(.text) - 0xC0000000)
    {
		*(.multiboot*)
        *(.text*)
    }

    .data : AT(ADDR(.data) - 0xC0000000)
    {
        *(.data*)
    }

    .bss : AT(ADDR(.bss) - 0xC0000000)
    {
        *(COMMON)
        *(.bss*)
    }

    PROVIDE(_KERNEL_END = .);
}