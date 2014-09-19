#include "Recordset.hpp"

#ifdef WIN32
#include <winsock.h>
#endif

#include <mysql.h>

class RecordsetMySQL : public Recordset 
{
 protected:
  MYSQL * m_pData;
  MYSQL_RES * m_psqlRes;
  MYSQL_ROW m_sqlRow;

 public:
  RecordsetMySQL(ReportError * pre,
		 const char * strDB, 
		 const char * strHost = NULL, 
		 const char * strUser = NULL, 
		 const char * strPass = NULL,
		 int nPort = MYSQL_PORT,
		 int nSimUpdate = 0);

  ~RecordsetMySQL();

  virtual int Exec(const char *);
  virtual int MoveNext();
  virtual const char * Get(int nCol, const char * pcNullReplace = NULL);
  virtual int RowReady();
  virtual int Reset();
  virtual int LoadFile(const char *pcFile, const char *pcTable);
  virtual int ExecCount(const char *);
  virtual long LastInsertID(const char * pcTable = NULL);
  virtual bool TableExists(const char * pcTable);
  int Free();
};
