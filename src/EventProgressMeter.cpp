#include "EventProgressMeter.hpp"
#include "StimulusImg.hpp"
#include "Template.hpp"

EventProgressMeter::EventProgressMeter(long id, long msec, long idCmd,
				     ArgMMap mmArgs, Template * pTemplate /*= NULL*/) :
  EventAnimation(id, msec, idCmd, mmArgs, pTemplate) {

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  /*
  pii = mmArgs.equal_range("DrawGridID");
  if (pii.first == pii.second) {
    g_pErr->Report("DrawGridID is mandatory argument for EventProgressMeter");
  } else {
    if (!from_string(m_idDrawGrid, (pii.first)->second, std::dec)) {
      g_pErr->Report("Invalid DrawGridID from EventProgressMeter");
    } else {}
  }
  */

  m_msTotal = 25000;
  pii = mmArgs.equal_range("TimerLength");
  if (pii.first == pii.second) {
  } else {
    long ms;
    if (!from_string(ms, (pii.first)->second, std::dec)) {
      g_pErr->Report("Invalid TimerLength from EventProgressMeter");
    } else {
      m_msTotal = (Uint32) ms;
    }
  }
}

int EventProgressMeter::StartProc() {
  m_pTemplate->Redraw(false);
  return 0;
}

int EventProgressMeter::Prepare() {
  g_pErr->DFI("EventProgressMeter::Prepare", ID(), 3);

  m_pImage->Prepare();

  StimulusImg * pStim = (StimulusImg *) m_pImage.get();
  pStim->DrawRect(1, 1, 374, 9, 255, 0, 0);

  m_pTemplate->Redraw(true);
  
  g_pErr->DFO("EventProgressMeter::Prepare", ID(), 3);
  return 0;
}

int EventProgressMeter::FinalSweep(Template * pTemplate) {

  g_pErr->DFI("EventProgressMeter::FinalSweep", ID(), 3);
  
  // EventPtr ep;

  // ep = pTemplate->FindEvent(m_idDrawGrid);
  
  m_pImage = StimulusPtr(new StimulusImg(0L, pTemplate, "pm1.bmp",
					 200,
					 400,
					 -1, -1, 1));

  // m_pTemplate->InsertAOI("progress", m_pImage);

  g_pErr->DFO("EventProgressMeter::FinalSweep", ID(), 3);
  
  return 0;
}

int EventProgressMeter::NextFrame() {
  
  static Uint32 msElapsed = 0;

  msElapsed = m_msLast - m_msBegin;
  if (msElapsed <= m_msTotal) {
    
    float pmwidth = (msElapsed >= m_msTotal) ?
      0.0 : ((float) (m_msTotal - msElapsed)) / m_msTotal;

    StimulusImg * pStim = (StimulusImg *) m_pImage.get();
    //pStim->DrawRect(1, 1, m_epDrawGrid->m_nGridWidth-1, 9, 0, 0, 0);
    //pStim->DrawRect(1, 1, s1+(m_epDrawGrid->m_nGridWidth-2)*pmwidth, 9, 255, 0, 0);

    m_pTemplate->Redraw(false);
  } else {
    Stop();
  }

  return 0;
}

