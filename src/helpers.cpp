#include <string>
using std::string;

#include <iostream>
using std::cout;
using std::endl;


string CheckSlashes(const char *pc) {
  string s(pc);

#ifdef WIN32
  size_t spos = 0;

  while ((spos = s.find("/", spos)) != s.npos) {
    s.replace(spos, 1, "\\");
    spos++;
  }

  //cout << s << endl;
#else
  size_t spos = 0;

  while ((spos = s.find("\\", spos)) != s.npos) {
    s.replace(spos, 1, "/");
    spos++;
  }
#endif

  return s;
}

string CheckSlashes(string s) {
  return CheckSlashes(s.c_str());
}
