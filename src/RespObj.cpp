#include "RespObj.hpp"
#include "pastestr.hpp"

RespObj::RespObj(unsigned long msec) {
  m_Msec = msec;
}

int RespObj::StoreResp(long id, Recordset * pRS) {
}

void RespObj::Print() {
  //g_pErr->Debug("shouldn't be here");
  g_pErr->Debug(pastestr::paste("ddd", " ", m_Msec, (long) m_nIndex, (long) m_nButton));
}

const char * RespObj::QueryStr(long idResp) {
  string s1;
  s1.assign(pastestr::paste("sdddd", ", ",
			    "NULL", idResp, (long) m_nIndex, (long) m_Msec, (long) m_nButton));
  return s1.c_str();
}
