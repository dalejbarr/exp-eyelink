#include "EventSocketSendMsg.hpp"
#include <boost/lexical_cast.hpp>
#include "Stimulus.hpp"

EventSocketSendMsg::EventSocketSendMsg(long idEvent, long msec, long idCmd, 
																			 ArgMMap mmArgs, Template * pTemplate, 
																			 InputDevPtr pDev) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventSocketSendMsg::EventSocketSendMsg", idEvent, 3);

  m_pTemplate = pTemplate;
	m_pDev = pDev;

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;
  string strTarget = "";

  pii = mmArgs.equal_range("SocketID");
  if (pii.first != pii.second) {
    m_idSocket = boost::lexical_cast<Uint32>((pii.first)->second.c_str());    
  } else {
		g_pErr->Report("no socket ID provided for EventSocketSendMsg");
	}

	m_bSendTimestamp = true;
  pii = mmArgs.equal_range("Message");
  if (pii.first != pii.second) {
		m_bSendTimestamp = false;
    m_strMessage = (pii.first)->second;
  } else {}

  g_pErr->DFO("EventSocketSendMsg::EventSocketSendMsg", idEvent, 3);
}

int EventSocketSendMsg::Action() {
  g_pErr->DFI("EventSocketSendMsg::Action", ID(), 3);
  string s1 = Stimulus::GetResourceString(m_strMessage.c_str());

	((Socket *) m_pDev.get())->SendMessage(s1);
  g_pErr->DFO("EventSocketSendMsg::Action", ID(), 3);

  return Event::Action();
}
