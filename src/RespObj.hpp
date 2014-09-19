#ifndef SBX_RESPOBJ_INCLUDED
#define SBX_RESPOBJ_INCLUDED

#include "global.hpp"

class RespObj {
public:
  unsigned long m_Msec;
  int m_nIndex;
  int m_nButton;  
  RespObj(unsigned long msec = 0);
  virtual int StoreResp(long id, Recordset * pRS);
  virtual void Print();
  const char * QueryStr(long idResp);
};

#endif
