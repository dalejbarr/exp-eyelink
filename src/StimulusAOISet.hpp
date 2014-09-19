#ifndef EXP_STIMULUSAOISET_INCLUDED
#define EXP_STIMULUSAOISET_INCLUDED

#include "StimulusAOI.hpp"

class StimulusAOISet : public Stimulus {
protected:
  StimAOISet m_setStimAOI;
public:
  StimulusAOISet(const char * pc, Template * pTemplate);
  virtual ~StimulusAOISet();
  int Cleanup();
};

#endif
