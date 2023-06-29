// this is the structure that holds configuration values for the
// experiment in a key, value map

#include "global.hpp"
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "pastestr.hpp"
#include "sys/stat.h"
#include "dirent.h"

using std::string;

EXPConfig::EXPConfig(unsigned long eid /*=0*/) {
  string qstr;
  struct stat st;

  if (g_prsStim) {
  } else {
    g_pErr->Report("Could not read configuration information from database.");
  }
  if (eid>0) {
    qstr = "SELECT Var, Data FROM EXPConfig WHERE ExpID=" + eid;
  } else {
    // eid not defined; just pull in all configuration information
    qstr = "SELECT Var, Data FROM EXPConfig"; 
  }
  if (g_prsStim->TableExists("EXPConfig")) { // backwards compatibility
    g_prsStim->ExecCount(qstr);
    if (g_prsStim->RowReady()) {
      do {
	m_configMap[g_prsStim->Get(0)] = g_prsStim->Get(1);
	g_pErr->Debug(pastestr::paste("sss", "",
				      g_prsStim->Get(0), " = ", g_prsStim->Get(1)));
      } while (g_prsStim->MoveNext());
    } else {}
  } else {
    g_pErr->Debug("...TABLE EXPConfig NOT FOUND!...");
  }

  if (stat(".private", &st) < 0) {
    g_pErr->Debug(".private directory does not exist");
  } else {
    // read private variables.
    // TODO: don't allow overlap with experiment variables
    g_pErr->Debug("checking for variables in subdirectory .private...");
    DIR * pdir = opendir(".private");
    struct dirent * dp = NULL;
    struct stat statbuf;
    std::ifstream infile;
    string fname;
    string sline;
    
    while ((dp = readdir(pdir)) != NULL) {
      fname = pastestr::paste("ss", "", ".private/", dp->d_name);
      if (stat(fname.c_str(), &statbuf) == -1)
	continue;

      if (S_ISREG(statbuf.st_mode)) {
	infile.open(fname.c_str());
	if (infile.good()) {
	  getline(infile, sline);
	} else {
	  g_pErr->Report(pastestr::paste("sss", "",
					 "can't read from file '",
					 fname.c_str(), "'; check permissions"));
	}
	infile.close();
	m_configMap[dp->d_name] = sline;
	g_pErr->Debug(pastestr::paste("sssss", "",
				      "private var: ", dp->d_name,
				      " = '",
				      m_configMap[dp->d_name].c_str(), "'"));
      }
    }
    closedir(pdir);
  }
  
  //	g_prsStim->ExecCount();
}

EXPConfig::~EXPConfig() {
  m_configMap.clear();
}

bool EXPConfig::GetConfig(string key, string *ps) {
  *ps = "";
  bool result = true;

  try {
    *ps = m_configMap.at(key);
  } catch (const std::out_of_range& e) {
    result = false;
  }
  return result;
}


bool EXPConfig::GetConfigInt(string key, int *pn) {
  string val;
  bool bRes = GetConfig(key, &val);
  if (bRes) {
    std::istringstream(val) >> (*pn);
  } else {}

  return bRes;
}
