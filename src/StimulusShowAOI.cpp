#include "StimulusShowAOI.hpp"
#include "StimulusImg.hpp"
#include "Template.hpp"

StimulusShowAOI::StimulusShowAOI(long id, long msec, long idCmd, ArgMMap mmArgs, 
			   Template * pTemplate) : 
  Stimulus(id, pTemplate) {
  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("AOI");
  ArgIter ii;

  if (pii.first == pii.second) {
    // THROW UP AN ERROR
    g_pErr->Report("no arguments given to SHOW_AOI");
  } else {}

  for (ii = pii.first; ii != pii.second; ii++) {
    m_vsShow.push_back( (*ii).second );
  }
}

StimulusShowAOI::~StimulusShowAOI() {
  m_vsShow.clear();
}

int StimulusShowAOI::Action() {
  string s1;

  for (int i = 0; i < m_vsShow.size(); i++) {
    s1.assign(Stimulus::GetResourceString(m_vsShow[i]));
    pair<StimulusPtrMMap::iterator,StimulusPtrMMap::iterator> pii;
    StimulusPtrMMap::iterator ii;
    StimulusImg * pStim = NULL;

    pii = m_pTemplate->m_mmapAllAOI.equal_range(s1.c_str());
    if (pii.first == pii.second) {
      g_pErr->Report(pastestr::paste("ss", " ", "couldn't find AOI named", s1.c_str()));
    } else {
      for (ii = pii.first; ii != pii.second; ii++) {
	pStim = (StimulusImg *) ii->second.get();      
      //g_pErr->Report(pastestr::paste("s", " ", pStim->
	pStim->Action();
      }
    }
  }

  return 0;
}
