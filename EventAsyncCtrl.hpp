#ifndef SBX_EVENTASYNCCTRL_INCLUDED
#define SBX_EVENTASYNCCTRL_INCLUDED

#include "EventAsync.hpp"

class EventAsyncCtrl : public Event {
protected:
  bool m_bStart; // whether event is start event (TRUE) or stop event (FALSE)
  EventPtr m_pEA; // event to be controlled
  long m_idEvent; // eventID
public:
  EventAsyncCtrl(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate = NULL,
		 bool bStart = true);
  virtual int Prepare();
  virtual int Action();
  virtual int FinalSweep(Template * pTemplate);
  inline EventPtr GetControlledEvent() { return m_pEA; };
};

#endif
