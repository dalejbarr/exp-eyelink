#ifndef EXP_WATCHGAMEPADMOVE_INCLUDED
#define EXP_WATCHGAMEPADMOVE_INCLUDED

#include "Watch.hpp"
#include "SDL.h"
#include "Stimulus.hpp"
#include "StimulusImg.hpp"
#include "GamePad_SDL.hpp"

#include <vector>
using std::vector;

class WatchGamePadMove : public Watch {
protected:
  Uint8 m_nAxis;
  Uint8 m_nDir; // direction
  GamePad_SDL * m_pGamePad;
public:
  WatchGamePadMove(long id, long idNext, ArgMMap mmap);
  virtual ~WatchGamePadMove();
  //inline Uint8 GetState() { return m_nState; }
  virtual bool CheckCondition(SDL_Event * pEvt);
  virtual bool CheckWhich(Uint8 nGamePadUsed);
};

#endif
