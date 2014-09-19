#include "Watch.hpp"
#include "global.hpp"

Watch::~Watch() {
  g_pErr->DFI("Watch::~Watch", m_id, 4);
  Cleanup();
  g_pErr->DFO("Watch::~Watch", m_id, 4);
}

int Watch::Cleanup() {
  return 0;
}

int Watch::Finish() {
  return 0;
}

bool Watch::CheckCondition(SDL_Event * pEvt) {
  return false;
}

int Watch::PostTrial() {
  return 0;
}
