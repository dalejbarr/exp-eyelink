#include "WatchGSC1Button.hpp"
#include "EventGSC1DrawGrid.hpp"

WatchGSC1Button::WatchGSC1Button(long id, long idNext, ArgMMap mmap) : WatchGamePadButton(id, idNext, mmap) {
}

bool WatchGSC1Button::CheckWhich(Uint8 nGamePadUsed) {
  if (EventGSC1DrawGrid::g_bEncoderLeft) {
    nGamePadUsed = !nGamePadUsed;
  } else {}
  return WatchGamePadButton::CheckWhich(nGamePadUsed);
}
