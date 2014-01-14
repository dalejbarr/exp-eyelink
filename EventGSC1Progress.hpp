#ifndef SBX_EVENTGSC1PROGRESS_INCLUDED
#define SBX_EVENTGSC1PROGRESS_INCLUDED

#include "EventAnimation.hpp"
#include "EventGSC1DrawGrid.hpp"

class EventGSC1Progress : public EventAnimation {
protected:
  long m_idDrawGrid; // drawgridID for locating grid on screen
  EventGSC1DrawGrid * m_epDrawGrid; // pointer to draw grid event
  Uint32 m_msTotal;
  StimulusPtr m_pEnc;
  StimulusPtr m_pDec;
public:
  EventGSC1Progress(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate = NULL);  
  virtual int FinalSweep(Template * pTemplate);
  virtual int Prepare();
  virtual int StartProc();
  virtual int NextFrame();
};

#endif
