#ifndef GAMEPAD_SDL_INCLUDED
#define GAMEPAD_SDL_INCLUDED

#define GP_TOP_BUTTON 3
#define GP_BOTTOM_BUTTON 1
#define GP_LEFT_BUTTON 0
#define GP_RIGHT_BUTTON 2

#include "InputDev.hpp"
#include "RespObj.hpp"
//#include "global.hpp"
#include "GPEvent.hpp"

class GamePad_SDL : public InputDev {
protected:
  SDL_Joystick * m_stick;
  //int m_nIndex;
  //vector<GPEvent> m_vpEvents;
public:
  GamePad_SDL(unsigned long idDev, int nIndex);
  virtual ~GamePad_SDL();  
  inline int GetIndex() {return m_nIndex;};
  virtual void HandleEvent(SDL_Event * pEvt);  
  virtual void Prepare();
  virtual void Start();
  virtual void Stop();
  virtual void Process(RespObj gp);
  virtual void Dealloc();
};

#endif
