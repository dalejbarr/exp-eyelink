#ifndef EXP_WATCHEYELINKAOI_INCLUDED
#define EXP_WATCHEYELINKAOI_INCLUDED

#include "Watch.hpp"
#include "EyeLinkMonitor.hpp"
#include "Stimulus.hpp"

class WatchEyeLinkAOI : public Watch {
protected:
  vector<string> m_vAOI;
  StimulusPtrMMap * m_pmmapAOI;
public:
  static EyeLinkMonitor * m_pEyelink;
  WatchEyeLinkAOI(long id, long idNext, ArgMMap mmap,
		  StimulusPtrMMap * pmmapAOI);
  virtual bool CheckCondition(SDL_Event * pEvt);
  virtual int Activate();
  virtual int Finish();
};

#endif
