#ifndef EXP_STARTUPINFO
#define EXP_STARTUPINFO

#include <string>
using std::string;

class StartupInfo {
 public:
  long idSession;

  string pcHost;
  string pcUser;
  string pcPass;
  string pcDB;

  int nMode;

  StartupInfo(string s = "");

  virtual ~StartupInfo();
};

#endif
