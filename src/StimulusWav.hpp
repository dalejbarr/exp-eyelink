#ifndef EXP_STIMULUSWAV_INCLUDED
#define EXP_STIMULUSWAV_INCLUDED

#include <string>
using std::string;

#include "Stimulus.hpp"
#include "SDL.h"
#include "SDL_mixer.h"
#include "global.hpp"

/*
#include <map>
using std::pair;
using std::multimap;
*/

#include <string>
using std::string;

class StimulusWav : public Stimulus {
protected:
  bool m_bConditional; // for conditional playback (using "Bit" arg)
  bool m_bConditional_TargetValue; // for conditional playback (using "Value" arg)
  int m_nBit;          // for conditional playback depending on
                       // bits of Experiment::g_bsFlag
public:
  Mix_Chunk * m_pSound;
  int m_nLoop;
  int m_msStart;
  int m_msEnd;
  StimulusWav(long id, Template * pTemplate, const char * pName);
  StimulusWav(long id, Template * pTemplate, long idCmd, ArgMMap mmArgs);
  void Initialize();
  virtual ~StimulusWav();
  virtual int Prepare();
  virtual int Action();
  virtual int Finish();
  virtual int Cleanup();
};

#endif
