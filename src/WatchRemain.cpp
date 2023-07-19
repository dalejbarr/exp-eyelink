#include "WatchRemain.hpp"
#include "Template.hpp"
#include "Experiment.hpp"
#include "CmdID.hpp"

WatchRemain::WatchRemain(long idWatch, long idNext, ArgMMap mmap) : 
  Watch(idWatch, idNext) {

  g_pErr->DFI("WatchRemain::WatchRemain", GetID(), 3);

	m_timerID = NULL;
	m_msWait = 0;

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  pii = mmap.equal_range("Total_Msec");
  if (pii.first == pii.second) {
    // no event defined
  } else {
    from_string<Uint32>(m_msWait, (pii.first)->second, std::dec);
  }

  g_pErr->DFO("WatchRemain::WatchRemain", GetID(), 3);
}

Uint32 WatchRemain::PostTimeout(Uint32 interval, void * param) {
	SDL_Event event;
  SDL_UserEvent userevent;

	userevent.type = SDL_USEREVENT;
	userevent.code = SBX_WATCH_REMAIN;
	userevent.data1 = (WatchRemain *) param;
	userevent.data2 = NULL;

	event.type = SDL_USEREVENT;
	event.user = userevent;
	SDL_PushEvent(&event);

	return 0;
}

int WatchRemain::Activate() {
	Uint32 msRemain = 0;
  g_pErr->DFI("WatchRemain::Activate", GetID(), 3);
	
	Uint32 msBegin = Template::g_pExperiment->GetTrialBegin();
	Uint32 msCurrent = ClockFn();
	g_pErr->Debug(pastestr::paste("ddds", " ", (long) msBegin, (long) msCurrent,
																(long) m_msWait,
																"trial begin vs current clock vs wait"));
	if ((msCurrent - msBegin) > m_msWait) {
		// stop immediately
		g_pErr->Debug("stop immediately");
		WatchRemain::PostTimeout(0, this);
	} else {
		msRemain = m_msWait - (msCurrent - msBegin);
		g_pErr->Debug(pastestr::paste("sd", " ", "wait for", (long) msRemain));
		m_timerID = SDL_AddTimer(msRemain, WatchRemain::PostTimeout, this);
	}

  g_pErr->DFO("WatchRemain::Activate", GetID(), 3);

	return 0;
}

bool WatchRemain::CheckCondition(SDL_Event * pEvt) {
	bool bResult = false;

  g_pErr->DFI("WatchRemain::CheckCondition", GetID(), 3);
 
	WatchRemain * p_wr = (WatchRemain *) pEvt->user.data1;
	if (p_wr->GetID() == this->GetID()) {
		bResult = true;
	}
  
  g_pErr->DFO("WatchRemain::CheckCondition", GetID(), 3);

  return bResult;
}
