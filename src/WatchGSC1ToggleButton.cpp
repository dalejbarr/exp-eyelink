#include "WatchGSC1ToggleButton.hpp"
#include "CmdID.hpp"

WatchGSC1ToggleButton::WatchGSC1ToggleButton(long id, long idNext, ArgMMap mmap, Template * pTemplate) : 
  WatchGamePadButton(id, idNext, mmap) {
  m_pGrid = NULL;
  m_pTemplate = pTemplate;
}

bool WatchGSC1ToggleButton::CheckWhich(Uint8 nGamePadUsed) {
  if (EventGSC1DrawGrid::g_bEncoderLeft) {
    nGamePadUsed = !nGamePadUsed;
  } else {}
  return WatchGamePadButton::CheckWhich(nGamePadUsed);
}

int WatchGSC1ToggleButton::Prepare() {
  //m_vPress.clear();
  m_pGrid = (EventGSC1DrawGrid *) m_pTemplate->FindEventByCmdID(SBX_EVENT_GSC1DRAWGRID).get();
}

bool WatchGSC1ToggleButton::CheckCondition(SDL_Event * pEvt) {
  bool bResult = false;

  if (m_pGrid && CheckWhich(pEvt->jbutton.which)) {
    if (WatchGamePadButton::CheckCondition(pEvt)) {
      //m_vPress.push_back(ClockFn());
      m_pGrid->Toggle();
    } else {}
  } else {}

  return false;
}
