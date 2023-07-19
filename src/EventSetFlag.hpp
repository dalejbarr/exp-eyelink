#ifndef SBX_EVENT_SET_FLAG_INCLUDED
#define SBX_EVENT_SET_FLAG_INCLUDED

#include "Template.hpp"

class EventSetFlag : public Event {
protected:
  Template * m_pTemplate;
  unsigned int m_nFlag;
public:
  EventSetFlag(long idEvent, long msec, long idCmd,
	       ArgMMap mmArgs, Template * pTemplate);
  virtual int Action();
};

#endif
