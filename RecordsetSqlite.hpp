#include "Recordset.hpp"
#include "sqlite3.h"

class RecordsetSqlite : public Recordset 
{
 protected:
  sqlite3 * m_pData;
  sqlite3_stmt * m_pStmt;
  int m_nLastResult;

 public:
  RecordsetSqlite(ReportError * pre,
		  const char * strDB, 
		  const char * strHost = NULL, 
		  const char * strUser = NULL, 
		  const char * strPass = NULL,
		  int nPort = 3306,
		  int nSimUpdate = 0);

  ~RecordsetSqlite();

  virtual int Exec(const char *);
  virtual int MoveNext();
  virtual const char * Get(int nCol, const char * pcNullReplace = NULL);
  virtual int RowReady();
  virtual int Reset();
  virtual int ExecCount(const char *);
  virtual long LastInsertID(const char * pcTable = NULL);
  virtual const char * RandCmd();
  virtual const char * NowFn();
  virtual bool TableExists(const char * pcTable);
};
