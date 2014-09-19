#include "StimulusAOISet.hpp"
#include "Experiment.hpp"
#include "global.hpp"
#include <vector>

StimulusAOISet::StimulusAOISet(const char * pc, Template * pTemplate) {

  std::vector<long> vecIDs;

  g_prsStim->Exec(pc);

  StimulusAOI * pStimAOI = NULL;
  StimulusAOIPtr ptrStimAOI;

  while (g_prsStim->RowReady()) {

    pStimAOI = (StimulusAOI *) 
      Experiment::FindOrCreateStimulus(EXP_STIMULUS_AOI, pTemplate);

    if (pStimAOI) {
      ptrStimAOI = StimulusAOIPtr(pStimAOI);
      m_setStimAOI.insert(ptrStimAOI);
    } else {}

    //m_setStimAOI.insert();
    g_prsStim->MoveNext();
  }
  
}

StimulusAOISet::~StimulusAOISet() {
  Cleanup();
}

int StimulusAOISet::Cleanup() {
  m_setStimAOI.clear();

  return 0;
}
