#ifndef SBX_WATCH_GSC1MOVE_INCLUDED
#define SBX_WATCH_GSC1MOVE_INCLUDED

#include "WatchGamePadMove.hpp"
#include "EventGSC1DrawGrid.hpp"

class WatchGSC1Move : public WatchGamePadMove {
protected:
  EventGSC1DrawGrid * m_pGrid;
  Template * m_pTemplate;
public:
  WatchGSC1Move(long, long, ArgMMap, Template *);
  virtual int Prepare();
  virtual bool CheckWhich(Uint8 nGamePadUsed);
  virtual bool CheckCondition(SDL_Event * pEvt);
};

#endif
