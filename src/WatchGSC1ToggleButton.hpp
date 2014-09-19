#ifndef SBX_WATCH_GSC1TOGGLEBUTTON_INCLUDED
#define SBX_WATCH_GSC1TOGGLEBUTTON_INCLUDED

#include "WatchGamePadButton.hpp"
#include "StimulusWav.hpp"
#include "EventGSC1DrawGrid.hpp"

class WatchGSC1ToggleButton : public WatchGamePadButton {
protected:
  EventGSC1DrawGrid * m_pGrid;
  Template * m_pTemplate;
  //vector<Uint32> m_vPress;
public:
  WatchGSC1ToggleButton(long, long, ArgMMap, Template *);
  virtual bool CheckWhich(Uint8 nGamePadUsed);
  virtual bool CheckCondition(SDL_Event * pEvt);  
  virtual int Prepare();
};

#endif
