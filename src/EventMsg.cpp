#include "EventMsg.hpp"
#include "Mouse_SDL.hpp"
#include "WatchMouse.hpp"
#include <sstream>
using std::ostringstream;

EventMsg::EventMsg(long idEvent, long msec, long idCmd, 
			 ArgMMap mmArgs, Template * pTemplate) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventMsg::EventMsg", idEvent, 3);

  m_newmsg = "";

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  pii = mmArgs.equal_range("Type");
  if (pii.first != pii.second) {
    m_msg.first = (pii.first)->second;
  } else {
    m_msg.first = "MSG";
  }

  pii = mmArgs.equal_range("Msg");
  if (pii.first != pii.second) {
    m_msg.second = (pii.first)->second;
  } else {
    m_msg.second = "";
  }

  g_pErr->DFO("EventMsg::EventMsg", idEvent, 3);
}

int EventMsg::Action() {
  g_pErr->DFI("EventMsg::Action", ID(), 3);

  ostringstream ostr;
  long idWatch = -1;

  if (m_msg.second[0] == '@') {
    for (int i = 7; i < m_msg.second.size(); i++) {
      if (m_msg.second[i] == ']') {
	break;
      } else {
	 ostr << m_msg.second[i];
	 //break;
       }
     }
     idWatch = atol(ostr.str().c_str());
     g_pErr->Debug(pastestr::paste("dss"," ",(long) idWatch, " watchID ", ostr.str().c_str()));
     WatchMouse * pwm = (WatchMouse *) m_pTemplate->FindWatch(idWatch);
     if (pwm) {
       m_newmsg = pwm->m_strSelectedAOIType;
     } else {
       g_pErr->Report(pastestr::paste("sd", "", "couldn't find watchID ", (long) idWatch));
     }
   } else {
     m_newmsg = m_msg.second;
   }

   m_pTemplate->StoreMsg(ClockFn(), MsgPair(m_msg.first, m_newmsg));

  g_pErr->DFO("EventMsg::Action", ID(), 3);

  return Event::Action();
}
