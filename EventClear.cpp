#include "EventClear.hpp"
#include "Display_SDL.hpp"

EventClear::EventClear(long idEvent, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {

  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("x1");
  ArgIter ii;

  m_bScreen = false;
  if (pii.first != pii.second) {
    ii = pii.first;
    m_x1 = atoi((*ii).second.c_str());

    pii = mmArgs.equal_range("x2"); ii = pii.first;
    m_x2 = atoi((*ii).second.c_str());

    pii = mmArgs.equal_range("y1"); ii = pii.first;
    m_y1 = atoi((*ii).second.c_str());

    pii = mmArgs.equal_range("y2"); ii = pii.first;
    m_y2 = atoi((*ii).second.c_str());
  } else {
    // we are clearing the screen
    m_bScreen = true;
  }

}

int EventClear::Action() {
  if (m_bScreen) {
    Display_SDL::ClearScreen();
  } else {
    Display_SDL::ClearRegion(m_x1, m_y1, m_x2, m_y2);
  }
}
