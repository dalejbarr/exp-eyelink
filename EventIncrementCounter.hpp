#ifndef SBX_EVENT_INCREMENT_COUNTER_INCLUDED
#define SBX_EVENT_INCREMENT_COUNTER_INCLUDED

#include "Template.hpp"

class EventIncrementCounter : public Event {
protected:
  Template * m_pTemplate;
  string m_strCounterID;
  string m_strOperation;
  long m_lTarget;
public:
  EventIncrementCounter(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Action();
};

#endif
