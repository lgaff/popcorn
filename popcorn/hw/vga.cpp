#include <kernel/vga.h>
#include <string.h>


namespace VGA {

  uint16_t *TextBufferBase = (uint16_t *)0xB8000;
  const unsigned int DefaultHeight = 25;
  const unsigned int DefaultWidth = 80;
  const Color4 DefaultFG = White;
  const Color4 DefaultBG = Black;  
  
  void TextBuffer::Init () {
    asm ("xchg %bx, %bx\n");    
    this->buffer = (uint16_t *)0xB8000;
    this->width = VGA::DefaultWidth;
    this->height = VGA::DefaultHeight;
    this->fg = VGA::DefaultFG;
    this->bg = VGA::DefaultBG;
    this->x = 0;
    this->y = 0;
  }

  void TextBuffer::Clear () {
    for (size_t row = 0; row < height; row++)
      {
        for (size_t col = 0; col < width; col++)
          {
            PutAt (row, col, ' ');
          }
      }
    x = 0;
    y = 0;
    Update();
  }

  void TextBuffer::Scroll () {
    x = 0;
    y++;
    if (y >= height)
    {
      memmove ((void *)buffer, (const void *)(buffer + width), width * 2 * (height - 1));
      y = height - 1;
      for (size_t col = 0; col < width; col++)
        PutAt (col, y, ' ');
    }
    Update();
  }

  /* TODO: This is not bounds checked correctly, as there's no mechanism for handling it at this stage.
     For now, out of bounds indexes fail silently. */
  void TextBuffer::PutAt (size_t row, size_t col, char c) {
    uint16_t cell = Cell(c);
    if (row < height && col < width) {
      size_t index = row * width + col;
      buffer[index] = cell;
    }
  }

  void TextBuffer::PutChar (char c) {
    if (IsPrintable(c)) {
        size_t index = y * width + x++;
        buffer[index] = Cell(c);
      }
    else {
      switch (c) {
      case '\n': Scroll(); break;
      case '\b': Backspace(); break;
      case '\t': HorizontalTab(); break;
      default: break;
      }
    if (x >= width) Scroll();
    }
  }

  void TextBuffer::Write (char const *str) {
    char const *sp = str;
    while (*sp) {
      PutChar (*sp++);
    }
  }

  void TextBuffer::WriteLine (char const *str) {
    Write (str);
    PutChar ('\n');
  }

  int TextBuffer::IsPrintable (char c) {
    return c >= ' ' && c != 0177;
  }

  void TextBuffer::Backspace() {
    if (x > 0) x--;
    PutAt (y, x, ' ');
  }

  void TextBuffer::HorizontalTab() {
    size_t offset = 8 - (x % 8);
    while (offset--) {
      PutChar (' ');
    }
  }

  void TextBuffer::Update () {
    // Do nothing for now. should update the hardware cursor.
  }
} /* namespace VGA */
