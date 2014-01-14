#ifndef SBX_STIMULUS_SHOWAOI_INCLUDED
#define SBX_STIMULUS_SHOWAOI_INCLUDED

#include "Stimulus.hpp"

class StimulusShowAOI : public Stimulus {
public:
  StimulusShowAOI(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  ~StimulusShowAOI();
  virtual int Action();
  vector<string> m_vsShow;
};

#endif
