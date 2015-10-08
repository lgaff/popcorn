#ifndef _LOG_H
#define _LOG_H

#include <stdarg.h>

namespace Log {

enum class Level {
  Debug = 0,
  Info = 1,
  Warning = 2,
  Error = 3,
  Fatal = 4
};

  //  class Log {
  //  public:
  //    Log (enum Level);
  void Write (const char *, const char *, int, const char *, va_list);  
  void _Debug (const char *, int, const char *, ...);
#define Debug(...) _Debug(__FUNCTION__, __LINE__, __VA_ARGS__)
  void _Info (const char *, int, const char *, ...);
#define Info(...) _Info(__FUNCTION__, __LINE__, __VA_ARGS__)
  void _Warn (const char *, int, const char *, ...);
#define Warn(...) _Warn(__FUNCTION__, __LINE__, __VA_ARGS__)
  void _Error (const char *, int, const char *, ...);
#define Error(...) _Error(__FUNCTION__, __LINE__, __VA_ARGS__)
  void _Fatal (const char *, int, const char *, ...);
#define Fatal(...) _Fatal(__FUNCTION__, __LINE__, __VA_ARGS__)
  
  
  //    void Info (const char *, int, const char *, ...);
  //    void Warn (const char *, int, const char *, ...);
  //    void Error (const char *, int, const char *, ...);
  //    void Fatal (const char *, int, const char *, ...);
  //    void Level (enum Level);
  //    enum Level Level ();
  //  private:

  //    enum Level level;
  //  };
} /* namespace Log */

#endif /* _LOG_H */
