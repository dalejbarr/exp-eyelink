#ifndef SCROLLTRACKGP_SDL_INCLUDED
#define SCROLLTRACKGP_SDL_INCLUDED

#include "InputDev.hpp"
#include "RespObj.hpp"
//#include "global.hpp"
#include "GamePad_SDL.hpp"
#include "Display_SDL.hpp"
#include "StimulusImg.hpp"

class ScrollTrackGP_SDL : public GamePad_SDL {
protected:
  StimulusImg * m_pDisp;
  string m_sResource;
  SDL_Rect m_rect;
  SDL_Rect m_rectOld;
  SDL_Rect m_rectViewPort;
public:
  ScrollTrackGP_SDL(unsigned long idDev, int nIndex);
  virtual ~ScrollTrackGP_SDL();
  virtual void Prepare();
  virtual void Start();
  virtual void Process(RespObj gp);
  virtual void SetResource(string s);
  virtual void Redraw(bool bForce = false);
};

// extern Display_SDL * g_pDisplay;

#endif
