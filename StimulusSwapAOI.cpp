#include "StimulusSwapAOI.hpp"
#include "Template.hpp"

StimulusSwapAOI::StimulusSwapAOI(Template * pTemplate, ArgMMap mmArgs) :
  Stimulus(pTemplate, mmArgs)
{
  m_pTemplate = pTemplate;
  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  m_pAOI1.reset();
  m_pAOI2.reset();

  if (mmArgs.count("AOI") != 2) {
    g_pErr->Report("Event SWAP_AOI must have exactly two AOI arguments");
  } else {}

  pii = mmArgs.equal_range("AOI");
  
  m_pAOI1 = AOIPtr(pTemplate->AOIArg(ii->second));
  ii++;
  m_pAOI2 = AOIPtr(pTemplate->AOIArg(ii->second));
}

int StimulusSwapAOI::Prepare() {
  g_pErr->DFI("StimulusSwapAOI::Prepare", 0L, 3);
  g_pErr->DFO("StimulusSwapAOI::Prepare", 0L, 3);

  return 0;
}

int StimulusSwapAOI::Action() {
  g_pErr->DFI("StimulusSwapAOI::Action", 0L, 3);
  g_pErr->DFO("StimulusSwapAOI::Action", 0L, 3);

  return 0;
}

int StimulusSwapAOI::Cleanup() {
  m_pAOI1.reset();
  m_pAOI2.reset();

  return 0;
}
