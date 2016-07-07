#include <iostream>
#include <cstddef>
#include <cstdlib>
#include <sstream>
#include <boost/lexical_cast.hpp>
using std::ostringstream;

#include "pastestr.hpp"
using pastestr::paste;

#include "State.hpp"
#include "global.hpp"
#include "Stimulus.hpp"
#include "Experiment.hpp"
#include "CmdID.hpp"
#include "StimulusDisplayOn.hpp"
#include "StimulusWav.hpp"
#include "StimulusEyeLinkMsg.hpp"
#include "StimulusTxt.hpp"
#include "StimulusWebcam.hpp"
#include "EventGrabAOI.hpp"
#include "EventSwapAOI.hpp"
#include "EventUpdateAOI.hpp"
#include "EventUpdate.hpp"
#include "EventRecord.hpp"
#include "EventSTRecord.hpp"
#include "EventMsg.hpp"
#include "EventAsync.hpp"
#include "EventAsyncCtrl.hpp"
#include "EventClear.hpp"
#include "EventRepeatIf.hpp"
#include "EventResetCounter.hpp"
#include "EventIncrementCounter.hpp"
#include "EventSocketSendMsg.hpp"

//#include "EventGSC1Feedback.hpp"
//#include "EventGSC1DrawGrid.hpp"
//#include "EventLC1Display.hpp"
#include "StimulusShowAOI.hpp"

#include "WatchKey.hpp"
#include "WatchMouse.hpp"
#include "WatchMouseMove.hpp"
#include "WatchDone.hpp"
#include "WatchGamePadButton.hpp"
#include "WatchGamePadMove.hpp"
#include "WatchSocketMsg.hpp"
#include "WatchRemain.hpp"
//#include "boost/lexical_cast.hpp"

//#include "WatchGSC1Button.hpp"
//#include "WatchGSC1Move.hpp"
//#include "WatchGSC1SoundButton.hpp"
//#include "WatchGSC1ToggleButton.hpp"

#include "Template.hpp"

OpInt State::s_nMouseCurX(0);
OpInt State::s_nMouseCurY(0);

bool State::s_bFinished = false;
bool State::s_bTimedOut = false;
bool State::s_bContinue = false;

SDL_Thread * State::s_pThread = NULL;

State::State(long id, const char * pcName, long seq) {

  m_nTimeout = 0;
  m_id = id;
  m_strName.assign(pcName);
  m_lSeq = seq;
  m_vMsBegin.clear();
  ostringstream ostr;
  ostr << m_strName << " (" << m_id << ")";
  m_strDebug = ostr.str();

  g_pErr->DFI("State::State", m_strDebug.c_str(), 3);
  m_pEvtMove = NULL;
  LockWatches();
  g_pErr->DFO("State::State", m_strDebug.c_str(), 3);
}

int State::Cleanup() {

  m_mmapEvent.clear();
  m_mmapWatch.clear();
  LockWatches();
  //g_pErr->Debug("here");
  //m_pEvtMove = NULL;

  return 0;
}

State::~State() {
  g_pErr->DFI("State::~State", m_strDebug.c_str(), 3);

  Cleanup();

  g_pErr->DFO("State::~State", m_strDebug.c_str(), 3);;
}

int State::Load(Template * pTemplate) {

  LoadEvents(pTemplate);
  LoadWatches(pTemplate);

  return 0;
}

int State::LoadEvents(Template * pTemplate) {
  g_pErr->DFI("LoadEvents", m_strDebug.c_str(), 3);

  StimulusPtr pStim;
  //Stimulus * pStimRaw = NULL;
  EventPtr pEvent;
  long idEvent = 0;
  long idCmd = 0;
  long msec = 0;
  dbData d;
  ArgMMap mmArgs;
  //EventPtr pGSC1DrawGrid;
  //EventPtr pGSC1Feedback;

  string q = pastestr::paste("sds", "", "\
SELECT EventID, EvtCmdID, Msec \n\
FROM Event \n\
WHERE StateID=", m_id, "\n\
ORDER BY Msec ASC");

  d = g_prsStim->Load(q);

  // TO DO: Load arguments
  for (int i = 0; i < d.size(); i++) {

    idEvent = atol(d[i][0].c_str());
    idCmd = atol(d[i][1].c_str());
    msec = atol(d[i][2].c_str());
    mmArgs = GetArgs("Event", idEvent);
    g_pErr->Debug(pastestr::paste("sd", " ", "the EvtCmdID was", idCmd));
    pStim.reset();

    // handle "stimulus" events
    switch (idCmd) {
    case SBX_EVENT_SHOW_BITMAP :
      pStim = StimulusPtr(new StimulusImg(0, pTemplate, idCmd, mmArgs));
      break;
    case SBX_EVENT_DISPLAY_ON :
      //pStim = StimulusPtr(new StimulusDisplayOn(0));
      pStim = StimulusPtr(new StimulusDisplayOn(0, pTemplate));
      break;
    case SBX_EVENT_PLAY_SOUND :
      pStim = StimulusPtr(new StimulusWav(0, pTemplate, idCmd, mmArgs));
      break;
    case SBX_EVENT_EYELINK_PRINTMSG :
      pStim = StimulusPtr(new StimulusEyeLinkMsg(0, pTemplate, idCmd, mmArgs));
      break;
    case SBX_EVENT_SHOWTEXT :
      pStim = StimulusPtr(new StimulusTxt(0, pTemplate, idCmd, mmArgs));
      break;
    case SBX_EVENT_SHOW_AOI :
      pStim = StimulusPtr(new StimulusShowAOI(idEvent, msec, idCmd, mmArgs, pTemplate));
      break;
		case SBX_EVENT_WEBCAM_START :
			if (!Experiment::s_pCam) {
				g_pErr->Debug("creating new webcam instance...");
				string strDev;
				if (!g_pConfig->GetConfig("Video_Device", &strDev)) {
					strDev.assign("/dev/video0");
				}
				int w, h;
				string str1;
				if (g_pConfig->GetConfig("Webcam_Width", &str1)) {
					w = boost::lexical_cast<int>(str1.c_str());
				} else {
					w = -1;
				}
				if (g_pConfig->GetConfig("Webcam_Height", &str1)) {
					h = boost::lexical_cast<int>(str1.c_str());
				} else {
					h = -1;
				}
				Experiment::s_pCam = new Webcam2(strDev.c_str(), w, h);
			} 
			pStim = StimulusPtr(new StimulusWebcam(idEvent, mmArgs, pTemplate, Experiment::s_pCam));
			break;
    }

    if (pStim.get()) {
      //pStim = StimulusPtr(pStimRaw);
      pEvent = EventPtr(new Event(idEvent, msec, pStim));
      //delete pStimRaw;
    } else {
      // now handle other kinds of events, self-instantiating
      switch (idCmd) { // post-process
      case SBX_EVENT_CLEARREGION :
				pEvent = EventPtr(new EventClear(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
      case SBX_EVENT_UPDATE :
				pEvent = EventPtr(new EventUpdate(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
      case SBX_EVENT_GRAB_AOI :
				m_pEvtMove = new EventGrabAOI(idEvent, msec, idCmd, mmArgs, pTemplate);
				pEvent = EventPtr(m_pEvtMove);
				//m_pEvtMove = pEvent.get();
				break;
      case SBX_EVENT_SWAP_AOI :
				pEvent = EventPtr(new EventSwapAOI(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
      case SBX_EVENT_UPDATE_AOI :
				//pEvent = EventPtr(new Event(idEvent, msec, idCmd, mmArgs, pTemplate));
				pEvent = EventPtr(new EventUpdateAOI(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
      case SBX_EVENT_MOUSE_REC :
				{
					InputDevPtr pDev = pTemplate->FindOrCreateInputDev(SBX_MOUSE_DEV);
					pEvent = EventPtr(new EventRecord(idEvent, msec, idCmd, mmArgs, pTemplate, pDev));
				}
				break;
      case SBX_EVENT_SCROLLTRACKGP :
				{
					// figure out which one
					pair<ArgIter, ArgIter> pii;
					ArgMMap::iterator ii;	  
					int ix = 0;
					pii = mmArgs.equal_range("Index");
					//vector<InputDevPtr> vIDP;
					InputDevPtr pDev;
					if (pii.first != pii.second) {
						ii = pii.first;
						ix = atoi((*ii).second.c_str());
						g_pErr->Debug(pastestr::paste("sd"," ","index is",(long) ix));
						pDev = pTemplate->FindOrCreateInputDev(SBX_SCROLLTRACKGP_DEV, ix);
					} else {
						g_pErr->Report("Event PERIDISP requires Argument 'Index'");
					}	  
					//((GamePad_SDL *) pDev.get())->SetRecMode(true);
					pEvent = EventPtr(new EventSTRecord(idEvent, msec, idCmd, mmArgs, pTemplate, pDev));
				}
				break;
      case SBX_EVENT_GAMEPADREC :
				{
					// figure out which one
					pair<ArgIter, ArgIter> pii;
					ArgMMap::iterator ii;	  
					int ix = 0;
					pii = mmArgs.equal_range("Index");
					vector<InputDevPtr> vIDP;
					InputDevPtr pDev;
					if (pii.first != pii.second) {
						for (ii = pii.first; ii != pii.second; ii++) {
							ix = atoi((*ii).second.c_str());
							g_pErr->Debug(pastestr::paste("sd"," ","index is",(long) ix));
							pDev = pTemplate->FindOrCreateInputDev(SBX_GAMEPAD_DEV, ix);
							vIDP.push_back(pDev);
						}
					} else {
						g_pErr->Report("Event GAMEPADREC requires Argument 'Index'");
					}	  
					//((GamePad_SDL *) pDev.get())->SetRecMode(true);
					pEvent = EventPtr(new EventRecord(idEvent, msec, idCmd, mmArgs, pTemplate, vIDP));
				}
				break;
      case SBX_EVENT_MSG :
				pEvent = EventPtr(new EventMsg(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
				//case SBX_EVENT_GSC1FEEDBACK :
				//pEvent = EventPtr(new EventGSC1Feedback(idEvent, msec, idCmd, mmArgs, pTemplate, NULL));
				//pGSC1Feedback = pEvent;
				//break;
				//case SBX_EVENT_GSC1DRAWGRID :
				//pEvent = EventPtr(new EventGSC1DrawGrid(idEvent, msec, idCmd, mmArgs, pTemplate));
				//pGSC1DrawGrid = pEvent;
				//break;
      case SBX_EVENT_ASYNCSTART :
				pEvent = EventPtr(new EventAsyncCtrl(idEvent, msec, idCmd, mmArgs, pTemplate, true));
				break;
      case SBX_EVENT_ASYNCSTOP :
				pEvent = EventPtr(new EventAsyncCtrl(idEvent, msec, idCmd, mmArgs, pTemplate, false));
				break;
				//case SBX_EVENT_LC1DISPLAY :
				//pEvent = EventPtr(new EventLC1Display(idEvent, msec, idCmd, mmArgs, pTemplate));
				//break;
      case SBX_EVENT_RECSOUND :
				{
					InputDevPtr pDev = pTemplate->FindOrCreateInputDev(SBX_AUDIOREC_DEV);
					pEvent = EventPtr(new EventRecord(idEvent, msec, idCmd, mmArgs, pTemplate, pDev));
				}
				break;
      case SBX_EVENT_INCREMENT_COUNTER :
				pEvent = EventPtr(new EventIncrementCounter(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
      case SBX_EVENT_RESET_COUNTER :
				pEvent = EventPtr(new EventResetCounter(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
      case SBX_EVENT_REPEATIF :
				pEvent = EventPtr(new EventRepeatIf(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
			case SBX_EVENT_SOCKET_SEND_MSG : {
				// figure out which one
				pair<ArgIter, ArgIter> pii;
				ArgMMap::iterator ii;	  
				unsigned short us = 0;
				pii = mmArgs.equal_range("SocketID");
				InputDevPtr pDev;
				if (pii.first != pii.second) {
						ii = pii.first;
						us = boost::lexical_cast<unsigned short>((*ii).second.c_str());
						g_pErr->Debug(pastestr::paste("sd"," ","socket index is",(long) us));
						pDev = pTemplate->FindOrCreateInputDev(SBX_SOCKET_DEV, (int) us);
				} else {
					g_pErr->Report("Event SOCKET_SEND_MSG requires argument 'SocketID'");
				}  
				pEvent = EventPtr(new EventSocketSendMsg(idEvent, msec, idCmd, 
																								 mmArgs, pTemplate, pDev));
			}
				break;
      default :
				g_pErr->Report(pastestr::paste("sd", " ", "UNDEFINED EVENT!", idCmd));
				pEvent = EventPtr(new Event(idEvent, msec, idCmd, mmArgs, pTemplate));
				break;
      }
    }

    if (pEvent.get()) {
      m_mmapEvent.insert(EventPair(pEvent->Msec(), pEvent));
    } else {
      // TO DO; uncomment this line
      //g_pErr->Report(pastestr::paste("ssssss", " ", "Couldn't find EventCmdID ", d[i][1].c_str(), "\n",
      //d[i][0].c_str(), d[i][1].c_str(), d[i][2].c_str() ));
    }
  }

  g_pErr->DFO("LoadEvents", m_strDebug.c_str(), 3);

  return 0;
}

int State::LoadWatches(Template * pTemplate) {
  g_pErr->DFI("LoadWatches", m_strDebug.c_str(), 3);

  string q = pastestr::paste("sds", "", "\
SELECT WatchID, WCmdID, NextStateID \n\
FROM Watch \n\
WHERE StateID=", m_id, "\n\
ORDER BY WatchID ASC");

  dbData d = g_prsStim->Load(q);
  long idCmd = 0L;
  long idWatch = 0L;
  long idNext = 0L;
  ArgMMap mmArgs;
  WatchPtr pWatch;
  pair<ArgIter, ArgIter> pii;

  for (int i = 0; i < d.size(); i++) {
    idCmd = atol(d[i][1].c_str());
    idWatch = atol(d[i][0].c_str());
    idNext = atol(d[i][2].c_str());

    mmArgs = GetArgs("Watch", idWatch);

    switch (idCmd) {
    case SBX_WATCH_KEYDOWN :
      pWatch = WatchPtr(new WatchKey(idWatch, idNext, mmArgs));
      break;
      // gamepad
    case SBX_WATCH_GAMEPAD_BUTTONDOWN :
      pWatch = WatchPtr(new WatchGamePadButton(idWatch, idNext, mmArgs));
      break;
      //case SBX_WATCH_GSC1BUTTON :
      //pWatch = WatchPtr(new WatchGSC1Button(idWatch, idNext, mmArgs));
      //idCmd = SBX_WATCH_GAMEPAD_BUTTONDOWN;
      //break;
    case SBX_WATCH_GAMEPAD_MOVE :
      pWatch = WatchPtr(new WatchGamePadMove(idWatch, idNext, mmArgs));
      break;
      //case SBX_WATCH_GSC1MOVE :
      //pWatch = WatchPtr(new WatchGSC1Move(idWatch, idNext, mmArgs, pTemplate));
      //idCmd = SBX_WATCH_GAMEPAD_MOVE;
      //break;
      //case SBX_WATCH_GSC1SOUNDBUTTON :
      //pWatch = WatchPtr(new WatchGSC1SoundButton(idWatch, idNext, mmArgs, pTemplate));
      //idCmd = SBX_WATCH_GAMEPAD_BUTTONDOWN;
      //break;
      //case SBX_WATCH_GSC1TOGGLEBUTTON :
      //pWatch = WatchPtr(new WatchGSC1ToggleButton(idWatch, idNext, mmArgs, pTemplate));
      //idCmd = SBX_WATCH_GAMEPAD_BUTTONDOWN;
      //break;
      // mouse
    case SBX_WATCH_MOUSEBUTTON :
      pWatch = WatchPtr(new WatchMouse(idWatch, idNext, mmArgs, 
																			 &(pTemplate->m_mmapAllAOI)));
      break;
    case SBX_WATCH_MOUSEMOVE :
      pWatch = WatchPtr(new WatchMouseMove(idWatch, idNext, mmArgs));
      break;
    case SBX_WATCH_DONE :
      pWatch = WatchPtr(new WatchDone(idWatch, idNext, mmArgs));
      break;
		case SBX_WATCH_REMAIN :
			pWatch = WatchPtr(new WatchRemain(idWatch, idNext, mmArgs));
			break;
    case SBX_WATCH_TIMEOUT :
      pii = mmArgs.equal_range("Msec");
      if (pii.first == pii.second) {
				g_pErr->Report("Argument Msec must be supplied to Watch TIMEOUT");
      } else {
				long msTime = 0;
				from_string<long>(msTime, (*pii.first).second, std::dec);
				m_nTimeout = (Uint32) msTime;
      }
      pWatch = WatchPtr(new Watch(idWatch, idNext));
			break;
		case SBX_WATCH_SOCKET_MSG :
			{
				// figure out which one
				pair<ArgIter, ArgIter> pii;
				ArgMMap::iterator ii;	  
				unsigned short us = 0;
				InputDevPtr pDev;
				pii = mmArgs.equal_range("SocketID");
				if (pii.first != pii.second) {
						ii = pii.first;
						us = boost::lexical_cast<unsigned short>((*ii).second.c_str());
						g_pErr->Debug(pastestr::paste("sd"," ","socket index is",(long) us));
						pDev = pTemplate->FindOrCreateInputDev(SBX_SOCKET_DEV, (int) us);
				} else {
					g_pErr->Report("Watch SOCKET_MSG requires argument 'SocketID'");
				}  
				if (Experiment::s_pSockListener == NULL) {
					Experiment::s_pSockListener = new SocketListener(pDev);
				}
				pWatch = WatchPtr(new WatchSocketMsg(idWatch, idNext, mmArgs));
			}
			break;
    }

    if (pWatch.get()) {
      m_mmapWatch.insert(WatchPair(idCmd, pWatch));
    } else {};
  }

  g_pErr->DFO("LoadWatches", m_strDebug.c_str(), 3);
  return 0;
}

ArgMMap State::GetArgs(string strTable, long id) {
  g_pErr->DFI("State::GetArgs", m_strDebug.c_str(), 4);
  ArgMMap r;
  ostringstream ostr;
  dbData d;

  ostr << "SELECT ArgID, Info \nFROM " << strTable << "Args\nWHERE " << strTable << "ID=" << id;
  d = g_prsStim->Load(ostr.str());

  for (int i = 0; i < d.size(); i++) {
    r.insert(pair<string, string>(d[i][0], d[i][1]));
  }

  g_pErr->DFO("State::GetArgs", m_strDebug.c_str(), 4);

  return r;
}

int State::Prepare() {
  // prepare events
  g_pErr->DFI("State::Prepare", m_strDebug.c_str(), 2);
  //m_bVisited = 0;

  EventMap::iterator ei;
  for (ei = m_mmapEvent.begin(); ei != m_mmapEvent.end(); ei++) {
    (*ei).second->Prepare();
  }

  // prepare watches
  WatchMap::iterator wi;
  for (wi = m_mmapWatch.begin(); wi != m_mmapWatch.end(); wi++) {
    (*wi).second->Prepare();
  }

  m_vMsBegin.clear();

  LockWatches();
  g_pErr->DFO("State::Prepare", m_strDebug.c_str(), 2);

  return 0;
}

int State::Finish() {
  g_pErr->DFI("State::Finish", m_strDebug.c_str(), 5);

	State::s_bContinue = false;
	int nStatus;
	SDL_WaitThread(s_pThread, &nStatus);
	State::s_pThread = NULL;
  // finish events
  EventMap::iterator ei;
  for (ei = m_mmapEvent.begin(); ei != m_mmapEvent.end(); ei++) {
    (*ei).second->Finish();
  }

  // finish watches
  WatchMap::iterator wi;
  for (wi = m_mmapWatch.begin(); wi != m_mmapWatch.end(); wi++) {
    (*wi).second->Finish();
  }

  LockWatches();

  g_pErr->DFO("State::Finish", m_strDebug.c_str(), 5);
}

int State::main(void * pVoid) { // static
	State * pThis = (State *) pVoid;
	pThis->Main();
}

int State::Start() {
  g_pErr->DFI("State::Start", m_strDebug.c_str(), 4);

	State::s_bFinished = false;
	State::s_bTimedOut = false;
	State::s_bContinue = true;

  //m_bVisited = 1;
  m_pCurEvent = m_mmapEvent.begin();
  m_vMsBegin.push_back(ClockFn());
	if (s_pThread) {
		g_pErr->Report("thread already running!");
	}
	State::s_pThread = SDL_CreateThread(State::main, this);
  //Update();
  UnlockWatches();

  // TO DO: do something about timeouts
  g_pErr->DFO("State::Start", m_strDebug.c_str(), 4);  
  return 0;
}

int State::Main() {
  g_pErr->DFI("State::Main", m_strDebug.c_str(), 4);  

  static Event * pEvent = NULL;
  static SDL_Event sdlEvent;  
  static SDL_Event sdlEventDone;  
  static Uint32 msDiff = 0;
  static SDL_Event event;
  static SDL_UserEvent userevent;
	bool bExit = false;
  WatchMap::iterator wi;

  for (wi = m_mmapWatch.begin(); wi != m_mmapWatch.end(); wi++) {
    (*wi).second->Activate();
  }

  while (State::s_bContinue) {
		msDiff = ClockFn() - m_vMsBegin.back();
		if (m_nTimeout && !State::s_bTimedOut) {
			if (msDiff >= m_nTimeout) {
				State::s_bTimedOut = true;
				State::s_bFinished = true; // don't run any more events
				//g_pErr->Report(pastestr::paste("sd", " ", "state timeout", m_id));
				m_pCurEvent = m_mmapEvent.end(); // make sure no more events will be triggered
				userevent.type = SDL_USEREVENT;
				userevent.code = SBX_WATCH_TIMEOUT;
				userevent.data1 = NULL;
				userevent.data2 = NULL;
				event.type = SDL_USEREVENT;
				event.user = userevent;
				SDL_PushEvent(&event);
				bExit = true;
			} else {}
		} else {}

		if (!State::s_bFinished) {
			pEvent = (*m_pCurEvent).second.get();
			if (pEvent) {
				if ((msDiff > pEvent->Msec()) || ((msDiff - pEvent->Msec()) < EXP_EVENT_TIMER_RESOLUTION)) {
					//g_pErr->Debug(pastestr::paste("dd", ":", (long) pEvent->Msec(), (long) msDiff));
					pEvent->Action();
					m_pCurEvent++;
					if (m_pCurEvent == m_mmapEvent.end()) { // that was last event
						State::s_bFinished = true;
						pEvent = NULL;
						sdlEventDone.type=SDL_USEREVENT;
						sdlEventDone.user.code=SBX_WATCH_DONE;
						sdlEventDone.user.data1=NULL;
						sdlEventDone.user.data2=NULL;
						SDL_PushEvent(&sdlEventDone);
						if ((!m_nTimeout) || s_bTimedOut) {
							bExit = true;
						}
					}
				}
			} else {
				State::s_bFinished = true; // no events to process
			}
		}
		SDL_Delay(2);
	}

  g_pErr->DFO("State::Main", m_strDebug.c_str(), 4);  

  return State::s_bFinished;
}

int State::Run() {
  g_pErr->DFI("State::Run", m_strDebug.c_str(), 2);

  Uint32 msBegin = ClockFn();
  Uint32 msNow = msBegin;
  Event * pEvent = NULL;
  SDL_Event sdlEvent;

  if (m_nTimeout) {
    // note: not clear if this works
    SDL_AddTimer(m_nTimeout, timeoutFn, NULL);
  } else {}

  EventMap::iterator ei;
  for (ei = m_mmapEvent.begin(); ei != m_mmapEvent.end(); ei++) {
    pEvent = (*ei).second.get();
    // TO DO: monitor a variable "quit" so that the trial is terminated when necessary
    while ((msNow - msBegin) < ((Uint32) pEvent->Msec())) {
      SDL_Delay(1);
      msNow = SDL_GetTicks();
    }
    pEvent->Action();
  }

  sdlEvent.type=SDL_USEREVENT;
  sdlEvent.user.code=SBX_WATCH_DONE;
  sdlEvent.user.data1=NULL;
  sdlEvent.user.data2=NULL;

  UnlockWatches();
  SDL_PushEvent(&sdlEvent);
  //g_pErr->Debug(pastestr::paste("sss", "", "done with event loop (StateID=",
  //m_strDebug.c_str(), ")"));

  g_pErr->DFO("State::Run", m_strDebug.c_str(), 2);  
  return 0;
}

Watch * State::HandleEvent(SDL_Event * pEvt, Template * pThis) {
  WatchMapIter wmip;
  WatchMap::iterator wmi;
  Watch * pwSignaled = NULL;
  Watch * pw1 = NULL;

  if (WatchesLocked()) {
    return NULL;
  } else {}
  
  switch (pEvt->type) {

    // KEYDOWN watch
    // handle the watch for specific keys first, then default to ANYKEY.
  case SDL_KEYDOWN : {
    WatchKey * pwk;
    g_pErr->Debug("~-~-~-~-~ key down ~-~-~-~-~");
    wmip = m_mmapWatch.equal_range(SBX_WATCH_KEYDOWN);
    if (wmip.first != wmip.second) {
      for (wmi = wmip.first; wmi != wmip.second; wmi++) {
				pwk = (WatchKey *) (*wmi).second.get();
				if (pwk->GetKey() == -1) {
					pw1 = (Watch *) pwk;
				} else {
					if (pwk->GetKey() == pEvt->key.keysym.sym) {
						pwSignaled = (Watch *) pwk;
					} else {}
				}
      }
      if (!pwSignaled) {
				if (pw1) {
					g_pErr->Debug(pastestr::paste("sds", "", 
																				"ANYKEY signaled (WatchID=", 
																				pw1->GetID(), ")"));
				} else {}
				pwSignaled = pw1;
      } else {
				g_pErr->Debug(pastestr::paste("sds", "", "key signaled (WatchID=", 
																			pwSignaled->GetID(), ")"));
      }
    }
	}
    break;
		
  case SDL_KEYUP :
    g_pErr->Debug("~-~-~-~-~ key up ~-~-~-~-~");
    break;

    // MOUSEBUTTON watch
    // handle the watch for specific keys first, then default to ANYKEY.
  case SDL_USEREVENT : {
    //g_pErr->Debug(pastestr::paste("sd", " ", "the user event was", 
		//(long) pEvt->user.code));
		g_pErr->Debug(pastestr::paste("sdd", " ", "... user event", (long) pEvt->user.code,
																	(long) ClockFn()));
    switch (pEvt->user.code) {
    case SBX_WATCH_DONE : {
			// FIRST, make sure we didn't miss any socket signals between trials
			if (Experiment::s_pSockListener != NULL) {
				Experiment::s_pSockListener->CheckForMissedMessages();
			}

      wmip = m_mmapWatch.equal_range(SBX_WATCH_DONE);
      if (wmip.first != wmip.second) {
				for (wmi = wmip.first; wmi != wmip.second; wmi++) {
					if ((*wmi).second->CheckCondition(pEvt)) {
						pwSignaled = (*wmi).second.get();
						g_pErr->Debug("done signaled");
					} else {}
				}
      } else {}
		}
      break;
    case SBX_WATCH_TIMEOUT : {
      wmip = m_mmapWatch.equal_range(SBX_WATCH_TIMEOUT);
      pwSignaled = (*wmip.first).second.get();
      g_pErr->Debug("timeout signaled");
		}
      break;
		case SBX_WATCH_SOCKET_MSG : {
      wmip = m_mmapWatch.equal_range(SBX_WATCH_SOCKET_MSG);
			for (wmi = wmip.first; wmi != wmip.second; wmi++) {
				if ((*wmi).second->CheckCondition(pEvt)) {
					pwSignaled = (*wmi).second.get();
				} else {}
			}
			// TODO: do something
		}
			break;
		case SBX_WATCH_REMAIN : {
      wmip = m_mmapWatch.equal_range(SBX_WATCH_REMAIN);
      if (wmip.first != wmip.second) {
				for (wmi = wmip.first; wmi != wmip.second; wmi++) {
					if ((*wmi).second->CheckCondition(pEvt)) {
						pwSignaled = (*wmi).second.get();
					}
				}
			}
		}
			break;
    }
	}
    break;

    // gamepad
  case SDL_JOYAXISMOTION : {
    wmip = m_mmapWatch.equal_range(SBX_WATCH_GAMEPAD_MOVE);
    for (wmi = wmip.first; wmi != wmip.second; wmi++) {
      if ((*wmi).second->CheckCondition(pEvt)) {
				pwSignaled = (*wmi).second.get();
      } else {}
    }        
	}
    break;

  case SDL_JOYBUTTONDOWN :
    {
      // TODO: find the associated GamePad device, and process the event.
      SDL_JoyButtonEvent * pJEvt = (SDL_JoyButtonEvent *) pEvt;
      GamePad_SDL * pGamePad = 
				(GamePad_SDL *) (pThis->GetDevice(SBX_GAMEPAD_DEV,
																					pJEvt->which)).get();
      if (!pGamePad) {
      } else {
      }
      wmip = m_mmapWatch.equal_range(SBX_WATCH_GAMEPAD_BUTTONDOWN);
      for (wmi = wmip.first; wmi != wmip.second; wmi++) {
				if ((*wmi).second->CheckCondition(pEvt)) {
					pwSignaled = (*wmi).second.get();
				} else {}
      }
    }
    break;

    // mouse
  case SDL_MOUSEBUTTONDOWN :
    pwSignaled = ProcessMouseButton(pEvt);
    break;

  case SDL_MOUSEBUTTONUP :
    pwSignaled = ProcessMouseButton(pEvt);

  case SDL_MOUSEMOTION :
	  // InputDevPtr pDev = pTemplate->FindOrCreateInputDev(SBX_MOUSE_DEV);
		/*
		// truncate values if limits are set and cursor goes beyond them
		if ( ! ((s_xLim0==0) && (s_xLim1==0) && (s_yLim0==0) && (s_yLim1==0)) ) { // limits are set
			g_pErr->Debug("checking limits");
			if ((m_xLast < s_xLim0) || (m_xLast > s_yLim1)) {
				g_pErr->Debug("out of x bounds");
			} else {}
			if ((m_xLast < s_yLim0) || (m_xLast > s_yLim1)) {
				g_pErr->Debug("out of y bounds");
			} else {}
			m_xLast = (m_xLast<=s_xLim0) * s_xLim0 + 
				((m_xLast>s_xLim0) && (m_xLast<s_xLim1)) * m_xLast +
				(m_xLast>=s_xLim1) * s_xLim1;
			m_yLast = (m_yLast<=s_yLim0) * s_yLim0 + 
				((m_yLast>s_yLim0) && (m_yLast<s_yLim1)) * m_yLast +
				(m_yLast>=s_yLim1) * s_yLim1;				
		} else { // limits unset
			g_pErr->Debug(pastestr::paste("sdddd", " ", ".limits",
																		(long) s_xLim0, (long) s_yLim0, (long) s_xLim1, (long) s_yLim1));
		}
		*/
    State::s_nMouseCurX.Set(pEvt->motion.x);
    State::s_nMouseCurY.Set(pEvt->motion.y);

    WatchMouseMove * pmm;
    wmip = m_mmapWatch.equal_range(SBX_WATCH_MOUSEMOVE);
    for (wmi = wmip.first; wmi != wmip.second; wmi++) {
      if ((*wmi).second->CheckCondition()) {
				pwSignaled = (*wmi).second.get();
      } else {}
    }
		
    if (m_pEvtMove) {
      ((EventGrabAOI *) m_pEvtMove)->Update(pEvt->motion.x, pEvt->motion.y);
    } else {}

    break;
  }

  if (pwSignaled) {
    pwSignaled->Signal();
    Finish();
  } else {}

  return (pwSignaled);
}

Watch * State::ProcessMouseButton(SDL_Event * pEvt) {
  g_pErr->DFI("State::ProcessMouseButton", m_strDebug.c_str(), 2);
  Watch * pwSignaled = NULL;
  WatchMouse * pwm = NULL;
  WatchMapIter wmip;
  WatchMap::iterator wmi;
  SDL_MouseButtonEvent * pMEvt = (SDL_MouseButtonEvent *) pEvt;

  vector<WatchMouse *> vpwmSignaled;

  // if multiple watches win, and the multiple watches are AOI,
  // then take the ones on the highest layers.

  wmip = m_mmapWatch.equal_range(SBX_WATCH_MOUSEBUTTON);
  if (wmip.first != wmip.second) {
    for (wmi = wmip.first; wmi != wmip.second; wmi++) {
      pwm = (WatchMouse *) (*wmi).second.get();
      if (pwm->CheckCondition(pEvt)) {
	if (pwm->GetButton() == EXP_ANYMOUSEBUTTON) {
	  g_pErr->Debug(pastestr::paste("sds", "", "ANYBUTTON signaled (WatchID=", pwm->GetID(), ")"));
	} else {
	  g_pErr->Debug(pastestr::paste("sds", "", "mouse button signaled (WatchID=", pwm->GetID(), ")"));
	}
	vpwmSignaled.push_back(pwm);
      } else {}
    }
  }

  if (vpwmSignaled.size() > 1) {
    int nTopPriority = -1;
    int nWinner = 0;
    int nTopLayer = -1;
    StimulusImg * pAOI = NULL;
    for (int i = 0; i < vpwmSignaled.size(); i++) {
      pwm = vpwmSignaled[i];
      if (pwm->GetRegion() > nTopPriority) {
	nTopPriority = pwm->GetRegion();
	if (pwm->GetRegion() == WATCHMOUSE_AOI) {
	  if (pwm->m_pSelectedAOI) {
	    pAOI = (StimulusImg *) pwm->m_pSelectedAOI.get();
	    if (pAOI->m_nLayer > nTopLayer) {
	      nWinner = i;
	      nTopLayer = pAOI->m_nLayer;
	    } else {}
	  } else {
	    g_pErr->Report(pastestr::paste("sds", " ", "WatchID", pwm->GetID(), "was signaled, but m_pSelectedAOI was null!!"));
	  }
	} else {
	  nWinner = i;
	}
      } else {}
    }
    if (nTopPriority == WATCHMOUSE_AOI) { // reset selected AOI to null for unselected watches
      for (int i = 0; i < vpwmSignaled.size(); i++) {
	if (i != nWinner) {
	  vpwmSignaled[i]->ResetSelection();
	} else {}
      }
    } else {}
    pwm = vpwmSignaled[nWinner];
  } else if (vpwmSignaled.size() > 0) {
    pwm = vpwmSignaled[0];
  } else {
    pwm = NULL;
  }

  pwSignaled = (Watch *) pwm;

  g_pErr->DFO("State::ProcessMouseButton", m_strDebug.c_str(), 2);
  return pwSignaled;
}

Uint32 State::timeoutFn(Uint32 interval, void * param) {
  SDL_Event event;
  SDL_UserEvent userevent;
 
  userevent.type = SDL_USEREVENT;
  userevent.code = SBX_WATCH_TIMEOUT;
  userevent.data1 = NULL;
  userevent.data2 = NULL;
 
  event.type = SDL_USEREVENT;
  event.user = userevent;
 
  SDL_PushEvent(&event);

  return (0);
}

vector<EventTime> GetEventTiming() {
  vector<EventTime> v;
  return v;
}

int State::FinalSweep(Template * pTemplate) {

  EventMap::iterator ei;
  for (ei = m_mmapEvent.begin(); ei != m_mmapEvent.end(); ei++) {
    (*ei).second->FinalSweep(pTemplate);
  }

  return 0;
}

EventPtr State::FindEvent(long id) {
  EventMap::iterator ei;
  EventPtr pEvent;
  long idCurrent;

  pEvent.reset();

  for (ei = m_mmapEvent.begin(); ei != m_mmapEvent.end(); ei++) {
    if ((*ei).second->ID() == id) {
      pEvent = (*ei).second;
      break;
    } else {}
  }

  return pEvent;
}

EventPtr State::FindEventByCmdID(long idCmd) {
  EventMap::iterator ei;
  EventPtr pEvent;

  pEvent.reset();

  for (ei = m_mmapEvent.begin(); ei != m_mmapEvent.end(); ei++) {
    if ((*ei).second->CmdID() == idCmd) {
      pEvent = (*ei).second;
      break;
    } else {}
  }

  return pEvent;
}

int State::PostTrial() {
  g_pErr->DFI("State::PostTrial", m_strDebug.c_str(), 5);
  // finish events
  EventMap::iterator ei;
  for (ei = m_mmapEvent.begin(); ei != m_mmapEvent.end(); ei++) {
    (*ei).second->PostTrial();
  }

  // finish watches
  WatchMap::iterator wi;
  for (wi = m_mmapWatch.begin(); wi != m_mmapWatch.end(); wi++) {
    (*wi).second->PostTrial();
  }

  LockWatches();
  g_pErr->DFO("State::Finish", m_strDebug.c_str(), 5);
}
