#ifndef SBX_WATCHDONE_INCLUDED
#define SBX_WATCHDONE_INCLUDED

#include "global.hpp"
#include "Watch.hpp"

class Template;

class WatchDone : public Watch {
protected:
  long m_idEvent;
public:
  WatchDone(long idWatch, long idNext, ArgMMap mmap);
  virtual bool CheckCondition(SDL_Event * pEvt);
};

#endif
