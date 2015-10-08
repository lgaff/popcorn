#ifndef _KERNEL_VGA_H
#define _KERNEL_VGA_H

#include <stdint.h>
#include <stddef.h>

namespace VGA {

  
  enum Color4 : uint8_t {
    Black = 0,
    Blue,
    Green,
    Cyan,
    Red,
    Magenta,
    Brown,
    LightGrey,
    DarkGrey,
    LightBlue,
    LightGreen,
    LightCyan,
    LightRed,
    LightMagenta,
    LightBrown,
    White
  };


  enum Blink : uint8_t {
    True = 0,
    False = 0x80
  };

  class TextBuffer {
  public:
    void Init ();
    void Clear ();
    void Scroll ();
    void SetX (size_t newX) { x = newX; }
    void SetY (size_t newY) { y = newY; }
    void SetXY (size_t newX, size_t newY) { x = newX; y = newY; }
    void SetFG (enum Color4 newFG) { fg = newFG; }
    void setBG (enum Color4 newBG) { bg = newBG; }
    void PutChar (char);
    void PutAt (size_t, size_t, char);
    void Write (char const *);
    void WriteLine (char const *);
    uint16_t *GetBuffer ();
  private:
    unsigned int x;
    unsigned int y;
    enum Color4 fg;
    enum Color4 bg;
    enum Blink blink;
    unsigned int width;
    unsigned int height;
    uint16_t *buffer;
    uint8_t Attribute() {
      return (bg << 4 | fg & 0x0F) | blink;
    }
    uint16_t Cell(char c) {
      return (uint16_t)this->Attribute() << 8 | c;
    }
    int IsPrintable (char);
    void Backspace ();
    void HorizontalTab ();
    void Update ();
  };
    
} /* namespace VGA */

#endif
