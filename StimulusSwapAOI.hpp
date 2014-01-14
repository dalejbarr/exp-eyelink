#ifndef SBX_STIMULUSSWAPAOI_INCLUDED
#define SBX_STIMULUSSWAPAOI_INCLUDED

#include "StimulusBmp.hpp"
#include "global.hpp"

class Template;

class StimulusSwapAOI : public Stimulus {
protected:
  Template * m_pTemplate;
  AOIPtr m_pAOI1;
  AOIPtr m_pAOI2;
public:
  StimulusSwapAOI() {};
  StimulusSwapAOI(Template * pTemplate, ArgMMap mmArgs);
  virtual ~StimulusSwapAOI();
  virtual int Cleanup();
  virtual int Prepare();
  virtual int Action();
};

#endif
