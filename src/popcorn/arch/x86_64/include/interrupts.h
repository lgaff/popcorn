// interrupts.h contains declarations for isr's and irq's
/* WARNING WARNING WARNING WARNING
   This code was lifted straight from Aspidistra to diagnose a problem elsewhere.
   it is -NOT- considered safe or part of any final solution to interrupt handling
   in popcorn. Remove as soon as done.
*/
#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include <stdint.h>

typedef struct registers
{
  uint64_t ds;
  uint8_t interrupt_number, error_code;
  uint64_t eip, cs, eflags, useresp, ss;
} registers_t;

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

void initialise_idt();

extern void idt_flush();

struct idt_entry
{
  uint16_t base_low;
  uint16_t selector;
  uint8_t always0;
  uint8_t flags;
  uint16_t base_middle;
  uint32_t base_high;
  uint32_t zero;
}__attribute__((packed));
typedef struct idt_entry idt_entry_t;

struct idt_ptr
{
  uint16_t limit;
  uint64_t base;
}__attribute__((packed));
typedef struct idt_ptr idt_ptr_t;

typedef void (*isr_t)(); 
void register_interrupt_handler(uint8_t index, isr_t handler);

void remap_irq();

// these isr's will be defined in assembly elsewhere.
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#endif /* INTERRUPTS_H */
