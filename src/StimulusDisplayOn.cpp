#include "StimulusDisplayOn.hpp"
#include "StimulusImg.hpp"
#include "Template.hpp"

StimulusDisplayOn::StimulusDisplayOn(long id, Template * pTemplate) : Stimulus(id, pTemplate) {
}

StimulusDisplayOn::~StimulusDisplayOn() {
}

int StimulusDisplayOn::Action() {
  g_pErr->DFI("StimulusDisplayOn::Action", 0L, 2);
  Display_SDL::FlipMemoryToScreen();
  g_pErr->DFO("StimulusDisplayOn::Action", 0L, 2);
  return Stimulus::Action();
}

int StimulusDisplayOn::Prepare() {
  g_pErr->DFI("StimulusDisplayOn::Prepare", 0L, 2);
  Display_SDL::ClearScreen(true);
  m_pTemplate->Redraw(1);
  g_pErr->DFO("StimulusDisplayOn::Prepare", 0L, 2);
}
