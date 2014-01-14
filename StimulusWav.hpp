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
public:
  Mix_Chunk * m_pSound;

  StimulusWav(long id, Template * pTemplate, const char * pName);
  StimulusWav(long id, Template * pTemplate, long idCmd, ArgMMap mmArgs);
  void Initialize();
  virtual ~StimulusWav();
  virtual int Prepare();
  virtual int Action();
  virtual int Finish();
};

#endif
