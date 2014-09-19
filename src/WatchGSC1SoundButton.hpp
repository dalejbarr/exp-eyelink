#ifndef SBX_WATCH_GSC1SOUNDBUTTON_INCLUDED
#define SBX_WATCH_GSC1SOUNDBUTTON_INCLUDED

#include "WatchGamePadButton.hpp"
#include "StimulusWav.hpp"
#include "GSC1RespObj.hpp"

class Template;

class WatchGSC1SoundButton : public WatchGamePadButton {
protected:
  StimulusPtr m_pStim;
  vector<GSC1SoundButton> m_vPress;
  Template * m_pTemplate;
public:
  WatchGSC1SoundButton(long, long, ArgMMap, Template *);
  virtual ~WatchGSC1SoundButton();
  virtual bool CheckCondition(SDL_Event * pEvt);  
  virtual int Prepare();
  virtual int Cleanup();
  virtual int PostTrial();
};

#endif
