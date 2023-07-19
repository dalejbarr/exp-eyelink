#ifndef EXP_STIMULUSTXT_INCLUDED
#define EXP_STIMULUSTXT_INCLUDED

#include "Stimulus.hpp"
#include "Display_SDL.hpp"

class StimulusTxt : public Stimulus {
public:
  StimulusTxt(long id, Template * pTemplate, long idCmd, ArgMMap mmArgs);
  virtual ~StimulusTxt();
  virtual int Prepare();
  virtual int Action();
  int m_x1;
  int m_y1;
};

// extern Display_SDL * g_pDisplay;

#endif
