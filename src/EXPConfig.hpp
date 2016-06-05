#ifndef EXPCONFIG_INCLUDED
#define EXPCONFIG_INCLUDED

#include <string>
#include <map>

class EXPConfig {
protected:
	std::map<std::string, std::string> m_configMap;
public:
	EXPConfig(unsigned long eid = 0);
	virtual ~EXPConfig();
	bool GetConfig(std::string key, std::string *ps);
	bool GetConfigInt(std::string key, int *pn);
	//int GetConfigInt(std::string key) {
};

#endif
