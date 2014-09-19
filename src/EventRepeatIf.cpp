#include "EventRepeatIf.hpp"
#include "Mouse_SDL.hpp"
#include "WatchMouse.hpp"
#include <sstream>
#include <boost/lexical_cast.hpp>
using std::ostringstream;

EventRepeatIf::EventRepeatIf(long idEvent, long msec, long idCmd, 
			 ArgMMap mmArgs, Template * pTemplate) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventRepeatIf::EventRepeatIf", idEvent, 3);

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

  m_ui32Timeout = 0;
  pii = mmArgs.equal_range("Timeout");
  if (pii.first != pii.second) {
    m_ui32Timeout = boost::lexical_cast<Uint32>((pii.first)->second.c_str());    
    g_pErr->Debug(pastestr::paste("sd", " ", "timeout set to", (long) m_ui32Timeout));
  } else {}

  g_pErr->Debug(pastestr::paste("sssd", " ",
				"repeating if", m_strCounterID.c_str(), 
				m_strOperation.c_str(),
				(long) m_lTarget));

  g_pErr->DFO("EventRepeatIf::EventRepeatIf", idEvent, 3);
}

int EventRepeatIf::Action() {
  bool bCondition = false;

  g_pErr->DFI("EventRepeatIf::Action", ID(), 3);

  if (m_strOperation=="LT") {
    bCondition = m_pTemplate->GetCounter(m_strCounterID.c_str()) < m_lTarget;
  } else {
    g_pErr->Report(pastestr::paste("sss", "", "EventRepeatIf operation '", m_strOperation.c_str(), "' undefined!"));
  }

  string strResult("not met");
  if (bCondition) {
    strResult = "met";
  } else {}

  g_pErr->Debug(pastestr::paste("sssds", " ", "Condition", m_strCounterID.c_str(), m_strOperation.c_str(), m_lTarget, strResult.c_str())); 

  Uint32 ui32Elapsed = m_pTemplate->GetMSElapsed();
  g_pErr->Debug(pastestr::paste("sd", "", "TIME ELAPSED: ", (long) ui32Elapsed));

  if (ui32Elapsed > m_ui32Timeout) {
    bCondition = false;
    g_pErr->Debug("timing out of Repeat Experiment");
  } else {}
  
  if (bCondition) {
    if (m_pTemplate != NULL) {
      m_pTemplate->RepeatExperiment();
    } else {
      g_pErr->Report("in EventRepeatIf::Action: m_pTemplate was NULL!");
    }
  } else {}
  g_pErr->DFO("EventRepeatIf::Action", ID(), 3);

  return Event::Action();
}
