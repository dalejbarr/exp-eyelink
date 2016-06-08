#include "EventGSC1Progress.hpp"
#include "StimulusImg.hpp"
#include "Template.hpp"

EventGSC1Progress::EventGSC1Progress(long id, long msec, long idCmd,
				     ArgMMap mmArgs, Template * pTemplate /*= NULL*/) :
  EventAnimation(id, msec, idCmd, mmArgs, pTemplate) {

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  pii = mmArgs.equal_range("DrawGridID");
  if (pii.first == pii.second) {
    g_pErr->Report("DrawGridID is mandatory argument for EventGSC1Progress");
  } else {
    if (!from_string(m_idDrawGrid, (pii.first)->second, std::dec)) {
      g_pErr->Report("Invalid DrawGridID from EventGSC1Progress");
    } else {}
  }

  m_msTotal = 25000;
  pii = mmArgs.equal_range("TimerLength");
  if (pii.first == pii.second) {
  } else {
    long ms;
    if (!from_string(ms, (pii.first)->second, std::dec)) {
      g_pErr->Report("Invalid TimerLength from EventGSC1Progress");
    } else {
      m_msTotal = (Uint32) ms;
    }
  }



}

int EventGSC1Progress::StartProc() {
  m_pTemplate->Redraw(false);
  return 0;
}

int EventGSC1Progress::Prepare() {
  g_pErr->DFI("EventGSC1Progress::Prepare", ID(), 3);

  m_pEnc->Prepare();
  m_pDec->Prepare();

  StimulusImg * pStim = (StimulusImg *) m_pEnc.get();
  pStim->DrawRect(1, 1, 374, 9, 255, 0, 0);
  pStim = (StimulusImg *) m_pDec.get();
  pStim->DrawRect(1, 1, 374, 9, 255, 0, 0);

  m_pTemplate->Redraw(true);
  g_pErr->DFO("EventGSC1Progress::Prepare", ID(), 3);
  return 0;
}

int EventGSC1Progress::FinalSweep(Template * pTemplate) {
  EventPtr ep;
  g_pErr->DFI("EventGSC1Progress::FinalSweep", ID(), 3);
  ep = pTemplate->FindEvent(m_idDrawGrid);
  m_epDrawGrid = (EventGSC1DrawGrid *) ep.get();

  m_pEnc = StimulusPtr(new StimulusImg(0L, pTemplate, "pm1.bmp", 
					m_epDrawGrid->m_nGridTopEncX, 
					m_epDrawGrid->m_nGridTopEncY + m_epDrawGrid->m_nGridWidth,
					-1, -1, 1));
  m_pDec = StimulusPtr(new StimulusImg(0L, pTemplate, "pm1.bmp", 
					m_epDrawGrid->m_nGridTopDecX, 
					m_epDrawGrid->m_nGridTopDecY + m_epDrawGrid->m_nGridWidth,
					-1, -1, 1));

  m_pTemplate->InsertAOI("progressEnc", m_pEnc);
  m_pTemplate->InsertAOI("progressDec", m_pDec);

  g_pErr->DFO("EventGSC1Progress::FinalSweep", ID(), 3);
  return 0;
}

int EventGSC1Progress::NextFrame() {
  static Uint32 msElapsed = 0;

  msElapsed = m_msLast - m_msBegin;
  if (msElapsed <= m_msTotal) {
    float pmwidth = msElapsed == m_msTotal ? 0.0 : ((float) (m_msTotal - msElapsed))/m_msTotal;

    StimulusImg * pStim = (StimulusImg *) m_pEnc.get();
    pStim->DrawRect(1, 1, m_epDrawGrid->m_nGridWidth-1, 9, 0, 0, 0);
    pStim->DrawRect(1, 1, 1+(m_epDrawGrid->m_nGridWidth-2)*pmwidth, 9, 255, 0, 0);

    pStim = (StimulusImg *) m_pDec.get();
    pStim->DrawRect(1, 1, m_epDrawGrid->m_nGridWidth-1, 9, 0, 0, 0);
    pStim->DrawRect(1, 1, 1+(m_epDrawGrid->m_nGridWidth-2)*pmwidth, 9, 255, 0, 0);

    m_pTemplate->Redraw(false);
  } else {
    Stop();
  }

  return 0;
}

