#include "EventIncrementCounter.hpp"
#include "Mouse_SDL.hpp"
#include "WatchMouse.hpp"
#include <sstream>
#include <boost/lexical_cast.hpp>
using std::ostringstream;

EventIncrementCounter::EventIncrementCounter(long idEvent, long msec, long idCmd, 
			 ArgMMap mmArgs, Template * pTemplate) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventIncrementCounter::EventIncrementCounter", idEvent, 3);

  m_pTemplate = pTemplate;

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;
  string strTarget = "";

  pii = mmArgs.equal_range("CounterID");
  if (pii.first != pii.second) {
    m_strCounterID = (pii.first)->second;
  } else {}

  g_pErr->Debug(pastestr::paste("ss", " ",
				"will increment counter", m_strCounterID.c_str()));

  g_pErr->DFO("EventIncrementCounter::EventIncrementCounter", idEvent, 3);
}

int EventIncrementCounter::Action() {
  g_pErr->DFI("EventIncrementCounter::Action", ID(), 3);
  if (m_pTemplate != NULL) {
    m_pTemplate->IncrementCounter(m_strCounterID.c_str());
  } else {
    g_pErr->Report("in EventIncrementCounter::Action: m_pTemplate was NULL!");
  }
  g_pErr->DFO("EventIncrementCounter::Action", ID(), 3);

  return Event::Action();
}
