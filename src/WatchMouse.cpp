#include "WatchMouse.hpp"
#include "global.hpp"
#include "pastestr.hpp"
#include "SDL.h"

Mouse_SDL * WatchMouse::s_pMouse = NULL;

WatchMouse::WatchMouse(long id, long idNext, ArgMMap mmap, StimulusPtrMMap * pmmapAOI) : Watch(id, idNext) {
  g_pErr->DFI("WatchMouse::WatchMouse", id, 4);
  m_nRegion = WATCHMOUSE_NOREGION;
  m_pmmapAOI = pmmapAOI;
  pair<ArgIter, ArgIter> pii = mmap.equal_range("Button");
  ArgIter ii;
  m_pSelectedAOI.reset();
  m_strSelectedAOIType = "";
  m_x1 = m_y1 = m_x2 = m_y2 = 0;

  pii = mmap.equal_range("Button");
  if (pii.first == pii.second) {
    g_pErr->Debug("watching for ANYBUTTON down");
    m_nButton = SDL_BUTTON_LEFT;
  } else {
    ii = pii.first;
    if ((*ii).second == "left") {
      m_nButton = SDL_BUTTON_LEFT;
    } else {}
    g_pErr->Debug(pastestr::paste("sd", " ", "watching for", ((long) m_nButton)));
  }

  pii = mmap.equal_range("State");
  if (pii.first == pii.second) {
    m_nState = SDL_PRESSED;
  } else {
    ii = pii.first;
    if ((*ii).second=="up") {
      m_nState = SDL_RELEASED;
    } else {
      if ((*ii).second=="down") {
	m_nState = SDL_PRESSED;
      } else {
	g_pErr->Report(pastestr::paste("sd", "", "error parsing argument 'State' for WatchID ", m_id));
      }
    }
    g_pErr->Debug(pastestr::paste("dsd", "", GetID(), " watch is looking for state ", m_nState));
  }

  pii = mmap.equal_range("AOI");
  if (pii.first != pii.second) {
    m_nRegion = WATCHMOUSE_AOI;
    for (ii = pii.first; ii != pii.second; ii++) {
      m_vAOI.push_back((*ii).second);
    }
  } else {}

  pii = mmap.equal_range("XY");
  if (pii.first != pii.second) {
    g_pErr->Debug("here");
    string s1( (*pii.first).second );
    //g_pErr->Debug(s1.c_str());
    istringstream iss(s1);
    iss >> m_x1; iss >> m_y1; iss >> m_x2; iss >> m_y2;
    m_nRegion = WATCHMOUSE_XY;
    g_pErr->Debug(pastestr::paste("sdddd"," ",
				  "watching for click in region",
				  m_x1, m_y1, m_x2, m_y2));
  } else {}

  g_pErr->DFO("WatchMouse::WatchMouse", m_id, 4);
}



bool WatchMouse::CheckCondition(SDL_Event * pEvt) {
  g_pErr->DFI("WatchMouse::CheckCondition", m_id, 4);

  unsigned int X=0;
  unsigned int Y=0;

  SDL_MouseButtonEvent * pMEvt = (SDL_MouseButtonEvent *) pEvt;
  int nResult = false;
  pair<StimulusPtrMMapIter, StimulusPtrMMapIter> pii;
  StimulusPtrMMap::iterator ii;
  m_pSelectedAOI.reset();

  if (s_pMouse) {
    X = s_pMouse->GetX();
    Y = s_pMouse->GetY();
  } else {
    X = pMEvt->x;
    Y = pMEvt->y;
  }

  if (m_nButton == EXP_ANYMOUSEBUTTON) {
    if (m_nState == pMEvt->state) {
      nResult = true;
    } else {}
  } else {
    g_pErr->Debug(pastestr::paste("dsdd", " ", GetID(), "watch", m_nState, pMEvt->state));
    if ( (m_nState == pMEvt->state) && (m_nButton == pMEvt->button) ) {
      nResult = true;
    } else {}
  }

  g_pErr->Debug(pastestr::paste("dsd", " ", GetID(), "result", nResult));

  if (nResult) {

    // now see if we need to check for a region
    switch (m_nRegion) {
    case WATCHMOUSE_AOI :
      nResult = false;
      for (int i = 0; i < m_vAOI.size(); i++) {
	if (nResult) {
	  break;
	} else {}
	pii = (*m_pmmapAOI).equal_range(m_vAOI[i]);
	for (ii = pii.first; ii != pii.second; ii++) {
	  if (WatchMouse::s_pMouse) {
	  } else {}
	  if (((StimulusImg *) (*ii).second.get())->InAOI(X, Y)) {
	    m_strSelectedAOIType = (*ii).first;
	    m_pSelectedAOI = (*ii).second;
	    nResult = true;
	    break;
	  } else {}
	}
      }
      break;
    case WATCHMOUSE_XY :
      // pMEvt->x, Y
      nResult = false;
      g_pErr->Debug(pastestr::paste("sdddd"," ",
				    "testing for mouse in region",
																		(long) m_x1, (long) m_y1, (long) m_x2, (long) m_y2));
      if ( ((X >= m_x1) && (X <= m_x2)) &&
	   ((Y >= m_y1) && (Y <= m_y2)) ) {
	nResult = true;
	g_pErr->Debug("success");
      } else {
	g_pErr->Debug(pastestr::paste("sdd", " ", "mouse at ", X, Y));
      }
      break;
    case WATCHMOUSE_NOREGION :
      nResult = true;
      break;
    }

    if (m_pSelectedAOI) {
      g_pErr->Debug(pastestr::paste("sssdsds", "", "selected AOI type ", m_strSelectedAOIType.c_str(),
				    " at (", (long) ((StimulusImg *) m_pSelectedAOI.get())->m_x1, ", ", 
				    (long) ((StimulusImg *) m_pSelectedAOI.get())->m_y1, ")"));
    } else {}
  } else {}

  g_pErr->DFO("WatchMouse::CheckCondition", m_id, 4);

  return nResult;
}

int WatchMouse::Prepare() {
  m_pSelectedAOI.reset();
  m_strSelectedAOIType = "";

  Watch::Prepare();
  return 0;
}

int WatchMouse::Finish() {
  Watch::Finish();
  return 0;
}

void WatchMouse::ResetSelection() {
  m_pSelectedAOI.reset();
  m_strSelectedAOIType = "";
}
