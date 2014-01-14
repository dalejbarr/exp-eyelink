#ifndef EXP_PASTESTR_INCLUDED
#define EXP_PASTESTR_INCLUDED

#include <string>

namespace pastestr {
  std::string paste(const char * fmt, const char * pcSep = NULL, ...);
  }

#endif
