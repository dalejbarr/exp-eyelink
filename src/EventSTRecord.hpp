#ifndef PERIRECORD_INCLUDED
#define PERIRECORD_INCLUDED

#include "EventRecord.hpp"

class EventSTRecord : public EventRecord {
public:
  EventSTRecord(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate, InputDevPtr pDev);
};

#endif
