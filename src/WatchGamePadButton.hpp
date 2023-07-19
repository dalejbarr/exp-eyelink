#ifndef EXP_WATCHGAMEPADBUTTON_INCLUDED
#define EXP_WATCHGAMEPADBUTTON_INCLUDED

#define EXP_ANYGAMEPADBUTTON -1

#include "Watch.hpp"
#include "SDL.h"
#include "Stimulus.hpp"
#include "StimulusImg.hpp"
#include "GamePad_SDL.hpp"

#include <vector>
using std::vector;

class WatchGamePadButton : public Watch {
protected:
  vector<int> m_vButton;
  Uint8 m_nState;
  GamePad_SDL * m_pGamePad;
public:
  WatchGamePadButton(long id, long idNext, ArgMMap mmap);
  virtual ~WatchGamePadButton();
  //inline Uint8 GetButton() { return m_nButton; }
  inline Uint8 GetState() { return m_nState; }
  bool TargetButton(Uint8 nButton = EXP_ANYGAMEPADBUTTON);
  virtual bool CheckCondition(SDL_Event * pEvt);
  virtual bool CheckWhich(Uint8 nGamePadUsed);
  //virtual int Prepare();  
  //virtual int Finish();
};

#endif
