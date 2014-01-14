#include "EventAsyncCtrl.hpp"
#include "CmdID.hpp"
//#include "EventGSC1Progress.hpp"
#include "Template.hpp"

EventAsyncCtrl::EventAsyncCtrl(long id, long msec, long idCmd, ArgMMap mmArgs, 
			       Template * pTemplate /*= NULL*/, bool bStart /* = true*/) :
  Event(id, msec, idCmd, mmArgs, pTemplate) {
  m_bStart = bStart;
  m_pEA.reset();

  pair<ArgIter, ArgIter> pii;

  if (idCmd == SBX_EVENT_ASYNCSTART) {
    // start event; need to define it
    pii = mmArgs.equal_range("EventType");
    if (pii.first == pii.second) {
      g_pErr->Report("EventType is obligatory argument for Event Async start");
    } else {
      /*
      if ((pii.first)->second == "AnimationGSC1Progress") {
	g_pErr->Debug("GSC1 progress meter: here");
	m_pEA = EventPtr(new EventGSC1Progress(id, msec, idCmd, mmArgs, pTemplate));
      }
      */
    }
    if (m_pEA.get()) {
      if (pTemplate) {
	pTemplate->AddEventAsync(m_pEA);
      } else {
	g_pErr->Report("EventAsyncCtrl: template undefined!");
      }
    } else {}
  } else {
    // stop event
    pii = mmArgs.equal_range("EventID");
    if (pii.first == pii.second) {
      g_pErr->Report("EventID is obligatory argument for Event Async stop");
    } else {
      long id;
      if (from_string<long>(id, (pii.first)->second, std::dec)) {
	if (id != 0) {
	  m_idEvent = id;
	} else {
	  g_pErr->Report("EventID for EventAsyncStop cannot be 0");
	}
      } else {
	g_pErr->Report(pastestr::paste("ss", " ", (pii.first)->second.c_str(),
				       "is not a valid EventID"));
      }
    }
  }

}

int EventAsyncCtrl::Prepare() {
  if (m_bStart) {
    m_pEA->Prepare();
  } else {}

  return 0;
}

int EventAsyncCtrl::Action() {
  g_pErr->DFI("EventAsyncCtrl::Action", 0L, 3);
  EventAsync * pEA = (EventAsync *) m_pEA.get();

  if (pEA) {
    if (m_bStart) {
      pEA->Start();
    } else {
      g_pErr->Debug(pastestr::paste("sd", " ", "EventAsyncCtrl::Action; stopping eventID", pEA->ID()));
      pEA->Stop();
    }
  } else {
    // error 
  }
  g_pErr->DFO("EventAsyncCtrl::Action", 0L, 3);

  return 0;
}

int EventAsyncCtrl::FinalSweep(Template * pTemplate) {
  EventPtr epAsyncCtrl;

  if (!m_bStart) {
    // stop event
    // need to locate EventPtr for EventID we are controlling
    epAsyncCtrl = pTemplate->FindEvent(m_idEvent);
    if (epAsyncCtrl.get()) {
      if (epAsyncCtrl->CmdID() != SBX_EVENT_ASYNCSTART) {
	g_pErr->Report(pastestr::paste("sdsd", " ",
				       "EventAsyncStop can only control EventAsyncStart events; event ID was",
				       epAsyncCtrl->ID(), "and EvtCmdID was", epAsyncCtrl->CmdID()));
      } else {
	m_pEA = ((EventAsyncCtrl *) epAsyncCtrl.get())->GetControlledEvent();
	g_pErr->Debug(pastestr::paste("sd", "", "controlling eventid=", m_pEA->ID()));
      }
    } else {
      g_pErr->Report(pastestr::paste("sds", " ",
				     "EventCtrlAsync::FinalSweep: EventID", m_idEvent, "not found!"));
    }
  } else {
    // start event
    if (m_pEA.get()) {
      m_pEA->FinalSweep(pTemplate);
    } else {}
  }

  return 0;
}
