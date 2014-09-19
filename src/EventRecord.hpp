#ifndef SBX_EVENT_RECORD_INCLUDED
#define SBX_EVENT_RECORD_INCLUDED

#include "Template.hpp"

class EventRecord : public Event {
protected:
  //InputDevPtr m_pDev;
  ArgMMap m_mmapArgs;
  string m_sResource;
  vector<InputDevPtr> m_vpDev;
public:
  EventRecord(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate, InputDevPtr pDev);
  EventRecord(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate, vector<InputDevPtr> vpDev);
  void ParseArgs(ArgMMap mmArgs);
  virtual int Prepare();
  virtual int Action();
  virtual int Cleanup();
};

#endif
