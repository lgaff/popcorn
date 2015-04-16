; vim:ft=nasm
;; Console writing routines for early boot
global print_console
global console_nl

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

console_nl:
  mov dx, 0x3F8
  mov al, 0x0A
  out dx, al
  mov al, 0x0D
  out dx, al
  ret


