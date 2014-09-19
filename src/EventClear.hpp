#ifndef SBX_EVENT_CLEAR_INCLUDED
#define SBX_EVENT_CLEAR_INCLUDED

#include "Template.hpp"

class EventClear : public Event {
public:
  EventClear(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Action();
  int m_x1;
  int m_y1;
  int m_x2;
  int m_y2;
  bool m_bScreen;  // clear the whole screen
};

#endif
