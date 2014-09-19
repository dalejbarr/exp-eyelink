#include "GPEvent.hpp"
#include "pastestr.hpp"

GPEvent::GPEvent(int ix, int nButton, unsigned long msec) : RespObj(msec) {
  m_nIndex = ix;
  m_nButton = nButton;
}

void GPEvent::Print() {
  g_pErr->Debug(pastestr::paste("ddd", " ", m_Msec, (long) m_nIndex, (long) m_nButton));
}
