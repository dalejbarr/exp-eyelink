#ifndef EXP_WATCHMOUSE_INCLUDED
#define EXP_WATCHMOUSE_INCLUDED

#define EXP_ANYMOUSEBUTTON 0

#include "Watch.hpp"
#include "SDL.h"
#include "Stimulus.hpp"
#include "StimulusImg.hpp"
#include "Mouse_SDL.hpp"

// numbers represent "priority" of event, AOI is highest
#define WATCHMOUSE_NOREGION 0
#define WATCHMOUSE_XY 1
#define WATCHMOUSE_AOI 2

#include <vector>
using std::vector;

class WatchMouse : public Watch {
protected:
  Uint8 m_nButton;
  Uint8 m_nState;
  int m_nRegion;
  int m_x1, m_y1, m_x2, m_y2;
  vector<string> m_vAOI;
  StimulusPtrMMap * m_pmmapAOI;
public:
  //StimulusBmp * m_pSelectedAOI;
  string m_strSelectedAOIType;
  StimulusPtr m_pSelectedAOI;
  WatchMouse(long id, long idNext, ArgMMap mmap, StimulusPtrMMap * pmmapAOI = NULL);
  inline Uint8 GetButton() { return m_nButton; }
  inline Uint8 GetState() { return m_nState; }
  inline int GetRegion() { return m_nRegion; }
  virtual bool CheckCondition(SDL_Event * pEvt);
  virtual int Prepare();
  virtual int Finish();
  void ResetSelection();
  static Mouse_SDL * s_pMouse;
};

#endif
