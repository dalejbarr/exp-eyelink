#include "EventUpdateAOI.hpp"

EventUpdateAOI::EventUpdateAOI(long idEvent, long msec, long idCmd, 
			       ArgMMap mmArgs, Template * pTemplate) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  m_mmapArgs = mmArgs;
  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  pii = m_mmapArgs.equal_range("AOI");
  if (pii.first == pii.second) {
    g_pErr->Report("name of AOI not provided for EventUpdateAOI");
  } else {
    m_str = (pii.first)->second;
  }


}

int EventUpdateAOI::Prepare() {
  g_pErr->DFI("EventUpdateAOI::Prepare", m_id, 4);
  m_ppAOI = m_pTemplate->AOIArg(m_str);
  if (!m_ppAOI) {
    // not attached to template?
    g_pErr->Report(pastestr::paste("ss", " ", "couldn't find AOI", 
				   m_str.c_str()));
  } else {}

  /*
  StimulusBmp * pAOI = (StimulusBmp *) (*m_ppAOI).get();
  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;
  pii = m_mmapArgs.equal_range("Toggle");
  if (pii.first != pii.second) {
    m_pAOI2 = StimulusPtr(new StimulusBmp(0L, m_pTemplate,
					  (*pii.first).second.c_str(), // filename
					  pAOI->m_CurX.Get(), pAOI->m_CurY.Get(),
					  -1, -1, pAOI->m_nLayer)); 
    //pStim->SetLayer(atoi((*pii.first).second.c_str()));
  } else {}
  */

  g_pErr->DFO("EventUpdateAOI::Prepare", m_id, 4);
  return Event::Prepare();
}

int EventUpdateAOI::Action() {

  g_pErr->DFI("EventUpdateAOI::Action", m_id, 4);
  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;
  StimulusImg * pStim = (StimulusImg *) (*m_ppAOI).get();
  if (!pStim) {
    g_pErr->Report("couldn't get m_ppAOI");
  } else {
    g_pErr->Debug(pStim->GetResource().c_str());
  }

  pii = m_mmapArgs.equal_range("Layer");
  if (pii.first != pii.second) {
    pStim->SetLayer(atoi((*pii.first).second.c_str()));
  } else {}

  pii = m_mmapArgs.equal_range("ResetLoc");
  if (pii.first != pii.second) {
    pStim->ResetLoc();
    //pStim->Draw();
    //StimulusImg::Flip();
  } else {}

  pii = m_mmapArgs.equal_range("Highlight");
  if (pii.first != pii.second) {
    pStim->Highlight((*pii.first).second.c_str());
  } else {}

  pii = m_mmapArgs.equal_range("Image");
  if (pii.first != pii.second) {    
    pStim->Load(pastestr::paste("ss", "", "resource/", (*pii.first).second.c_str()));
  } else {}

  //AOIPtr p1 = AOIPtr(pStim);
  if (!m_pTemplate) {
    g_pErr->Report("programming error: m_pTemplate undefined");
  } else {}
  m_pTemplate->Reinsert(pStim);
  m_pTemplate->Redraw(0);

  // remove the old AOI
  // insert the new AOI

  g_pErr->DFO("EventUpdateAOI::Action", m_id, 4);
  return 0;
  //Event::Action();
}

int EventUpdateAOI::Cleanup() {
  g_pErr->DFI("EventUpdateAOI::Cleanup", m_id, 4);
  m_ppAOI = NULL;
  g_pErr->DFO("EventUpdateAOI::Cleanup", m_id, 4);
  return 0;
}
