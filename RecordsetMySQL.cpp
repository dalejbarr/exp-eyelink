#include <string>
#include <iostream>
#include "RecordsetMySQL.hpp"
#include "pastestr.hpp"

using namespace std;

RecordsetMySQL::RecordsetMySQL(ReportError * pre,
			       const char * strDB, const char *strHost, 
			       const char *strUser, const char *strPass, int nPort,
			       int nSimUpdate) : 
  Recordset(pre, strDB, strHost, strUser, strPass, nPort, nSimUpdate)
{
  m_psqlRes = NULL;
  m_sqlRow = NULL;

  m_pData = mysql_init((MYSQL *) 0);

  if (!mysql_real_connect(m_pData, strHost, strUser, strPass, strDB, 
			  nPort, NULL, 0)) 
    {
      m_pErr->Report(pastestr::paste("sss", "", "Could not connect to the experiment database ", strDB, "."));
      mysql_close(m_pData);
      m_pData = NULL;
    } else {}
}

RecordsetMySQL::~RecordsetMySQL() {

  //printf("In RecordsetMySQL::~RecordsetMySQL()\n");

  int rc = 0;

  if (m_pData) {
    Free();
  } else {}

}

int RecordsetMySQL::Exec(const char *pc)
{
  int nResult = 0;

  if (m_psqlRes) {
    Free();
  } else {}

  if (!m_pData) {
    m_pErr->Report("error: no connection");
    return 1;
  }

  m_pErr->Debug(pc);

  if (mysql_query(m_pData, pc))
    return 1;

  m_psqlRes = mysql_store_result(m_pData);
  
  if (m_psqlRes) {
    m_nRows = (long) mysql_num_rows(m_psqlRes);
    m_nCols = mysql_num_fields(m_psqlRes);
    MoveNext();
  } else {}

  return 0;
}

int RecordsetMySQL::MoveNext()
{
  if (!m_psqlRes) {
    return NULL;
  } else {}

  m_sqlRow = mysql_fetch_row(m_psqlRes);

  return 0;
}

const char * RecordsetMySQL::Get(int nCol, const char * pcNullReplace) {
  const char * pc;

  if (m_sqlRow) {
    pc = m_sqlRow[nCol];
    if (pc == NULL) {
      return pcNullReplace;
    } else {
      return m_sqlRow[nCol];
    }
  } else {
    return NULL;
  }
}

int RecordsetMySQL::RowReady() {
  if (m_sqlRow) {
    return 1;
  } else {
    return 0;
  }
}

int RecordsetMySQL::Reset() {
  if (!m_psqlRes)
    return NULL;

  mysql_data_seek(m_psqlRes, 0);
  MoveNext();

  return 0;
}


int RecordsetMySQL::LoadFile(const char *pcFile, const char *pcTable) {
  string strQuery = "LOAD DATA LOCAL INFILE '";
  strQuery.append(pcFile);
  strQuery += "' INTO TABLE ";
  strQuery.append(pcTable);
  
  //cout << strQuery << endl;
  Exec(strQuery.c_str());

  return 0;
}

int RecordsetMySQL::Free() {
    mysql_free_result(m_psqlRes);
    m_psqlRes = NULL;
    m_sqlRow = NULL;

	return 0;
}

int RecordsetMySQL::ExecCount(const char *pc) {
  Exec(pc);
  return m_nRows;
}

long RecordsetMySQL::LastInsertID(const char * pcTable) {
  if (m_nSimUpdate) {
    return Recordset::LastInsertID(NULL);
  } else {
    return (long) mysql_insert_id(m_pData);
  }
  
  return 0L;
}

bool RecordsetMySQL::TableExists(const char * pcTable) {
  string strT;
  strT.assign(pcTable);
  string strQ = "SHOW TABLES LIKE '" + strT + "'";
  int nResult = ExecCount(strQ.c_str());
  return(nResult>0);
}
