#ifndef SBX_EVENTUPDATEAOI_INCLUDED
#define SBX_EVENTUPDATEAOI_INCLUDED

#include "Template.hpp"

class EventUpdateAOI : public Event {
protected:
  StimulusPtr * m_ppAOI;
  //StimulusPtr m_pAOI2;
  string m_str;
  ArgMMap m_mmapArgs;
public:
  EventUpdateAOI(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Prepare();
  virtual int Action();
  virtual int Cleanup();
};

#endif
