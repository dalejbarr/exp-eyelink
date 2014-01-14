#include "RespObjGSC1.hpp"
#include "pastestr.hpp"

RespObjGSC1::RespObjGSC1() {
}

int RespObjGSC1::StoreResp(Recordset * pRS) {
  string s1;

  g_pErr->DFI("RespObjGSC1::StoreResp", 0L, 3);

  // TODO : store who's who?
  // or just allow it to be inferred by response ID.

  // store locations of bombs
  pRS->BeginTransaction();
  for (int i = 0; i < m_vBombLocs.size(); i++) {
    s1.assign(pastestr::paste("dddd", ", ", m_idResp, (long) (i+1),
			      m_vBombLocs[i].m_x, m_vBombLocs[i].m_y));
    pRS->Insert("GridBombs", s1.c_str());
  }
  pRS->EndTransaction();

  // store location of flags
  pRS->BeginTransaction();
  for (int i = 0; i < m_vFlagLocs.size(); i++) {
    s1.assign(pastestr::paste("dddd", ", ", m_idResp, (long) (i+1),
			      m_vFlagLocs[i].m_x, m_vFlagLocs[i].m_y));
    pRS->Insert("GridFlags", s1.c_str());
  }
  pRS->EndTransaction();

  g_pErr->DFO("RespObjGSC1::StoreResp", 0L, 3);
  return 0;
}
