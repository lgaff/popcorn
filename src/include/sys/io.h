#ifndef _IO_H
#define _IO_H

  static inline void outb (uint16_t port, uint8_t val);
  static inline void outw (uint16_t port, uint16_t val);
  static inline void outl (uint16_t port, uint32_t val);

  static inline uint8_t inb (uint16_t port);
  static inline uint16_t inw (uint16_t port);
  static inline uint32_t inl (uint16_t port);
#endif
