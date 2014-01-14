// Recordset.h: interface for the Recordset class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RECORDSET_H__2A7BB2CB_6F19_4C0D_9F07_CA1DE0C6494B__INCLUDED_)
#define AFX_RECORDSET_H__2A7BB2CB_6F19_4C0D_9F07_CA1DE0C6494B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define RECORDSET_UNDEFINED -1
#define RECORDSET_ROW 1

#include <string>
using std::string;

#include <sstream>
using std::istringstream;

#include "ReportError.hpp"

#include <vector>
using std::vector;

typedef vector<string> dbRow;
typedef vector<dbRow> dbData;

#include <map>
typedef std::multimap<std::string, std::string> ArgMMap;
typedef std::multimap<std::string, std::string>::iterator ArgIter;
typedef std::map<long, ArgMMap> mID2mmArgs;

template <class T>
bool from_string(T& t,
		 const std::string& s,
		 std::ios_base& (*f)(std::ios_base&)) {
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}

class Recordset  
{
 protected:
  long m_nRows;
  int m_nCols;
  long m_lCtr;
  ReportError * m_pErr;

 public:
  int m_nSimUpdate;

 public:
  Recordset(ReportError * pre,
	    const char * strDB, 
	    const char * strHost = NULL, 
	    const char * strUser = NULL, 
	    const char * strPass = NULL,
	    int nPort = 3306, int nSimUpdate = 0);

  void Dump();

  int Exec(string);
  int ExecCount(string);
  dbData Load(string);
  mID2mmArgs GetID2Args(const char * q);

  virtual int Modify(string, int nSimUpdate = 0);
  virtual int Modify(const char *, int nSimUpdate = 0);

  virtual int Exec(const char *) = 0;
  virtual int MoveNext() = 0;
  virtual const char * Get(int nCol, const char * pcNullReplace = NULL) = 0;
  
  virtual int NumFields();
  virtual int NumRows() { return m_nRows; };
  virtual int RowReady() = 0;
  virtual int Reset() = 0;
  virtual int LoadFile(const char *pcFile, const char *pcTable);
  virtual int BeginTransaction();
  virtual int EndTransaction();
  virtual int ExecCount(const char *) = 0;
  virtual long LastInsertID(const char * pcTable = NULL);
  virtual const char * RandCmd();
  virtual const char * NowFn();
  virtual string InsertQueryStr(const char * pcTable, const char * pcVals);
  virtual int Insert(const char * pcTable, const char * pcVals);
  virtual long InsertGetID(const char * pcTable, const char * pcVals);
  virtual int DeleteWhere(const char * pcTable, const char * pcWhere);
  virtual bool TableExists(const char * pcTable) = 0;
  virtual const char * IDType();
};

#endif // !defined(AFX_RECORDSET_H__2A7BB2CB_6F19_4C0D_9F07_CA1DE0C6494B__INCLUDED_)
