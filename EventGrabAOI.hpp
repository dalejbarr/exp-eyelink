#ifndef SBX_EVENTGRABAOI_INCLUDED
#define SBX_EVENTGRABAOI_INCLUDED

#include "Event.hpp"

class EventGrabAOI : public Event {
protected:
  Uint32 m_msLastDraw;
  int m_nOffX;
  int m_nOffY;
public:
  EventGrabAOI(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual int Action();
  virtual int Update(int x, int y);
};

#endif
