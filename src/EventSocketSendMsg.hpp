#ifndef SBX_EVENT_SOCKET_SEND_MSG_INCLUDED
#define SBX_EVENT_SOCKET_SEND_MSG_INCLUDED

#include "Template.hpp"
#include "Socket.hpp"

class EventSocketSendMsg : public Event {
protected:
  Template * m_pTemplate;
  Uint32 m_idSocket;
  string m_strMessage;
	bool m_bSendTimestamp;
	InputDevPtr m_pDev;
  // long m_lTarget;
  // Uint32 m_ui32Timeout;
public:
  EventSocketSendMsg(long idEvent, long msec, long idCmd, ArgMMap mmArgs, 
										 Template * pTemplate, InputDevPtr pDev);
  virtual int Action();
};

#endif
