#include <sstream>
using std::ostringstream;

#include <iostream>
using std::cout;
using std::endl;

#include <cstdlib>

#include "ReportError.hpp"

ReportError::ReportError() {
}

int ReportError::Report(const char * pc, int bFatal) {
  cout << pc << endl;

  if (bFatal) {
    Exit();
  } else {}

  return 0;
}

int ReportError::Report(std::string s, int bFatal) {
  Report(s.c_str());
}

int ReportError::Exit(int nExitCode) {

  std::exit(nExitCode);

  return 0;
}

int ReportError::Debug(std::string s) {
  return Debug(s.c_str());
}

int ReportError::Debug(const char *pc) {
#ifdef DEBUG_MODE
  cout << pc << endl;
#endif
  return 0;
}

int ReportError::DFI(const char * pcFn, const char * pcInfo, int nLevel) {
  ostringstream ostr;

  for (int i = 1; i < nLevel; i++) {
    ostr << "   ";
  }
  ostr << "<--- Entering " << pcFn << "()";
  if (pcInfo) {
    ostr << " " << pcInfo;
  } else {}
  ostr << " --->";
  Debug(ostr.str());
  return 0;
}

int ReportError::DFO(const char * pcFn, const char * pcInfo, int nLevel) {
  ostringstream ostr;

  for (int i = 1; i < nLevel; i++) {
    ostr << "   ";
  }
  ostr << "<--- Exiting " << pcFn << "()";
  if (pcInfo) {
    ostr << " " << pcInfo;
  } else {}
  ostr << " --->";
  Debug(ostr.str());

  return 0;
}

int ReportError::DFI(const char * pcFn, long lg, int nLevel) {
  ostringstream ostr;

  ostr << lg;
  DFI(pcFn, ostr.str().c_str(), nLevel);
}

int ReportError::DFO(const char * pcFn, long lg, int nLevel) {
  ostringstream ostr;

  ostr << lg;
  DFO(pcFn, ostr.str().c_str(), nLevel);
}
