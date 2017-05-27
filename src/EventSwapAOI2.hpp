#ifndef SBX_EVENTSWAPAOI2_INCLUDED
#define SBX_EVENTSWAPAOI2_INCLUDED

#include "EventSwapAOI.hpp"

class EventSwapAOI2 : public EventSwapAOI {
protected:
  string m_strMessage;
public:
  EventSwapAOI2(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int ProcessArgs();
  virtual int Prepare();
  virtual int Action();
};

#endif
