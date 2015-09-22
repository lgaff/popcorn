; vim:ft=nasm
; Mutliboot constants
MODULEALIGN equ 1 << 0
MEMINFO equ 1 << 1
FLAGS equ MODULEALIGN | MEMINFO
MAGIC equ 0x1BADB002
CHECKSUM equ -(MAGIC + FLAGS)

; Popcorn memory locations
KERNEL_VBASE equ 0xFFFFFFFF80000000
STACKSIZE equ 0x4000 ; 16k initial kernel stack.

; Multiboot header - should be first, load to 1MiB
section .multiboot
align 4
global mb_info
multiboot:
  dd MAGIC
  dd FLAGS
  dd CHECKSUM

; Start here in ia-32 mode.
section .start32_text
bits 32
global goshort
goshort:
; xchg bx, bx
mov esi, VERSION
call print_console
mov esi, BUILD_DATE
call print_console
; save the multiboot info in e[ab]x
mov [mb_signature - KERNEL_VBASE], eax
mov [mb_ptr - KERNEL_VBASE], ebx

; check if long mode available
mov eax, 0x80000000
cpuid
cmp eax, 0x80000001
jb nolong
mov eax, 0x80000001
cpuid
test edx, 1 << 29
jz nolong

goLong:
  ; Enable ia-32e mode
  ; set flags PSE, PG, PAE
  mov eax, cr4
  or eax, 0x80 | 0x20 | 0x10
  mov cr4, eax

  mov esi, str32e
  call print_console

  mov eax, (PML4 - KERNEL_VBASE)
  mov cr3, eax

  ; set the long mode bit in EFER
  ; TODO: enable NX/syscall later (maybe)
  mov ecx, 0xC0000080
  rdmsr
  or eax, 1 << 8
  wrmsr

  ; Enable paging
  mov ecx, cr0
  or ecx, 1 << 31
  mov cr0, ecx

  mov esi, strPgOK
  call print_console

  lgdt [GDT64.Ptr - KERNEL_VBASE]
  jmp 0x8:start64

nolong:
  mov esi, strNolong
  call print_console
.halt:
  jmp .halt

print_console:
  mov dx, 0x3F8
.print_char:
  lodsb
  cmp al, 0
  jz .done
  out dx, al
  inc ecx
  jmp .print_char
.done:
  mov dx, 0x3F8
  mov al, 0x0A
  out dx, al
  mov al, 0x0D
  out dx, al
  ret



bits 64
start64:
  mov dx, 0x3F8
  mov al, '.'
  out dx, al

  mov rax, highHalf
  jmp rax

section .text
extern _kmain
extern _init
highHalf:
        mov dx, 0x3F8
        mov al, '.'
        out dx, al
        ; crt0 begins here        
        mov rsp, stack_bottom + STACKSIZE
        ; Call libc global constructors
        xchg bx, bx
        call _init
        call _kmain

        cli
.loop:
  jmp .loop

section .start32_data
strNolong:
  db "CPU does not support long mode operation",0
  str32e db "IA-32e mode: OK",0
  strPgOK db "Paging: OK",0
  %include "src/buildver.inc"
;; TODO: fix hard link for build versions here.

section .pagedata
global PML4
PML4:
  dd PDP0 - KERNEL_VBASE + 3, 0
  times 512-2-($-PML4)/8 dq 0
  dd PML4 - KERNEL_VBASE + 3, 0
  dd KPDP - KERNEL_VBASE + 3, 0

global PDP0
PDP0:
  dd PD0 - KERNEL_VBASE + 3, 0
  times 511 dq 0
KPDP:
  times 510 dq 0
  dd PD0 - KERNEL_VBASE + 3, 0
  dq 0

; first/last GiB, with PSE means 2 MiB per page.
; ID map first 4 MiB to -2GiB, stack at top of memory
; after kernel image
global PD0
PD0:
  dd 0x000183, 0 ; 0-2 MiB
  dd 0x200183, 0 ; 2-4 MiB
  times 510 dq 0

section .data
GDT64:
  dq 0             ; Null descriptor
  dd 0, 0x00209800 ; 64 bit code
  dd 0, 0x00009000 ; 64 bit data
.Ptr:
  dw $ - GDT64 - 1
  dq GDT64 - KERNEL_VBASE
  dd 0

strBoot db "Boot: OK\n",0
strEnter db "Entering the kernel: ",0

section .bss
align 32
mb_signature:
  resb 2
mb_ptr:
  resb 2
stack_bottom:
  resb STACKSIZE
