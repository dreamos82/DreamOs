OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS
{
	. = 0x100000;

	.text : { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) *(.bss) *(COMMON) }
}
