#include "EventSTRecord.hpp"
#include "ScrollTrackGP_SDL.hpp"

EventSTRecord::EventSTRecord(long idEvent, long msec, long idCmd, 
			 ArgMMap mmArgs, Template * pTemplate, InputDevPtr pDev) :
  EventRecord(idEvent, msec, idCmd, mmArgs, pTemplate, pDev) {
  g_pErr->DFI("EventSTRecord::EventSTRecord", idEvent, 3);
  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("Resource");
  ArgIter ii;

  ii = pii.first;
  ((ScrollTrackGP_SDL *) pDev.get())->SetResource((*ii).second);

  //m_vpDev.push_back();  

  g_pErr->DFO("EventSTRecord::EventSTRecord", idEvent, 3);
}
