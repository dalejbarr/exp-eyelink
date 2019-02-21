#include "RespData.hpp"
#include "pastestr.hpp"

RespData::RespData() {
}

RespData::RespData(const char * pcTable, vector<RespObj> vResp) {
  m_sTable.assign(pcTable);
  m_vResp = vResp;
}

int RespData::Store(long id, Recordset * pRS) {
  string s;
  pRS->BeginTransaction();
  for (int i = 0; i < m_vResp.size(); i++) {
    //m_vResp[i].Print();
    //g_pErr->Debug(m_vResp[i].QueryStr(id));
    s.assign(m_vResp[i].QueryStr(id).c_str());
    pRS->Insert("GamePad", s.c_str());
  }
  pRS->EndTransaction();
  return 0;
}

void RespData::AddEvent(const RespObj ro) {
  m_vResp.push_back(ro);
  //g_pErr->Debug(pastestr::paste("d", "", m_vResp.size()));
}

/*
int RespData::Size() {
  return m_vRespData.size();
}
*/

void RespData::Print() {
  for (int i = 0; i < m_vResp.size(); i++) {
    m_vResp[i].Print();
    //g_pErr->Report(pastestr::paste(""))
  }
}

void RespData::Clear() {
  m_vResp.clear();
}
