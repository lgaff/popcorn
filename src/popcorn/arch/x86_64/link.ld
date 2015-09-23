ENTRY(goshort)
OUTPUT_FORMAT(elf64-x86-64)
_kernel_vbase = 0xFFFFFFFF80000000;

SECTIONS {
  . = 0x100000;

  .boot : AT(ADDR(.boot)) {
    KEEP(*(.multiboot))
    *(.multiboot)    
    *(.start32_text)
    *(.start32_data)
  }

        .init ALIGN(0x1000): AT(ADDR(.init)) {
        KEEP(*(.init))
        *(.init)
        *(.init_array)
        *(.jcr)
        }
.fini ALIGN(0x1000) : AT(ADDR(.fini)) {
        *(.fini)
        *(.fini_array)
        *(.tm_clone_table)
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

.eh_frame : AT(ADDR(.eh_frame) - _kernel_vbase) {
        *(.eh_frame_hdr)
        *(.eh_frame)
        }
        

  .bss : AT(ADDR(.bss) - _kernel_vbase) {
    _sbss = .;
    *(COMMON)
    *(.bss)
    _ebss = .;
        }

        /DISCARD/ : {
        *(.note)
        *(.note.*)
        }
}
