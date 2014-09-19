#include "WatchDone.hpp"
#include "Template.hpp"

WatchDone::WatchDone(long idWatch, long idNext, ArgMMap mmap) : 
  Watch(idWatch, idNext) {

  g_pErr->DFI("WatchDone::WatchDone", GetID(), 3);

  m_idEvent = 0;

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  pii = mmap.equal_range("EventID");
  if (pii.first == pii.second) {
    // no event defined
  } else {
    // find particular event ID
    from_string<long>(m_idEvent, (pii.first)->second, std::dec);
    if (m_idEvent == 0) {
      g_pErr->Report("eventID for WatchDone cannot be zero");
    } else {}
  }

  g_pErr->DFO("WatchDone::WatchDone", GetID(), 3);
}

bool WatchDone::CheckCondition(SDL_Event * pEvt) {

  int nResult = false;
  long * pData = NULL;
  
  if (m_idEvent != 0) {
    pData = (long *) pEvt->user.data1;
    if (*pData==m_idEvent) {
      nResult = true;
    } else {}
    g_pErr->Debug(pastestr::paste("sdsd", " ",
				  "looking for", m_idEvent, "found", *pData));
  } else {}

  return nResult;
}
