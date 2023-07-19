#include <regex>
using namespace std;

#include "EventSwapAOI2.hpp"

EventSwapAOI2::EventSwapAOI2(long id, long msec, long idCmd, 
			     ArgMMap mmArgs, Template * pTemplate) :
  EventSwapAOI(id, msec, idCmd, mmArgs, pTemplate) {
}

int EventSwapAOI2::ProcessArgs() {
  g_pErr->DFI("EventSwapAOI2::ProcessArgs", m_id, 4);
  
  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  m_ppAOI1 = NULL;
  m_ppAOI2 = NULL;

  if (m_mmArgs.count("Message") != 1) {
    g_pErr->Report("Event SWAP_AOI2 must have 'Message' argument");
  } else {}

  pii = m_mmArgs.equal_range("Message");
  ii = pii.first;
  m_strMessage = ii->second;

  g_pErr->DFO("EventSwapAOI2::ProcessArgs", m_id, 4);
  
  return 0;
}

int EventSwapAOI2::Prepare() {
  ProcessArgs();
  
  return 0;
}

int EventSwapAOI2::Action() {
  g_pErr->Debug(pastestr::paste("ss", "", "watch is: ", m_strMessage.c_str()));
  string smsg = m_pTemplate->GetMessageFromWatchID(m_strMessage);

  g_pErr->Debug(pastestr::paste("ss", " ", "string received was >>>>> : ",
				smsg.c_str()));
  
  regex rx(",");
  smatch m;
  
  if (regex_search(smsg, m, rx)) {
    m_ppAOI1 = m_pTemplate->AOIArg(m.prefix());
    m_ppAOI2 = m_pTemplate->AOIArg(m.suffix());
  } else {}
  
  //m_ppAOI1 = NULL;
  //m_ppAOI2 = NULL;
  // m_ppAOI1 = m_pTemplate->AOIArg(m_sAOI1);
  
  return EventSwapAOI::Action();
  //return 0;
}
