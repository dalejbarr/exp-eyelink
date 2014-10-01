// this is the structure that holds configuration values for the
// experiment in a key, value map

#include "global.hpp"
#include <sstream>
#include <stdexcept>
#include "pastestr.hpp"

using std::string;

EXPConfig::EXPConfig(unsigned long eid /*=0*/) {
	string qstr;

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
				g_pErr->Debug(pastestr::paste("sss", "", g_prsStim->Get(0), " = ", g_prsStim->Get(1)));
			} while (g_prsStim->MoveNext());
		} else {}
	} else {
		g_pErr->Debug("...TABLE EXPConfig NOT FOUND!...");
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
