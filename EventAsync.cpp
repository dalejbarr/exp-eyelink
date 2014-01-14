#include "EventAsync.hpp"
#include "CmdID.hpp"

EventAsync::EventAsync(long id, long msec) : Event(id, msec) {
  Initialize();
}

EventAsync::EventAsync(long id, long msec, StimulusPtr pStim) : Event(id, msec, pStim) {
  Initialize();
}

EventAsync::EventAsync(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate /* = NULL*/) :
  Event(id, msec, idCmd, mmArgs, pTemplate) {
  Initialize();
}

void EventAsync::Initialize() {
  m_msUpdate = 50;
  m_msBegin = 0;
  m_msLast = 0;
  m_bRunning = false;
}

int EventAsync::Start() {
  int nResult = 0;

  if (m_bRunning) {
    // error!
  } else {
    // start the event
    g_pErr->Debug("----> EventAsync started");
    m_bRunning = true;
    m_msBegin = ClockFn();
    m_msLast = m_msBegin;
    nResult = StartProc();
  }

  return nResult;
}

int EventAsync::Stop() {
  int nResult = 0;
  SDL_UserEvent stopEvent;
  SDL_Event event;

  if (m_bRunning) {
    g_pErr->Debug("----> EventAsync stopped");
    nResult = StopProc();
    m_bRunning = false;
    // send notification
    stopEvent.type = SDL_USEREVENT;
    stopEvent.code = SBX_WATCH_DONE;
    stopEvent.data1 = (void *) &m_id;
    stopEvent.data2 = NULL;
    event.type = SDL_USEREVENT;
    event.user = stopEvent;
    SDL_PushEvent(&event);
  } else {
    // error 
  } 

  return nResult;
}

int EventAsync::Update() {
  static Uint32 msCurrent = 0;

  if (m_bRunning) {
    msCurrent = ClockFn();
    if ((msCurrent - m_msLast) >= m_msUpdate) {
      UpdateProc();
      m_msLast = msCurrent;
    } else {}
  } else {
    // not running yet; do nothing
  }

  return 0;
}

int EventAsync::StartProc() {
  return 0;
}

int EventAsync::StopProc() {
  return 0;
}

int EventAsync::UpdateProc() {
  return 0;
}
