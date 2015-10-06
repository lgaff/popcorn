#include <kernel/tty.h>
#include <kernel/io.h>
#include <interrupts.h>
#include <string.h>

isr_t interrupt_handlers[256];
idt_entry_t idt_entries[256]; 
idt_ptr_t idt_pointer;


static char *interrupt_types[19] =
  {
    "Division by zero",
    "Debug exception",
    "Non-maskable interrupt",
    "Breakpoint encountered",
    "Overflow detected",
    "Out of bounds",
    "Invalid opcode",
    "Coprocessor not present",
    "Double fault",
    "Coprocessor segment overrun",
    "Bad TSS",
    "Segment not present",
    "Stack fault",
    "General protection fault",
    "Page fault",
    "Unknown interrupt",
    "Coprocessor fault",
    "Alignment check exception",
    "Machine check exception"
  };

static void idt_set_gate(uint8_t index, uint64_t base, uint16_t selector, uint8_t flags)
{
  idt_entries[index].base_low = base & 0xFFFF;
  idt_entries[index].base_middle = (base >> 16) & 0xFFFF;
  idt_entries[index].base_high = (base >> 32) & 0xFFFFFFFF;

  idt_entries[index].selector = selector;
  idt_entries[index].always0 = 0;
  idt_entries[index].zero = 0;
  idt_entries[index].flags = flags /* | 0x60 */;
}

void isr_handler(uint8_t interrupt_number)
{
  asm ("xchg %bx, %bx\n");
  tty_writestring ( "PANIC: ");
  tty_writestring(interrupt_types[interrupt_number]);
  for(;;);
}

void irq_handler(registers_t regs)
{
  // Send an end of interrupt signal to the PIC. 
  if(regs.interrupt_number >= 40) // need to signal the slave as well.
    {
      outb(0xA0, 0x20);
    }

  outb(0x20, 0x20); // Either way, we need to signal the master.
  if (interrupt_handlers[regs.interrupt_number] != 0)
    {
      isr_t handler = interrupt_handlers[regs.interrupt_number];
      handler(regs);
    }
}

void register_interrupt_handler(uint8_t index, isr_t handler)
{
  interrupt_handlers[index] = handler;
}

 
idt_entry_t idt_entries[256]; 
idt_ptr_t idt_pointer;

void initialise_idt()
{
  idt_pointer.limit = sizeof(idt_entry_t) * 256 -1;
  idt_pointer.base = (uint64_t)&idt_entries;


  memset((uint8_t *)&idt_entries, 0, sizeof(idt_entry_t)*256);


  // The 8259 PIC(s) irq lines on the PC are by default configured to listen on the first 16 interrupt gates.
  // This causes a problem when anything routine happens like say, a page fault. 
  // To get around this, it's necessary as part of the startup process to map the 16 IRQ lines onto interrupt gates
  // that are not in use.
  outb(0x20, 0x11);
  outb(0xA0, 0x11);
  outb(0x21, 0x20);
  outb(0xA1, 0x28);
  outb(0x21, 0x04);
  outb(0xA1, 0x02);
  outb(0x21, 0x01);
  outb(0xA1, 0x01);
  outb(0x21, 0x0);
  outb(0xA1, 0x0);

  idt_set_gate(0, (uint64_t)isr0, 0x08, 0x8E);
  idt_set_gate(1, (uint64_t)isr1, 0x08, 0x8E);
  idt_set_gate(2, (uint64_t)isr2, 0x08, 0x8E);
  idt_set_gate(3, (uint64_t)isr3, 0x08, 0x8E);
  idt_set_gate(4, (uint64_t)isr4, 0x08, 0x8E);
  idt_set_gate(5, (uint64_t)isr5, 0x08, 0x8E);
  idt_set_gate(6, (uint64_t)isr6, 0x08, 0x8E);
  idt_set_gate(7, (uint64_t)isr7, 0x08, 0x8E);
  idt_set_gate(8, (uint64_t)isr8, 0x08, 0x8E);
  idt_set_gate(9, (uint64_t)isr9, 0x08, 0x8E);
  idt_set_gate(10, (uint64_t)isr10, 0x08, 0x8E);
  idt_set_gate(11, (uint64_t)isr11, 0x08, 0x8E);
  idt_set_gate(12, (uint64_t)isr12, 0x08, 0x8E);
  idt_set_gate(13, (uint64_t)isr13, 0x08, 0x8E);
  idt_set_gate(14, (uint64_t)isr14, 0x08, 0x8E);
  idt_set_gate(15, (uint64_t)isr15, 0x08, 0x8E);
  idt_set_gate(16, (uint64_t)isr16, 0x08, 0x8E);
  idt_set_gate(17, (uint64_t)isr17, 0x08, 0x8E);
  idt_set_gate(18, (uint64_t)isr18, 0x08, 0x8E);
  idt_set_gate(19, (uint64_t)isr19, 0x08, 0x8E);
  idt_set_gate(20, (uint64_t)isr20, 0x08, 0x8E);
  idt_set_gate(21, (uint64_t)isr21, 0x08, 0x8E);
  idt_set_gate(22, (uint64_t)isr22, 0x08, 0x8E);
  idt_set_gate(23, (uint64_t)isr23, 0x08, 0x8E);
  idt_set_gate(24, (uint64_t)isr24, 0x08, 0x8E);
  idt_set_gate(25, (uint64_t)isr25, 0x08, 0x8E);
  idt_set_gate(26, (uint64_t)isr26, 0x08, 0x8E);
  idt_set_gate(27, (uint64_t)isr27, 0x08, 0x8E);
  idt_set_gate(28, (uint64_t)isr28, 0x08, 0x8E);
  idt_set_gate(29, (uint64_t)isr29, 0x08, 0x8E);
  idt_set_gate(30, (uint64_t)isr30, 0x08, 0x8E);
  idt_set_gate(31, (uint64_t)isr31, 0x08, 0x8E);
  // 8259 PIC controlled IRQ's follow.
  idt_set_gate(32, (uint64_t)irq0, 0x08, 0x8E);
  idt_set_gate(33, (uint64_t)irq1, 0x08, 0x8E);
  idt_set_gate(34, (uint64_t)irq2, 0x08, 0x8E);
  idt_set_gate(35, (uint64_t)irq3, 0x08, 0x8E);
  idt_set_gate(36, (uint64_t)irq4, 0x08, 0x8E);
  idt_set_gate(37, (uint64_t)irq5, 0x08, 0x8E);
  idt_set_gate(38, (uint64_t)irq6, 0x08, 0x8E);
  idt_set_gate(39, (uint64_t)irq7, 0x08, 0x8E);
  idt_set_gate(40, (uint64_t)irq8, 0x08, 0x8E);
  idt_set_gate(41, (uint64_t)irq9, 0x08, 0x8E);
  idt_set_gate(42, (uint64_t)irq10, 0x08, 0x8E);
  idt_set_gate(43, (uint64_t)irq11, 0x08, 0x8E);
  idt_set_gate(44, (uint64_t)irq12, 0x08, 0x8E);
  idt_set_gate(45, (uint64_t)irq13, 0x08, 0x8E);
  idt_set_gate(46, (uint64_t)irq14, 0x08, 0x8E);
  idt_set_gate(47, (uint64_t)irq15, 0x08, 0x8E);

  idt_flush((uint64_t)&idt_pointer);
}
