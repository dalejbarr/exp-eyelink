// Recordset.cpp: implementation of the Recordset class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>
#include "Recordset.hpp"
#include "pastestr.hpp"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Recordset::Recordset(ReportError * pre,
		     const char * strDB, const char *strHost, const char *strUser, 
		     const char *strPass, int nPort, int nSimUpdate) {
  m_nRows = RECORDSET_UNDEFINED;
  m_nCols = RECORDSET_UNDEFINED;
  m_pErr = pre;
  m_nSimUpdate = nSimUpdate;
  m_lCtr = 1L;
}

int Recordset::NumFields() {
  return m_nCols;
}

void Recordset::Dump() {

  int nRows = 1;
  while (RowReady()) {
    cout << nRows++ << ' ';
    for (int i = 0; i < NumFields(); i++) {
      cout << Get(i) << ' ';
    }
    cout << endl;
  
    MoveNext();
  }
}

int Recordset::BeginTransaction() {
  Exec((char *) "BEGIN;");
  return 0;
}

int Recordset::EndTransaction() {
  Exec((char *) "COMMIT;");
  return 0;
}

int Recordset::LoadFile(const char *pcFile, const char *pcTable) {
  ifstream fin;
  string strLine, strSub;
  size_t found, last, pos;
  string strSQL = "INSERT INTO ";
  strSQL.append(pcTable);
  strSQL += " VALUES (";
  string strFields, strQuery;

  BeginTransaction();

  fin.open(pcFile, ios::in);
  if (!fin.is_open()) {
    return 1;
  } else {}

  while (fin.good()) {
    last = -1;
    found = 0;
    getline(fin, strLine);
    //strFields = strSQL;
    strFields.erase();

    do {
      found = strLine.find("\t", found+1);
      if (found == string::npos) {
	pos = strLine.length()-1;
      } else {
	pos = found;
      }
    
      strSub.assign(strLine.substr(last+1, pos-last-1).c_str());
      //cout << last+1 << ' ' << int(pos) << " *" << strSub << "* " << endl;
      if (last != -1) {
	strFields += ",";
      } else {}
      strFields += strSub;

      last = pos;
    } while (found != string::npos) ;
    //strFields += ");"
    if (strFields.length() > 0) {
      strQuery = strSQL + strFields + ");";
      //cout << strQuery << endl;
      Exec(strQuery.c_str());
    } else {}
  }
  fin.close();

  EndTransaction();

  return 0;
}

int Recordset::Exec(std::string s) {
  return Exec(s.c_str());
}

int Recordset::ExecCount(std::string s) {
  return ExecCount(s.c_str());
}

int Recordset::Modify(std::string s, int nSimUpdate) {
  return Modify(s.c_str(), nSimUpdate);
}

int Recordset::Modify(const char * pc, int nSimUpdate) {
  int nResult = 0;

  m_pErr->Debug(pc);

  if ((!m_nSimUpdate) && (!nSimUpdate)) {
    nResult = Exec(pc);
  } else {}

  return nResult;
}

long Recordset::LastInsertID(const char * pcTable) {
  return m_lCtr++;
}

const char * Recordset::RandCmd() {
  return "RAND()";
}

const char * Recordset::IDType() {
  return "INTEGER";
}

const char * Recordset::NowFn() {
  return "NOW()";
}

dbData Recordset::Load(string q) {
 
  Exec(q);
  int nf = NumFields();
  dbData d;
  dbRow r;
  string s;

  while (RowReady()) {
    for (int i = 0; i < nf; i++) {
      if (Get(i)) {
	r.push_back(string(Get(i)));
      } else {
	r.push_back(string(""));
      }
    }
    d.push_back(r);
    r.clear();
    MoveNext();
  }

  return d;
}

mID2mmArgs Recordset::GetID2Args(const char * q) {
  mID2mmArgs mID2mma;
  ArgMMap mmap;
  long idLast = -1;
  long idThis = -1;

  Exec(q);
  if (RowReady()) {
    idLast = atol(Get(0));
  } else {}

  while (RowReady()) {
    idThis = atol(Get(0));
    if (idThis != idLast) {
      mID2mma[idLast] = mmap;
      mmap.clear();
      idLast = idThis;
    } else {}
    mmap.insert(pair<string, string>(Get(1), Get(2)));
    MoveNext();
  }
  mID2mma[idThis] = mmap;

  return mID2mma;
}

int Recordset::Insert(const char * pcTable, const char * pcVals) {

  Exec(InsertQueryStr(pcTable, pcVals).c_str());

  return 0;
}

long Recordset::InsertGetID(const char * pcTable, const char * pcVals) {

  Insert(pcTable, pcVals);
  return LastInsertID(pcTable);
}

string Recordset::InsertQueryStr(const char * pcTable, const char * pcVals) {
  
  string s1;
  s1.assign(pastestr::paste("sssss", "",
			    "INSERT INTO ", pcTable, " VALUES (",
			    pcVals, ")"));
  return s1;
}

int Recordset::DeleteWhere(const char * pcTable, const char * pcWhere) {
  string s1;

  s1.assign(pastestr::paste("sss", " ", "DELETE FROM ", pcTable, pcWhere));
  Exec(s1.c_str());

  return 0;
}
