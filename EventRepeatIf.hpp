#ifndef SBX_EVENT_REPEATIF_INCLUDED
#define SBX_EVENT_REPEATIF_INCLUDED

#include "Template.hpp"

class EventRepeatIf : public Event {
protected:
  Template * m_pTemplate;
  string m_strCounterID;
  string m_strOperation;
  long m_lTarget;
  Uint32 m_ui32Timeout;
public:
  EventRepeatIf(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Action();
};

#endif
