#include "WatchGSC1Move.hpp"
#include "EventGSC1DrawGrid.hpp"
#include "CmdID.hpp"

WatchGSC1Move::WatchGSC1Move(long id, long idNext, ArgMMap mmap, Template * pTemplate) : WatchGamePadMove(id, idNext, mmap) {
  m_pGrid = NULL;
  m_pTemplate = pTemplate;
}

bool WatchGSC1Move::CheckWhich(Uint8 nGamePadUsed) {
  if (EventGSC1DrawGrid::g_bEncoderLeft) {
    nGamePadUsed = !nGamePadUsed;
  } else {}
  return WatchGamePadMove::CheckWhich(nGamePadUsed);
}

int WatchGSC1Move::Prepare() {
  m_pGrid = (EventGSC1DrawGrid *) m_pTemplate->FindEventByCmdID(SBX_EVENT_GSC1DRAWGRID).get();
}

bool WatchGSC1Move::CheckCondition(SDL_Event * pEvt) {
  bool bResult = false;

  if (m_pGrid && CheckWhich(pEvt->jaxis.which)) {

    if (pEvt->jaxis.axis == 0) {
      if (pEvt->jaxis.value < -3000) {
	m_pGrid->MoveLeft();
      } else if (pEvt->jaxis.value > 3000) {
	m_pGrid->MoveRight();
      } else {}
    } else if (pEvt->jaxis.axis == 1) {
      if (pEvt->jaxis.value < -3000) {
	m_pGrid->MoveUp();
      } else if (pEvt->jaxis.value > 3000) {
	m_pGrid->MoveDown();
      } else {}
    } else {}
  } else {}

  return bResult;
}
