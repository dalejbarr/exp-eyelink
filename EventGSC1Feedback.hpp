#ifndef SBX_EVENTGSC1FEEDBACK_INCLUDED
#define SBX_EVENTGSC1FEEDBACK_INCLUDED

#include "Event.hpp"
#include "Template.hpp"
#include <utility>
#include "xy.hpp"
#include "EventGSC1DrawGrid.hpp"

class EventGSC1Feedback : public Event {
protected:
  EventGSC1DrawGrid * m_pGrid;
public:
  EventGSC1Feedback(long id, long msec, 
		    long idCmd, ArgMMap mmArgs, Template * pTemplate, EventGSC1DrawGrid * pGrid);
  virtual int Prepare();
  virtual int Action();
  inline void SetGridPtr(EventGSC1DrawGrid * pGrid) { m_pGrid = pGrid; };
};

#endif
