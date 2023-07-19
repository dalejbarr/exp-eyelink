#ifndef SBX_EVENTPROGRESSMETER_INCLUDED
#define SBX_EVENTPROGRESSMETER_INCLUDED

#include "EventAnimation.hpp"

/*********************************************
 Draw a rectangle that empties as time passes
 ********************************************/

class EventProgressMeter : public EventAnimation {
protected:
  // EventGSC1DrawGrid * m_epDrawGrid; // pointer to draw grid event
  Uint32 m_msTotal;
  StimulusPtr m_pImage;
  //StimulusPtr m_pDec;
public:
  EventProgressMeter(long id, long msec, long idCmd,
		     ArgMMap mmArgs, Template * pTemplate = NULL);  
  virtual int FinalSweep(Template * pTemplate);
  virtual int Prepare();
  virtual int StartProc();
  virtual int NextFrame();
};

#endif
