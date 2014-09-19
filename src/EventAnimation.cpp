#include "EventAnimation.hpp"

EventAnimation::EventAnimation(long id, long msec, long idCmd, ArgMMap mmArgs, 
			       Template * pTemplate /*= NULL*/) :
  EventAsync(id, msec, idCmd, mmArgs, pTemplate) {  
}

int EventAnimation::UpdateProc() {
  return NextFrame();
}

int EventAnimation::NextFrame() {
  return 0;
}
