#include "WatchEyeLinkAOI.hpp"
#include "CmdID.hpp"
#include "StimulusImg.hpp"
#include "pastestr.hpp"

EyeLinkMonitor * WatchEyeLinkAOI::m_pEyelink = NULL;

WatchEyeLinkAOI::WatchEyeLinkAOI(long id,
				 long idNext,
				 ArgMMap mmap,
				 StimulusPtrMMap * pmmapAOI) :
  Watch(id, idNext) {

  m_pmmapAOI = pmmapAOI;
  
  pair<ArgIter, ArgIter> pii = mmap.equal_range("AOI");
  ArgIter ii;

  if (pii.first != pii.second) {
    for (ii = pii.first; ii != pii.second; ii++) {
      m_vAOI.emplace_back((*ii).second);
    }
  } else {
    g_pErr->Report(
	pastestr::paste("sds", " ",
			"WatchEyeLinkAOI requires AOI argument (watch ID",
			(long) GetID(), ")"));
  }

}

int WatchEyeLinkAOI::Activate() {
  g_pErr->DFI("WatchEyeLinkAOI::Activate", "", 2);
  StimulusImg * pi = NULL;
  pair<StimulusPtrMMapIter, StimulusPtrMMapIter> pii;
  StimulusPtrMMap::iterator ii;
	  
  if (WatchEyeLinkAOI::m_pEyelink) {
    // add in the AOIs that we're monitoring
    for (std::size_t i = 0; i < m_vAOI.size(); i++) {
      pii = (*m_pmmapAOI).equal_range(m_vAOI[i]);
      for (ii = pii.first; ii != pii.second; ii++) {
	pi = (StimulusImg *) (*ii).second.get();
	WatchEyeLinkAOI::m_pEyelink->RegisterAOI(pi->m_x1, pi->m_y1,
						 pi->m_x2, pi->m_y2,
						 GetID());
      }
    }
    
    WatchEyeLinkAOI::m_pEyelink->StartMonitoring();
  }
  
  g_pErr->DFO("WatchEyeLinkAOI::Activate", "", 2);
  return 0;
}

int WatchEyeLinkAOI::Finish() {
  g_pErr->DFI("WatchEyeLinkAOI::Finish", "", 2);

  if (WatchEyeLinkAOI::m_pEyelink) {
    WatchEyeLinkAOI::m_pEyelink->StopMonitoring();
    WatchEyeLinkAOI::m_pEyelink->ClearAOIs();
  }
  
  g_pErr->DFO("WatchEyeLinkAOI::Finish", "", 2);
  return 0;
}

bool WatchEyeLinkAOI::CheckCondition(SDL_Event * pEvt) {
  bool bResult = false;

  if (pEvt->type == SDL_USEREVENT) {
    if (pEvt->user.code == SBX_WATCH_EYELINKAOI) {
      // TODO check whether it is in my AOI
      long * pl = (long *) pEvt->user.data1;
      if (*pl == GetID()) bResult = true;
    }
  }

  if (bResult) {
    g_pErr->Debug(pastestr::paste("sds", " ",
				  "eyelink watch", (long) GetID(),
				  "triggered"));
  }
  
  return bResult;
}
