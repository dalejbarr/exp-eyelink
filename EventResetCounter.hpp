#ifndef SBX_EVENT_RESET_COUNTER_INCLUDED
#define SBX_EVENT_RESET_COUNTER_INCLUDED

#include "Template.hpp"

class EventResetCounter : public Event {
protected:
  Template * m_pTemplate;
  string m_strCounterID;
  string m_strOperation;
  long m_lTarget;
public:
  EventResetCounter(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Action();
};

#endif
