#ifndef REPORTERR_INCLUDED
#define REPORTERR_INCLUDED

#include <string>

class ReportError {
 public:
  ReportError();
  virtual int Report(const char *, int bFatal = 1);
  virtual int Report(std::string, int bFatal = 1);
  virtual int Debug(const char *);
  virtual int Debug(std::string);
  virtual int DFI(const char * pcFn, const char * pcInfo, int nLevel = 1);
  virtual int DFI(const char * pcFn, long lg, int nLevel = 1);
  virtual int DFO(const char * pcFn, const char * pcInfo, int nLevel = 1);
  virtual int DFO(const char * pcFn, long lg, int nLevel = 1);
  virtual int Exit(int nExitCode = 0);
};

#endif
