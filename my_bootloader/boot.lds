SECTIONS{
    . = 0x33f80000;      /*0x34000000 - 0x33f80000 = 512k  给boot留下的空间*/
    .text    : { *(.text) }

    . = ALIGN(4);
    .rodata  : { *(.rodata) }

    . = ALIGN(4);
    .data    : { *(.data) }

    . = ALIGN(4);
    __bss_start = .;
    .bss     : { *(.bss) *(.COMMON) }
    __bss_end = .;

}
