#ifndef EXP_STIMULUSDISPLAYON_INCLUDED
#define EXP_STIMULUSDISPLAYON_INCLUDED

#include "Stimulus.hpp"

class StimulusDisplayOn : public Stimulus {
public:
  StimulusDisplayOn(long id, Template * pTemplate);
  virtual ~StimulusDisplayOn();
  virtual int Prepare();
  virtual int Action();
};

#endif
