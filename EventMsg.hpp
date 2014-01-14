#ifndef SBX_EVENT_MSG_INCLUDED
#define SBX_EVENT_MSG_INCLUDED

#include "Template.hpp"

class EventMsg : public Event {
protected:
  MsgPair m_msg;
  string m_newmsg;
public:
  EventMsg(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Action();
};

#endif
