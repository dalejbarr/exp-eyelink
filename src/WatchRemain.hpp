#ifndef SBX_WATCHREMAIN_INCLUDED
#define SBX_WATCHREMAIN_INCLUDED

#include "global.hpp"
#include "Watch.hpp"

//class Template;

class WatchRemain : public Watch {
protected:
  Uint32 m_msWait;
	SDL_TimerID m_timerID;
public:
  WatchRemain(long idWatch, long idNext, ArgMMap mmap);
	virtual int Activate();
  virtual bool CheckCondition(SDL_Event * pEvt);
	static Uint32 PostTimeout(Uint32 interval, void * param);
};

#endif
