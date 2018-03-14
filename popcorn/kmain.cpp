#include <stdint.h>

#include <multiboot.h>

#include <kernel/io.h>
#include <kernel/interrupts.h>
#include <kernel/panic.h>
#include <kernel/log.h>
#include <kernel/vga.h>


Log::Level k_loglevel = Log::Level::Info;
VGA::TextBuffer tty0;
  


extern "C" void _kmain (multiboot_info_t *mbd, unsigned int magic)
{
  tty0.Init();
  /* Test code follows. delete me when kdev gets going. */
  /* This should still be identity mapped */
   Log::Info ("popcorn kernel 0.0.2-very-alpha");
   Log::Info ("Multiboot magic: %x", magic);
   initialise_idt ();
   Log::Warn ("Interrupts enabled (but not configured)");
   Log::Debug ("Checking Multiboot info");
   if (mbd->flags & MULTIBOOT_INFO_MEMORY) {
     Log::Debug ("Multiboot info exists for lower and upper mem");
     asm ("xchg %bx, %bx\n");
     uint16_t lower;
     uint16_t upper;

     lower = mbd->mem_lower;
     upper = mbd->mem_upper;
     Log::Info ("Lower mem %dK", mbd->mem_lower);
     Log::Info ("Upper mem %dK", mbd->mem_upper);
     Log::Info ("Total mem %dK", mbd->mem_lower + mbd->mem_upper);

     
     if (mbd->flags & MULTIBOOT_INFO_MEM_MAP) {
       uint8_t * rp = (uint8_t *)mbd->mmap_addr;
       Log::Debug("Multiboot structure at %x length %x", mbd->mmap_addr, mbd->mmap_length);
       size_t mmap_end = mbd->mmap_addr + mbd->mmap_length;
       Log::Debug("mmap start %x end %x", mbd->mmap_addr, mmap_end);
       Log::Info("Available memory regions:");
       Log::Info("Base\t\tLength\t\tType");

       multiboot_memory_map_t *region;
       char * type = NULL;
       while ((size_t)rp < mmap_end) {
         region = (multiboot_memory_map_t *)rp;

         switch (region->type) {
         case MULTIBOOT_MEMORY_AVAILABLE: type= (char *)"Available"; break;
           case MULTIBOOT_MEMORY_RESERVED: type= "Reserved"; break;
           case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE: type= "ACPI"; break;
           case MULTIBOOT_MEMORY_NVS: type="Non-volatile"; break;
           case MULTIBOOT_MEMORY_BADRAM:type= "Bad"; break;
           default: "Unknown";
         }
         Log::Debug("next mmap entry at %x size %d", rp, region->size);
         Log::Info("%8x\t%d\t%s", region->addr, region->len, type);
         rp += region->size + sizeof(uint32_t);
       }
     }
       
   }
   else {
     Log::Fatal ("No Multiboot info exists, cannot determine memory layout");
     panic();
   }
     
     
   panic();
  for(;;) {}
}
