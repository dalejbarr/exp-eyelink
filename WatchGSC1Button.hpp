#ifndef SBX_WATCH_GSC1BUTTON_INCLUDED
#define SBX_WATCH_GSC1BUTTON_INCLUDED

#include "WatchGamePadButton.hpp"

class WatchGSC1Button : public WatchGamePadButton {
public:
  WatchGSC1Button(long, long, ArgMMap);
  virtual bool CheckWhich(Uint8 nGamePadUsed);
};

#endif
