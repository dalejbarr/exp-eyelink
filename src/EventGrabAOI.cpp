#include "EventGrabAOI.hpp"
#include "State.hpp"
#include "global.hpp"
#include "Template.hpp"
#include "pastestr.hpp"
#include "WatchMouse.hpp"

#include <sstream>
using std::ostringstream;

EventGrabAOI::EventGrabAOI(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate) : 
  Event(id, msec, idCmd, mmArgs, pTemplate) {

  g_pErr->DFI("EventGrabAOI::Action", id, 4);

  m_msLastDraw = 0;
  m_nOffX = 0; m_nOffY = 0;

  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("AOI");
  ArgIter ii;

  if (pii.first == pii.second) {
    g_pErr->Report("Missing argument for GRAB_AOI");
  } else {}

  for (ii = pii.first; ii != pii.second; ii++) {
    m_ppStim = m_pTemplate->AOIArg((*ii).second);
  }
  // TO DO: pii = m_mmArgs.equal_range("x");
  // TO DO: pii = m_mmArgs.equal_range("y");
  g_pErr->DFO("EventGrabAOI::Action", id, 4);
}

int EventGrabAOI::Action() {
  g_pErr->DFI("EventGrabAOI::Action", "", 2);

  StimulusImg * pStim = (StimulusImg *) (*m_ppStim).get();
  
  if (pStim) {
    m_nOffX = pStim->m_CurX.Get() - State::s_nMouseCurX.Get();
    m_nOffY = pStim->m_CurY.Get() - State::s_nMouseCurY.Get();
  } else {
    g_pErr->Debug("stim was not attached");
    m_nOffX = 0; m_nOffY = 0;
  }

  g_pErr->Debug(pastestr::paste("dddddd", " - ", 
				State::s_nMouseCurX.Get(), State::s_nMouseCurY.Get(),
				pStim->m_CurX.Get(), pStim->m_CurY.Get(),
				m_nOffX, m_nOffY));

  m_msLastDraw = ClockFn();
  //g_pErr->DFI("EventGrabAOI::Action()", m_id, 4);
  //g_pErr->DFO("EventGrabAOI::Action()", m_id, 4);
  g_pErr->DFO("EventGrabAOI::Action", "", 2);

  return Event::Action();
}

int EventGrabAOI::Update(int x, int y) {

  StimulusImg * pStim = (StimulusImg *) (*m_ppStim).get();
  if ((ClockFn() - m_msLastDraw) > 33) {
    if (m_pTemplate) {

      if (pStim) {
	pStim->m_CurX.Set(State::s_nMouseCurX.Get() + m_nOffX);
	pStim->m_CurY.Set(State::s_nMouseCurY.Get() + m_nOffY);
	//g_pErr->Debug(pastestr::paste("dd", " ", (long) pStim->m_CurX, (long) pStim->m_CurY));
      } else {
	g_pErr->Report("pStim invalid!");
      }
      m_pTemplate->Redraw(0);
      m_msLastDraw = ClockFn();
    } else {
      g_pErr->Report("programming error: m_pTemplate is null");
    }
    //g_pErr->Debug("draw!");
  } else {
    //g_pErr->Debug("no draw");
  }
  //g_pErr->DFO("EventGrabAOI::Update()", m_id, 4);  
  return 0;
}
