#ifndef SBX_EVENTTIME_INCLUDED
#define SBX_EVENTTIME_INCLUDED
#include "global.hpp"

class EventTime {
public:
  unsigned long m_id;
  unsigned long m_ms;
  EventTime(unsigned long id, unsigned long ms = ClockFn()) :
    m_id( id ), m_ms( ms ) {};
};

typedef boost::shared_ptr<EventTime> EventTimePtr;
#endif
