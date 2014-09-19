#include "RespData.hpp"

RespData::RespData() {
}

RespData::RespData(const char * pcTable, vector<RespObj> vResp) {
  m_sTable.assign(pcTable);
  m_vResp = vResp;
}

int RespData::Store(long id, Recordset * pRS) {
  pRS->BeginTransaction();
  for (int i = 0; i < m_vResp.size(); i++) {
    pRS->Insert("GamePad", m_vResp[i].QueryStr(id));
    //g_pErr->Debug(m_vResp[i].QueryStr(id));
  }
  pRS->EndTransaction();
  return 0;
}

void RespData::AddEvent(const RespObj ro) {
  m_vResp.push_back(ro);
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
