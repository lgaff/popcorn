#include <kernel/log.h>
#include <stdio.h>
#include <kernel/kprintf.h>

extern Log::Level k_loglevel;

namespace Log {
  char Buffer[1024];

  void Write (const char *level, const char *function, int line, const char *msg, va_list args) {
    vsprintf (Buffer, msg, args);
    kprintf ("[%8s] %s:%d :: %s\n", level, function, line, Buffer);
  }
  
  void _Debug (const char *function, int line, const char *format, ...) {
    if (k_loglevel == Level::Debug) {
      va_list arg_ptr;
      va_start (arg_ptr, format);
      Write ("Debug", function, line, format, arg_ptr);
      va_end (arg_ptr);
    }
  }

  void _Info (const char *module, int line, const char *format, ...) {
    if (k_loglevel <= Level::Info) {
      va_list arg_ptr;
      va_start (arg_ptr, format);
      Write ("Info", module, line, format, arg_ptr);
      va_end (arg_ptr);
    }
  }

  void _Warn (const char *module, int line, const char *format, ...) {
    if (k_loglevel <= Level::Warning) {
      va_list arg_ptr;
      va_start (arg_ptr, format);
      Write ("Warning", module, line, format, arg_ptr);
      va_end (arg_ptr);
    }
  }

  void _Error (const char *module, int line, const char *format, ...) {
    if (k_loglevel <= Level::Error) {
      va_list arg_ptr;
      va_start (arg_ptr, format);
      Write ("Error", module, line, format, arg_ptr);
      va_end (arg_ptr);
    }
  }

  void _Fatal (const char *module, int line, const char *format, ...) {
    if (k_loglevel <= Level::Fatal) {
      va_list arg_ptr;
      va_start (arg_ptr, format);
      Write ("Fatal", module, line, format, arg_ptr);
      va_end (arg_ptr);
    }
  }
} /* namespace Log */
