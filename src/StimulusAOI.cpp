#include "StimulusAOI.hpp"
#include "Experiment.hpp"
#include "global.hpp"

StimulusAOI::StimulusAOI() {
  g_pErr->DFI("StimulusAOI::StimulusAOI", (const char *) NULL, 4);

  m_x1 = m_y1 = m_x2 = m_y2 = 0;

  g_pErr->DFO("StimulusAOI::StimulusAOI", (const char *) NULL, 4);
}

StimulusAOI::~StimulusAOI() {
}

int StimulusAOI::Cleanup(void) {
  return 0;
}
