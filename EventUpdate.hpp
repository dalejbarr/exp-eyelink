#ifndef SBX_EVENTUPDATE_INCLUDED
#define SBX_EVENTUPDATE_INCLUDED

#include "Event.hpp"
#include "Operation.hpp"

class EventUpdate : public Event {
protected:
  Operation m_oNew;
  Operation * m_poSource;
  ArgMMap m_mmArgs;
public:
  EventUpdate(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Prepare();
  virtual int Action();
  ~EventUpdate();
};

#endif
