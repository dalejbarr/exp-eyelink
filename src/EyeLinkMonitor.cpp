#include "global.hpp"
#include "EyeLinkMonitor.hpp"
#include "CmdID.hpp"
#include "pastestr.hpp"

long EyeLinkMonitor::s_idLastWatch = 0;

void EyeLinkMonitor::Prepare() {
  // this is run at the start of the trial;
  // don't think we really need to do anything
  g_pErr->DFI("EyeLinkMonitor::Prepare", "", 2);
  g_pErr->DFO("EyeLinkMonitor::Prepare", "", 2);
}

void EyeLinkMonitor::Start() {
  g_pErr->DFI("EyeLinkMonitor::Start", "", 2);
  g_pErr->DFO("EyeLinkMonitor::Start", "", 2);
}

void EyeLinkMonitor::Stop() {
  g_pErr->DFI("EyeLinkMonitor::Stop", "", 2);
  g_pErr->DFO("EyeLinkMonitor::Stop", "", 2);
}

void EyeLinkMonitor::StartMonitoring() {
  // only start if we're not already monitoring
  if (!m_abContinue) {
    m_abContinue = true;
    if (!m_pThread) {
      g_pErr->Debug("starting eyelink monitor thread...");
      m_pThread = SDL_CreateThread(EyeLinkMonitor::main, this);
    }
  }
}

void EyeLinkMonitor::StopMonitoring() {
  
  if (m_abContinue) {
    g_pErr->Debug("stopping eyelink monitor...");
    m_abContinue = false;
    int nStatus = 0;
    if (m_pThread) {
      SDL_WaitThread(m_pThread, &nStatus);
    }
  }

  m_pThread = NULL;
}

EyeLinkMonitor::EyeLinkMonitor(unsigned long idDev, int nIndex /*= 0*/) :
  InputDev(idDev, nIndex) {
  m_pThread = NULL;
  m_abContinue = false;
}

EyeLinkMonitor::~EyeLinkMonitor() {
  if (m_pThread) {
    SDL_KillThread(m_pThread);
    m_abContinue = false;
    m_pThread = NULL;
  }
}

int EyeLinkMonitor::main(void *pVoid) {
  EyeLinkMonitor * pThis;

  pThis = (EyeLinkMonitor *) pVoid;
  return pThis->Main();
}

int EyeLinkMonitor::Main() {
  // g_pErr->Debug("entering eyelink monitor loop...");
  CircularStack<xy_coord> stack(SBX_EYELINK_SAMPLE_THRESH);
  xy_coord xy(0, 0);
  int nTicks = 0;
  std::size_t i = 0, j = 0;
  bbox bb;

  g_pErr->Debug(pastestr::paste("sds", " ",
				"we have ", (long) m_bbpairs.size(),
				"AOIs to watch"));
  
  while (m_abContinue) {
    // do stuff
    SDL_GetMouseState(&xy.first, &xy.second);
    stack.push(xy);
    nTicks++;

    if (stack.is_full()) {
      // check each AOI to see if threshhold is reached
      for (i = 0; i < m_bbpairs.size(); i++) {
	bb = m_bbpairs[i].first;

	for (j = 0; j < stack.get_nElements(); j++) {
	  xy = stack.get_at(j);
	  if (!IsPointInAOI(xy.first, xy.second, bb)) break;
	}
	if (j == stack.get_size()) { // condition satisfied
	  EyeLinkMonitor::s_idLastWatch = m_bbpairs[i].second;
	  Signal(m_bbpairs[i].second);
	  m_abContinue = false;
	}
      }
    }
    
    SDL_Delay(SBX_EYELINK_SAMPLE_INTERVAL_MS);
  }

  m_abContinue = false;
  
  return 0;
}

int EyeLinkMonitor::Signal(long id) {
  static SDL_Event event;
  static SDL_UserEvent userevent;
  
  userevent.type = SDL_USEREVENT;
  userevent.code = SBX_WATCH_EYELINKAOI;
  userevent.data1 = (void *) &s_idLastWatch;
  userevent.data2 = NULL;
  event.type = SDL_USEREVENT;
  event.user = userevent;
  SDL_PushEvent(&event);
  
  return 0;
}

void EyeLinkMonitor::RegisterAOI(int x1, int y1, int x2, int y2, long id) {
  xy_coord xy_from(x1, y1);
  xy_coord xy_to(x2, y2);
  bbox my_bbox(xy_from, xy_to);
  bbpair my_bbpair(my_bbox, id);
  m_bbpairs.emplace_back(my_bbpair);
}

bool EyeLinkMonitor::IsPointInAOI(int x, int y, bbox bb) {
  bool bResult = false;

  bResult = (x >= bb.first.first) && // x >= x1
    (x <= bb.second.first) &&     // x <= x2
    (y >= bb.first.second) &&     // y >= y1
    (y <= bb.second.second);      // y <= y2

  return bResult;
}

void EyeLinkMonitor::ClearAOIs() {
  m_bbpairs.clear();
}
