#include "EventResetCounter.hpp"
#include "Mouse_SDL.hpp"
#include "WatchMouse.hpp"
#include <sstream>
#include <boost/lexical_cast.hpp>
using std::ostringstream;

EventResetCounter::EventResetCounter(long idEvent, long msec, long idCmd, 
			 ArgMMap mmArgs, Template * pTemplate) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventResetCounter::EventResetCounter", idEvent, 3);

  m_pTemplate = pTemplate;

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  pii = mmArgs.equal_range("CounterID");
  if (pii.first != pii.second) {
    m_strCounterID = (pii.first)->second;
  } else {}

  g_pErr->Debug(pastestr::paste("ss", " ",
				"resetting", m_strCounterID.c_str()));

  g_pErr->DFO("EventResetCounter::EventResetCounter", idEvent, 3);
}

int EventResetCounter::Action() {
  g_pErr->DFI("EventResetCounter::Action", ID(), 3);

  if (m_pTemplate != NULL) {
    m_pTemplate->ResetCounter(m_strCounterID.c_str());
  } else {
    g_pErr->Report("in EventResetCounter::Action: m_pTemplate was NULL!");
  }

  g_pErr->DFO("EventResetCounter::Action", ID(), 3);
  return Event::Action();
}
