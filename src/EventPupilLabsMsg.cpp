#include "EventPupilLabsMsg.hpp"

PupilLabsTracker * EventPupilLabsMsg::s_pPupilLabsTracker = NULL;

EventPupilLabsMsg::EventPupilLabsMsg(long idEvent, long msec, long idCmd,
				     ArgMMap mmArgs, 
				     Template * pTemplate) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
						 }

EventPupilLabsMsg::~EventPupilLabsMsg() {
}

int EventPupilLabsMsg::Action() {
  g_pErr->DFI("EventPupilLabsMsg::Action", 0L, 3);
  if (EventPupilLabsMsg::s_pPupilLabsTracker) {
    //s_pPupilLabsTracker->SendEvent("{\"name\" : \"SYNCTIME\"}");
    s_pPupilLabsTracker->SendEvent("SYNCTIME");
    g_pErr->Debug(s_pPupilLabsTracker->GetPayload());
  }
  g_pErr->DFO("EventPupilLabsMsg::Action", 0L, 3);

  return Event::Action();
}

int EventPupilLabsMsg::Prepare() {
  if (!s_pPupilLabsTracker->IsRecording()) {
    s_pPupilLabsTracker->StartRecording();
  }
  
  return Event::Prepare();
}
