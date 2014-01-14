#ifndef SBX_EVENTASYNC_INCLUDED
#define SBX_EVENTASYNC_INCLUDED

#include "Event.hpp"

class EventAsync : public Event {
protected:
  bool m_bRunning;
  Uint32 m_msUpdate; // milliseconds between each update
  Uint32 m_msBegin; // time when event began
  Uint32 m_msLast; // time of last update
public:
  EventAsync(long id, long msec);
  EventAsync(long id, long msec, StimulusPtr pStim);
  EventAsync(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate = NULL);
  virtual void Initialize();
  virtual int Update();
  virtual int Start();
  virtual int Stop();
  virtual int StartProc();
  virtual int StopProc();
  virtual int UpdateProc();
};

#endif
