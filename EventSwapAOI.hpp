#ifndef SBX_EVENTSWAPAOI_INCLUDED
#define SBX_EVENTSWAPAOI_INCLUDED

#include "Event.hpp"
#include "Template.hpp"

class EventSwapAOI : public Event {
private:
  StimulusPtr * m_ppAOI1;
  StimulusPtr * m_ppAOI2;
  string m_sAOI1;
  string m_sAOI2;
public:
  EventSwapAOI(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Prepare();
  virtual int Action();
  virtual int Cleanup();
};

#endif
