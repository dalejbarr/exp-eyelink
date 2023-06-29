#include "EventSwapAOI.hpp"

EventSwapAOI::EventSwapAOI(long id, long msec, long idCmd, 
			   ArgMMap mmArgs, Template * pTemplate) :
  Event(id, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventSwapAOI::EventSwapAOI", id, 4);

  m_ppAOI1 = NULL;
  m_ppAOI2 = NULL;;

  if (!pTemplate) {
    g_pErr->Report("problem: m_pTemplate undefined");
  } else {}

  m_mmArgs = mmArgs;
  
  g_pErr->DFO("EventSwapAOI::EventSwapAOI", id, 4);
}

int EventSwapAOI::ProcessArgs() {
  g_pErr->DFI("EventSwapAOI::ProcessArgs", m_id, 4);

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  if (m_mmArgs.count("AOI") != 2) {
    g_pErr->Report("Event SWAP_AOI must have exactly two AOI arguments");
  } else {}

  pii = m_mmArgs.equal_range("AOI");

  ii = pii.first;
  m_ppAOI1 = m_pTemplate->AOIArg(ii->second);
  m_sAOI1 = ii->second;
  ii++;
  m_ppAOI2 = m_pTemplate->AOIArg(ii->second);  
  m_sAOI2 = ii->second;

  g_pErr->Debug(pastestr::paste("ssss", " ", "AOI1=",
				m_sAOI1.c_str(), "AOI2=",
				m_sAOI2.c_str()));

  g_pErr->DFO("EventSwapAOI::ProcessArgs", m_id, 4);

  return 0;
}

int EventSwapAOI::Prepare() {
  ProcessArgs();
  
  if (!m_ppAOI1) {
    m_ppAOI1 = m_pTemplate->AOIArg(m_sAOI1);
  } else {}

  if (!m_ppAOI2) {
    m_ppAOI2 = m_pTemplate->AOIArg(m_sAOI2);
  } else {}

  if ( (!m_ppAOI1) || (!m_ppAOI2) ) {
    g_pErr->Report("null values in EventSwapAOI::Prepare()");
  } else {}

  g_pErr->DFI("EventSwapAOI::Prepare", 0L, 3);
  g_pErr->DFO("EventSwapAOI::Prepare", 0L, 3);

  return 0;
}

int EventSwapAOI::Action() {
  StimulusImg * p1 = NULL;
  StimulusImg * p2 = NULL;

  g_pErr->DFI("EventSwapAOI::Action", 0L, 3);
  
  if ( (!m_ppAOI1) || (!m_ppAOI2) ) {
    g_pErr->Report("null values in EventSwapAOI::Action()");
  } else {}

  p1 = (StimulusImg *) (*m_ppAOI1).get();
  p2 = (StimulusImg *) (*m_ppAOI2).get();
  
  int x = p1->m_x1;
  int y = p1->m_y1;
  int nLayer = p1->m_nLayer;

  g_pErr->Debug("here1");  
  p1->m_CurX.Set(p2->m_x1);
  g_pErr->Debug("here2");    
  p1->m_x1 = p2->m_x1;

  
  p1->m_CurY.Set(p2->m_y1);
  p1->m_y1 = p2->m_y1;

  p1->m_nLayer = p2->m_nLayer;
  p2->m_nLayer = nLayer;

  p2->m_CurX.Set(x);
  p2->m_x1 = x;
  p2->m_CurY.Set(y);
  p2->m_y1 = y;

  g_pErr->Debug("swapped attributes: now reinserting");
  
  m_pTemplate->Reinsert(p1);
  m_pTemplate->Reinsert(p2);
  m_pTemplate->Redraw(false);

  g_pErr->DFO("EventSwapAOI::Action", 0L, 3);

  return 0;
}

int EventSwapAOI::Cleanup() {
  m_ppAOI1 = NULL;
  m_ppAOI2 = NULL;

  return 0;
}
