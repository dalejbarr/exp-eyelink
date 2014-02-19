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
  string strTarget = "";

  pii = mmArgs.equal_range("CounterID");
  if (pii.first != pii.second) {
    m_strCounterID = (pii.first)->second;
  } else {}

  pii = mmArgs.equal_range("Target");
  if (pii.first != pii.second) {
    strTarget = (pii.first)->second;
  } else {}
  m_lTarget = boost::lexical_cast<long>(strTarget.c_str());

  pii = mmArgs.equal_range("Operation");
  if (pii.first != pii.second) {
    m_strOperation = (pii.first)->second;
  } else {}

  g_pErr->Debug(pastestr::paste("sssd", " ",
				"repeating if", m_strCounterID.c_str(), 
				m_strOperation.c_str(),
				(long) m_lTarget));

  g_pErr->DFO("EventResetCounter::EventResetCounter", idEvent, 3);
}

int EventResetCounter::Action() {
  bool bCondition = true;

  g_pErr->DFI("EventResetCounter::Action", ID(), 3);
  if (bCondition) {
    if (m_pTemplate != NULL) {
      m_pTemplate->RepeatExperiment();
    } else {
      g_pErr->Report("in EventResetCounter::Action: m_pTemplate was NULL!");
    }
  } else {}
  g_pErr->DFO("EventResetCounter::Action", ID(), 3);

  return Event::Action();
}
