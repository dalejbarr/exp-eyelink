#ifndef SBX_EVENTPUPILLABSMSG_INCLUDED
#define SBX_EVENTPUPILLABSMSG_INCLUDED

#include "Event.hpp"
#include "PupilLabsTracker.hpp"

class EventPupilLabsMsg : public Event {
public:  
  EventPupilLabsMsg(long idEvent, long msec, long idCmd, ArgMMap mmArgs, 
		    Template * pTemplate);
  virtual ~EventPupilLabsMsg();
  static PupilLabsTracker * s_pPupilLabsTracker;
  virtual int Prepare();
  virtual int Action();
};

#endif
