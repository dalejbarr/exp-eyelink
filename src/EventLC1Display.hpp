#ifndef SBX_EVENT_LC1DISPLAY_INCLUDED
#define SBX_EVENT_LC1DISPLAY_INCLUDED

#include "Event.hpp"

class EventLC1Display : public Event {
public:
  EventLC1Display(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Action();
};

#endif
