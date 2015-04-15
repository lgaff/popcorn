;; Disable kmain call for now while we setup the inital environment.
; extern _kmain

MODULEALIGN equ 1 << 0
MEMINFO equ 1 << 1
FLAGS equ MODULEALIGN | MEMINFO
;; This is the old GRUB 1 magic number, should still work, but may need changing.
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

KERNEL_VBASE equ 0xc0000000
KERNEL_PNUM equ (KERNEL_VBASE >> 22)

section .data
align 0x1000
%include "src/buildver.inc"

BootPageDir:
  ; 4 meg page, read/write, page is present (bits 7, 1, 0)
  dd 0x00000083 ; (bootPTE-KERNEL_VBASE) + 0x3
  times (KERNEL_PNUM -1) dd 0 ; empty pages before kernel space
  dd 0x00000083 ; hhPTE - KERNEL_VBASE + 0x3
  times (1024 - KERNEL_PNUM -1) dd 0

section .mboot
align 4
MultibootHeader:
  dd MAGIC
  dd FLAGS
  dd CHECKSUM

STACKSIZE equ 0x4000
global start
start:
  ; Need to enable paging and jump to 3GiB.
  xchg bx, bx
  mov ecx, (BootPageDir - KERNEL_VBASE)
  mov cr3, ecx

  mov ecx, cr4
  or ecx,  0x00000010 ; Page size enable.
  mov cr4, ecx

  mov ecx, cr0
  or ecx, 0x80000000 ; PG enable
  mov cr0, ecx

  ; Paging is enabled, we should be in kernel space now. Will need a long jump to set eip correctly
  ; as it still has the paddr
  lea ecx, [KernelHigh]
  jmp ecx

section .text
align 4
KernelHigh:
  ; Here's where we should invalidate the 0th page, but we still need it for now for VGA terminal.
;  mov dword [BootPageDir], 0
;  invlpg [0]

  mov esp, stack + STACKSIZE
  push eax  ; Multiboot magic number
  push ebx  ; Multiboot info paddr
.pingconsole:
  mov ecx, VERSION
  call print_console

;;  call _kmain
  hlt
.loop:
  jmp .loop

print_console:
  mov dx, 0x3F8
.print_char:
  mov al, [ecx]
  cmp al, 0
  jz .done
  out dx, al
  inc ecx
  jmp .print_char
.done:
  ret


section .bss
align 32
stack:
  resb STACKSIZE
