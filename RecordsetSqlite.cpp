#include <string>
#include <iostream>
#include <cstdlib>

#include "RecordsetSqlite.hpp"
#include "pastestr.hpp"

using namespace std;

RecordsetSqlite::RecordsetSqlite(ReportError * pre,
				 const char * strDB, const char *strHost, 
				 const char *strUser, const char *strPass, int nPort, int nSimUpdate) : 
  Recordset(pre, strDB, strHost, strUser, strPass, nPort, nSimUpdate)
{
  int rc;
  m_pStmt = NULL;
  m_nLastResult = 0;

  rc = sqlite3_open_v2(strDB, &m_pData, SQLITE_OPEN_READWRITE, NULL);
  if (rc != SQLITE_OK) {
    m_pErr->Report(pastestr::paste("ss", NULL, "Could not open file ", strDB));

    sqlite3_close(m_pData);
    m_pData = NULL;
  } else {}
}

RecordsetSqlite::~RecordsetSqlite() {

  //printf("In RecordsetSqlite::~RecordsetSqlite()\n");

  int rc = 0;
  
  if (m_pData) {
    if (m_pStmt) {
      sqlite3_finalize(m_pStmt);
    } else {}

    int rc = sqlite3_close(m_pData);
    if (rc == SQLITE_BUSY) {
      m_pErr->Report("sqlite_busy: maybe a memory leak?\n");
    } else {
      if (rc != SQLITE_OK) {
	m_pErr->Report(pastestr::paste("sd", "", "sqlite error code ", rc));
      } else {}
    }
  } else {}
}

int RecordsetSqlite::Exec(const char *pc)
{
  int nResult = 0;

  if (m_pStmt) {
    sqlite3_finalize(m_pStmt);
  } else {}

  m_pErr->Debug(pastestr::paste("ss", "", "sqlite: ", pc));

  nResult = sqlite3_prepare(m_pData,
			    pc, -1, &m_pStmt, NULL);

  if (nResult != SQLITE_OK) {
    m_pErr->Report(pastestr::paste("sd", "", "Error in Recordset::Exec.\nSQLite Err Code ", nResult));
    
    return 1;
  } else {
    m_nCols = sqlite3_column_count(m_pStmt);
    //printf("%d **\n", m_nCol);
    nResult = MoveNext();
  }

  return 0;
}

int RecordsetSqlite::MoveNext()
{
  int nResult = NULL;

  m_nLastResult = sqlite3_step(m_pStmt);
  if (m_nLastResult == SQLITE_ROW) {
    nResult = RECORDSET_ROW;
  } else {
    if ((m_nLastResult != SQLITE_OK) && (m_nLastResult != SQLITE_DONE)) {
      m_pErr->Report(pastestr::paste("sd", "", "SQLITE error code ", m_nLastResult));
    } else {}
  }

  return nResult;
}

const char * RecordsetSqlite::Get(int nCol, const char * pcNullReplace) {
  const char * pc = (const char *) sqlite3_column_text(m_pStmt, nCol);
  if (pc == NULL) {
    return pcNullReplace;
  } else {
    return pc;
  }
}

int RecordsetSqlite::RowReady() {
  if (m_nLastResult == SQLITE_ROW) {
    return 1;
  } else {
    return 0;
  }
}

int RecordsetSqlite::Reset() {
  if (m_pStmt) {
    sqlite3_reset(m_pStmt);
    MoveNext();
  } else {
    //MessageBox(NULL, "problem in Recordset::MoveFirst", "Error", MB_OK);	
  }

  return 0;
}


/*
int RecordsetSqlite::ExecCount(const char *pc) {
  string strPC;
  strPC.assign(pc);
  size_t pos, last;
  string strSelect, strFrom, strQuery;
  int nResult = 0;

  int nFound = 0;
  last = -1;

  while (!nFound) {

    pos = strPC.find("from", last+1);
    if (pos == string::npos) {
      pos = strPC.find("FROM", last+1);
    } else {}

    if (pos == string::npos) {
      cout << "error!  query '" << strPC <<  "' \n didn't have FROM statement" << endl;
      break;
    } else {
      if ( ((strPC[pos-1] == '\t') || (strPC[pos-1] == ' ') || (strPC[pos-1] == '\n')) &&
	   ((strPC[pos+4] == '\t') || (strPC[pos+4] == ' ') || (strPC[pos+4] == '\n')) ) {
	nFound = 1;
	break;
      } else {
	last = pos;
      }
    }

  }

  if (nFound) {
    strSelect = strPC.substr(0, pos);
    strFrom = strPC.substr(pos, strPC.length() - pos);
    strQuery = "SELECT COUNT(*) ";
    strQuery.append(strFrom);
    cout << strQuery << endl;
    Exec(strQuery.c_str());
    m_nRows = sqlite3_column_int(m_pStmt, 0);
    cout << "sqlite ROWS " << m_nRows << endl;
    Exec(pc);

    //cout << "found it " << int(pos) << " *" << strPC[pos-1] << "* *" << strPC[pos+4] << "*" << endl;
    //cout << strQuery << ' ' << m_nRows << endl;
    //cout << strFrom << endl;
    nResult = m_nRows;
  } else {}

  return nResult;
}
*/

int RecordsetSqlite::ExecCount(const char *pc) {
  string strPC;
  strPC.assign(pc);

  Exec(pc);
  long i = 0;
  while (RowReady()) {
    MoveNext();
    i++;
    //    cout << "count = " << i << endl;
  }
  m_nRows = i;

  //cout << "sqlite ROWS " << m_nRows << endl;
  Reset();

  return 0;
}


long RecordsetSqlite::LastInsertID(const char * pcTable) {
  
  string strQ = "SELECT seq FROM SQLITE_SEQUENCE WHERE name='";
  string strT;
  strT.assign(pcTable);
  strQ += strT + "'";

  //cout << strQ << endl;
  //cout << m_nSimUpdate << endl;
  if (m_nSimUpdate) {
    return Recordset::LastInsertID(NULL);
  } else {
    Exec(strQ.c_str());
    return (atol(Get(0)));
  }
  
  return 0;
}

const char * RecordsetSqlite::RandCmd() {
  return "RANDOM()";
}

const char * RecordsetSqlite::NowFn() {
  return "datetime('now')";
}

bool RecordsetSqlite::TableExists(const char * pcTable) {
  string strT;
  strT.assign(pcTable);
  string strQ = "SELECT COUNT(*) FROM sqlite_master WHERE type='table' and name='" + strT + "'";
  Exec(strQ.c_str());
  int nResult = atoi(Get(0));
  return(nResult>0);
}
