#ifndef SBX_RESPDATA_INCLUDED
#define SBX_RESPDATA_INCLUDED

#include "global.hpp"
#include "RespObj.hpp"

class RespData {
protected:
  string m_sTable;
public:
  vector<RespObj> m_vResp;
  RespData();
  RespData(const char * pc, vector<RespObj> vResp);
  int Store(long id, Recordset * pRS);
  void AddEvent(const RespObj ro);
  int Size() const { return m_vResp.size(); };
  void Print();
  void Clear();
};

#endif
