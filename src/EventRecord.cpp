#include "EventRecord.hpp"
#include "Mouse_SDL.hpp"

// #ifndef WIN32
// #include "PASoundIn.hpp"
// #endif

EventRecord::EventRecord(long idEvent, long msec, long idCmd, 
			 ArgMMap mmArgs, Template * pTemplate, InputDevPtr pDev) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventRecord::EventRecord", idEvent, 3);

  m_vpDev.push_back(pDev);

  ParseArgs(mmArgs);

  g_pErr->DFO("EventRecord::EventRecord", idEvent, 3);
}

EventRecord::EventRecord(long idEvent, long msec, long idCmd, 
			 ArgMMap mmArgs, Template * pTemplate, vector<InputDevPtr> vpDev) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventRecord::EventRecord", idEvent, 3);
  //m_pDev = pDev;
  m_vpDev = vpDev;

  ParseArgs(mmArgs);

  g_pErr->DFO("EventRecord::EventRecord", idEvent, 3);
}


void EventRecord::ParseArgs(ArgMMap mmArgs) {
  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  pii = mmArgs.equal_range("Draw");
  if (pii.first != pii.second) {
    Mouse_SDL * pMouse = (Mouse_SDL *) m_vpDev[0].get();
    pMouse->SetDraw(atoi((pii.first)->second.c_str()));
  } else {}

  pii = mmArgs.equal_range("xyHome");
  if (pii.first != pii.second) {
    Mouse_SDL * pMouse = (Mouse_SDL *) m_vpDev[0].get();
    pMouse->SetHome((pii.first)->second.c_str());
  } else {}

  pii = mmArgs.equal_range("xyLimits");
  if (pii.first != pii.second) {
    Mouse_SDL * pMouse = (Mouse_SDL *) m_vpDev[0].get();
    pMouse->SetLimits((pii.first)->second.c_str());
  } else {}

  pii = mmArgs.equal_range("Soundfile");
  if (pii.first != pii.second) {
    m_sResource = pii.first->second;
    g_pErr->Debug(pastestr::paste("ss", " ", "output file set to ", m_sResource.c_str()));
  } else {
    m_sResource.assign("");
  }
}


int EventRecord::Prepare() {

  g_pErr->DFI("EventRecord::Prepare", m_id, 3);
  
  if (m_sResource != "") {
    m_vpDev[0]->SetOutputFile(m_sResource.c_str());
  } else {}
  /*
  PASoundIn * pSound = (PASoundIn *) m_pDev.get();
  pSound->SetResource( pii.first->second.c_str() );
  */

  //m_pDev->Prepare();
  for (int i = 0; i < m_vpDev.size(); i++) {
    m_vpDev[i]->SetRecMode(true);
  }

  g_pErr->DFO("EventRecord::Prepare", m_id, 3);

  return Event::Prepare();
}

int EventRecord::Action() {
  for (int i = 0; i < m_vpDev.size(); i++) {
    m_vpDev[i]->Start();
  }
  return Event::Action();
}

int EventRecord::Cleanup() {
  for (int i = 0; i < m_vpDev.size(); i++) {
    m_vpDev[i]->Stop();
  }

  return Event::Cleanup();
}
