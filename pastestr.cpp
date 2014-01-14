#include <sstream>

#include <iostream>
using std::cout;
using std::endl;

#include <cstdarg>


#include "pastestr.hpp"

std::string pastestr::paste(const char * fmt, const char * pcSep, ...) {
  std::ostringstream ostr;
  std::string strSep;
  std::string s;
  int n = (s.assign(fmt)).size();

  std::va_list list;
  va_start(list, pcSep);
  const char * pc = NULL;
  
  if (pcSep == NULL) {
    strSep = "";
  } else {
    strSep.assign(pcSep);
  }

  for (int i = 0; i < n; i++) {
    if (i > 0) {
      ostr << strSep;
    } else {}

    switch(*(fmt+i)) {
    case 's' :
      ostr << va_arg(list, const char *);
      break;
    case 'd' :
      ostr << va_arg(list, long);
      break;
    case 'i' :
      ostr << va_arg(list, int);
      break;
    case 'u' :
      ostr << va_arg(list, unsigned int);
      break;
    }
  } 

  va_end(list);

  return ostr.str();
}
