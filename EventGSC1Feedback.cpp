#include "EventGSC1Feedback.hpp"
#include "StimulusWav.hpp"
#include "CmdID.hpp"

EventGSC1Feedback::EventGSC1Feedback(long id, long msec, 
				     long idCmd, ArgMMap mmArgs, Template * pTemplate,
				     EventGSC1DrawGrid * pGrid) :
  Event(id, msec, idCmd, mmArgs, pTemplate) {
  m_pGrid = pGrid;
  g_pErr->DFI("EventGSC1Feedback::EventGSC1Feedback", 0L, 3);
  g_pErr->DFO("EventGSC1Feedback::EventGSC1Feedback", 0L, 3);
}

int EventGSC1Feedback::Prepare() {
  Event::Prepare();
  EventPtr pEv = m_pTemplate->FindEventByCmdID(SBX_EVENT_GSC1DRAWGRID);  
  m_pGrid = (EventGSC1DrawGrid *) pEv.get();

  return 0;
}

int EventGSC1Feedback::Action() {

  m_pGrid->GiveFeedback();

  //return Event::Action();
  return 0;
}
