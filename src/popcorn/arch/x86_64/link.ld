ENTRY(goshort)
OUTPUT_FORMAT(elf64-x86-64)
_kernel_vbase = 0xFFFFFFFF80000000;

SECTIONS {
  . = 0x100000;

  .boot : AT(ADDR(.boot)) {
    KEEP(*(.multiboot))
    *(.start32_text)
    *(.start32_data)
  }
        . += _kernel_vbase;

  .text : AT(ADDR(.text) - _kernel_vbase) {
    *(.text)
    *(.text.*)
    *(.rodata*)
        }

  .data ALIGN (0x1000) : AT(ADDR(.data) - _kernel_vbase) {
    *(.pagedata)
    *(.data)

  }

  .bss : AT(ADDR(.bss) - _kernel_vbase) {
    _sbss = .;
    *(COMMON)
    *(.bss)
    _ebss = .;
  }
}
