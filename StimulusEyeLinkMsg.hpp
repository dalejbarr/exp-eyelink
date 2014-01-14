#ifndef EXP_STIMULUSEYELINKMSG_INCLUDED
#define EXP_STIMULUSEYELINKMSG_INCLUDED

#include "Stimulus.hpp"

class StimulusEyeLinkMsg : public Stimulus {
protected:
  char * m_pcMsg;
public:
  StimulusEyeLinkMsg(long id, Template * pTemplate, long idCmd, ArgMMap mmArgs);
  virtual ~StimulusEyeLinkMsg();
  //virtual int Prepare();
  virtual int Action();
};

#endif
