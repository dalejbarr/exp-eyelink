#include "EventUpdate.hpp"
#include "pastestr.hpp"
#include "Template.hpp"

EventUpdate::EventUpdate(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate) :
  Event(id, msec) {

  g_pErr->DFI("EventUpdate::EventUpdate", id, 4);
  m_poSource = NULL;
  m_mmArgs = mmArgs;
  m_pTemplate = pTemplate;

  g_pErr->DFO("EventUpdate::EventUpdate", id, 4);
}

int EventUpdate::Prepare() {
  g_pErr->DFI("EventUpdate::Prepare", m_id, 4);
  m_poSource = NULL;

  pair<ArgIter, ArgIter> pii;
  ArgIter ii;
  
  pii = m_mmArgs.equal_range("SOURCE");
  if (pii.first == pii.second) {
    g_pErr->Report(pastestr::paste("sd", "", 
				   "Missing argument 'SOURCE' for UPDATE, EventID=", m_id));
  } else {
    m_poSource = m_pTemplate->FindVar((*pii.first).second);
    if (!m_poSource) {
      g_pErr->Report(pastestr::paste("ss", "",
				     "Couldn't find source variable ", (*pii.first).second.c_str()));
    } else {
      g_pErr->Debug(pastestr::paste("ss", "",
				    "SOURCE is ", (*pii.first).second.c_str()));
    }
  }

  pii = m_mmArgs.equal_range("TARGET");
  if (pii.first == pii.second) {
    g_pErr->Report(pastestr::paste("sd", "", 
				   "Missing argument 'TARGET' for UPDATE, EventID=", m_id));
  } else {
    g_pErr->Debug(pastestr::paste("ss", "", "TARGET is ", (*pii.first).second.c_str()));
    //m_oNew.Set(m_pTemplate->NewOplist((*pii.first).second));
  }

  g_pErr->DFO("EventUpdate::Prepare", m_id, 4);
  return 0;
}

int EventUpdate::Action() {
  g_pErr->DFI("EventUpdate::Action", m_id, 4);  
  (*m_poSource) = m_oNew;
  g_pErr->DFO("EventUpdate::Action", m_id, 4);  
  return 0;
}

EventUpdate::~EventUpdate() {
  m_mmArgs.clear();
}
