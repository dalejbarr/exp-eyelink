#ifndef SBX_ANIMATION_INCLUDED
#define SBX_ANIMATION_INCLUDED

#include "global.hpp"
#include "EventAsync.hpp"

class EventAnimation : public EventAsync {
protected:
  Uint32 m_msLastUpdate;
public:
  EventAnimation(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate = NULL);
  virtual int NextFrame();
  virtual int UpdateProc();
};

#endif
