#include <cstdlib>
#include <sstream>
using std::ostringstream;

#include "Template.hpp"
#include "pastestr.hpp"
using pastestr::paste;

#include "global.hpp"
#include "CmdID.hpp"
#include "WatchKey.hpp"
#include "WatchMouse.hpp"
#include "WatchMouseMove.hpp"
#include "Mouse_SDL.hpp"
#include "SDL.h"
#include "EventAsync.hpp"
#include "ItemCell.hpp"
#include "Trial.hpp"
#include "Experiment.hpp"
#include "GamePad_SDL.hpp"
#include "ScrollTrackGP_SDL.hpp"
#include "Experiment.hpp"
#include "SoundInput.hpp"
#include "Socket.hpp"

#ifndef WIN32
#include "AlsaSoundIn.hpp"
#endif

InputDevPtrMMap Template::s_mmapInputDevPtr;
Experiment * Template::g_pExperiment = NULL;

//extern Uint32 ClockFn();

Template::Template(long id, long timeout, const char * pcName)
{
  m_pItemCell = NULL;
  m_id = id;
  m_strName.assign(pcName);
  ostringstream ostr;
  ostr << m_strName << " (" << m_id << ")";
  m_strDebug = ostr.str();

  g_pErr->DFI("Template::Template", m_strDebug.c_str(), 2);

  m_lgTimeout = timeout;
  m_vEventsAsync.clear();

  m_nStatus = SBX_INITIALIZED;

  g_pErr->DFO("Template::Template", m_strDebug.c_str(), 2);
}

Template::~Template()
{
  g_pErr->DFI("Template::~Template", m_strDebug.c_str(), 2);

  Cleanup();
  s_mmapInputDevPtr.clear();
  m_msgMap.clear();
  m_vEventsAsync.clear();

  g_pErr->DFO("Template::~Template", m_strDebug.c_str(), 2);
}

int Template::Finish() {
  Trial * pCurTrial = GetCurTrial();

  if (m_nStatus != SBX_FINISHED) {
    g_pErr->DFI("Template::Finish", m_strDebug.c_str(), 2);
    // stop all running input devices
    InputDevPtrMMap::iterator jj;

    for (jj = s_mmapInputDevPtr.begin(); jj != s_mmapInputDevPtr.end(); jj++) {
      (*jj).second->Stop();
      if (jj->second->RecMode()) {
	if (pCurTrial) {
	  pCurTrial->SaveRespData(jj->second->GetRespData());
	} else {
	  g_pErr->Report("Template::Finish, pCurTrial not defined!");
	}
      } else {}
    }

    StimulusPtrMMap::iterator ii;

    for (ii = m_mmapAllAOI.begin(); ii != m_mmapAllAOI.end(); ii++) {
      (*ii).second->Finish();
    }

    m_mmapAllAOI.clear();
    m_mmapAllAOI2.clear();
    m_mapResources.clear();

    StateMap::iterator si;
    for (si = m_mapState.begin(); si != m_mapState.end(); si++) {
      si->second->Finish();
      si->second->PostTrial();
    }

    m_nStatus = SBX_FINISHED;
    g_pErr->DFO("Template::Finish", m_strDebug.c_str(), 2);
  } else {}

  //g_pErr->DFO("Template::Finish", m_strDebug.c_str(), 2);

  return 0;
}

int Template::Cleanup() {
  m_mapState.clear();
  m_mmapAllAOI.clear();
  m_mmapAOI.clear();
  m_nStatus = SBX_DEALLOCATED;
  //m_mapAOI.clear();
}

int Template::LoadStates() {
  std::string q = pastestr::paste("sds", " ", "\
SELECT StateID, Name, Sequence \n\
FROM State \n\
WHERE TemplateID=", m_id, "\n\
ORDER BY Sequence;\n");

  g_prsStim->ExecCount(q);

  int nStates = (int) g_prsStim->NumRows();

  g_pErr->Debug(pastestr::paste("sdsds", " ",
				"TemplateID", m_id, "has", (long) nStates, "states.\n"));

  if (nStates > 0) {

    for (int i = 0; i < nStates; i++) {
      m_mapState[atol(g_prsStim->Get(0))] = StatePtr(new State(atol(g_prsStim->Get(0)),
							       g_prsStim->Get(1), atol(g_prsStim->Get(2)) ));
      g_prsStim->MoveNext();
    }

    StateMap::iterator i;
    for (i = m_mapState.begin(); i != m_mapState.end(); i++) {
      i->second->Load(this);
    }
  } else {
    g_pErr->Debug(pastestr::paste("sd", " ", "NO STATES WERE LOADED for TemplateID", m_id));
  }

  StateMap::iterator i;
  for (i = m_mapState.begin(); i != m_mapState.end(); i++) {
    i->second->FinalSweep(this);
  }

  return 0;
}

int Template::LoadAOIs() {

  g_pErr->DFI("Template::LoadAOIs", m_strDebug.c_str(), 2);

  LoadAOI<Template>(this, "template", this);

  g_pErr->DFO("Template::LoadAOIs", m_strDebug.c_str(), 2);

  return 0;
}

int Template::AddAOIs(StimulusPtrMMap m, bool bTemplateAlso /*=true*/) {
  // used to add AOIs from Item to Template
  StimulusImg * pt = NULL;

  g_pErr->DFI("Template::AddAOIs", m_strDebug.c_str(), 2);
  StimulusPtrMMap::iterator ii;

  if (bTemplateAlso) {
    m_mmapAllAOI.clear();
    m_mmapAllAOI2.clear();

    m_mmapAllAOI = StimulusPtrMMap(m_mmapAOI.begin(), m_mmapAOI.end());
    
    for (ii = m_mmapAOI.begin(); ii != m_mmapAOI.end(); ii++) {
      m_mmapAllAOI2.insert(StimulusPair2( ((StimulusImg *) (*ii).second.get())->m_nLayer, (*ii).second));
      pt = (StimulusImg *) ii->second.get();
      g_pErr->Debug(pastestr::paste("sddd", ", ",
				    ii->first.c_str(),
				    (long) pt->m_x1, (long) pt->m_y1, (long) pt->m_nLayer));
      //m_mmapAllAOI.insert(AOIPair((*ii).first, (*ii).second));
    }
  } else {}

  for (ii = m.begin(); ii != m.end(); ii++) {
    m_mmapAllAOI2.insert(StimulusPair2( ((StimulusImg *) (*ii).second.get())->m_nLayer, (*ii).second));
    m_mmapAllAOI.insert(StimulusPair( (*ii).first, (*ii).second));
  }

  g_pErr->DFO("Template::AddAOIs", m_strDebug.c_str(), 2);

  return 0;
}

int Template::PrepareAOIs() {
  // pre-load all of the AOIs
  StimulusPtrMMap2::iterator ii;
  for (ii = m_mmapAllAOI2.begin(); ii != m_mmapAllAOI2.end(); ii++) {
    (*ii).second->Prepare();
  }

  return 0;
}

int Template::Prepare() {
  g_pErr->DFI("Template::Prepare", m_strDebug.c_str(), 2);

  if (!GetCurTrial()) {
    g_pErr->Report("Template::Prepare before undefined");
  } else {
    //g_pErr->Report("Template::Prepare before defined");
  }

  PrepareAOIs();

  m_vRespData.clear();
  //m_vGSC1SoundButton.clear();
  //m_vGSC1Toggle.clear();
  //m_vGSC1Cursor.clear();
  //m_vGSC1Stars.clear();
  //m_vGSC1Flags.clear();
  m_msgMap.clear();

  // find the first state in the sequence
  multimap<long, StatePtr> mmapStateSeq;

  //StateMap mapStateSeq;

  multimap<long, StatePtr>::iterator mis;
  pair<long, StatePtr> pi1;
  StateMap::iterator is;

  for (is = m_mapState.begin(); is != m_mapState.end(); is++) {
    mmapStateSeq.insert(pair<long, StatePtr>(is->second->GetSeq(), is->second));
  }

  for (mis = mmapStateSeq.begin(); mis != mmapStateSeq.end(); mis++) {
    //g_pErr->Debug(pastestr::paste("dd", " ",
    //(*mis).second->GetID(), (*mis).second->GetSeq()));    
    (*mis).second->Prepare();
  }

  mis = mmapStateSeq.begin();
  m_pCurState = (*mis).second.get();
  AttachState(m_pCurState);

  // get the input devices ready?
  for (InputDevPtrMMap::iterator jj = s_mmapInputDevPtr.begin();
       jj != s_mmapInputDevPtr.end(); jj++) {
    (*jj).second->Prepare();
  }

  m_nStatus = SBX_PREPARED;

  if (!GetCurTrial()) {
    g_pErr->Report("Template::Prepare after undefined");
  } else {
    //g_pErr->Report("Template::Prepare after defined");
  }
  
  g_pErr->DFO("Template::Prepare", m_strDebug.c_str(), 2);
  return 0;
}

int Template::Run() {
  g_pErr->DFI("Template::Run", m_id, 1);

  m_nStatus = SBX_IN_PROGRESS;
  if (m_pCurState) {
    m_pCurState->Run();
  } else {
    g_pErr->Report(pastestr::paste("ss", " ", "no states found for Template", 
																	 m_strName.c_str()));
  }

  g_pErr->DFO("Template::Run", m_id, 1);
  return 0;
}

int Template::HandleEvent(SDL_Event * pEvt) {
  Watch * pwSignaled = NULL;

  if (m_pCurState) {
    
    // input devices should handle it
    for (InputDevPtrMMap::iterator ii = s_mmapInputDevPtr.begin();
         ii != s_mmapInputDevPtr.end(); ii++) {
      (*ii).second->HandleEvent(pEvt);
    }

    //g_pErr->Debug("here2");
    pwSignaled = m_pCurState->HandleEvent(pEvt, this);
    //g_pErr->Debug("here3");

    if (pwSignaled) {
      g_pErr->DFI("Template::HandleEvent", m_id, 1);
      
      if (pwSignaled->GetNextID() != 0) {
				AttachState(m_mapState[pwSignaled->GetNextID()].get());
				(m_mapState[pwSignaled->GetNextID()].get())->Start();
				//m_pCurState->Run();
      } else {
				g_pErr->Debug("ending trial");
				Finish();
				SDL_Event event;
				event.type = SDL_USEREVENT;
				event.user.code = SBX_FINISHED;
				SDL_PushEvent(&event);
      }

      g_pErr->DFO("Template::HandleEvent", m_id, 1);
    } else {}
  } else {}

  return 0;
}

int Template::AttachState(State * pState) {
  g_pErr->DFI("Template::AttachState", m_id, 1);

  m_pCurState = pState;
  //pState->Start(ClockFn());

  //m_pCurState->Prepare();
  //m_mmapWatch = m_pCurState->GetWatches();

  g_pErr->DFO("Template::AttachState", m_id, 1);
  return 0;
}

Watch * Template::FindWatch(long id) {
  Watch * pWatch = NULL;
  StateMap::iterator ii;
  WatchMapIter wpi;
  WatchMap::iterator wi;
  StatePtr pState;

  for (ii = m_mapState.begin(); ii != m_mapState.end(); ii++) {
    if (pWatch) {
      break;
    } else {}
    pState = (*ii).second;
    //g_pErr->Debug(pastestr::paste("sd", "", "State ", pState->GetID()));
    for (wi = pState->m_mmapWatch.begin(); wi != pState->m_mmapWatch.end(); wi++) {
      if ((*wi).second->GetID() == id) {
	pWatch = (*wi).second.get();
	//g_pErr->Debug("found it");
	break;
      } else {
	//g_pErr->Debug(pastestr::paste("sd", "", "Watch ", (*wi).second->GetID()));
      }
    }
  }

  if (!pWatch) {
    g_pErr->Debug("watch not found");
  } else {}

  return pWatch;
}

int Template::Redraw(setStimulus setExcluding, bool bMem /* = FALSE */) {
  StimulusPtr pAOI;
  StimulusPtrMMap2::iterator ii;

  for (ii = m_mmapAllAOI2.begin(); ii != m_mmapAllAOI2.end(); ii++) {
    pAOI = (*ii).second;
    ((StimulusImg *) pAOI.get())->Draw(bMem);
    //if (setExcluding.find(pAOI)) {
      //pAOI->
    //} else {}
  }

  return 0;
}

int Template::Redraw(bool bMem /* = FALSE */) {
  StimulusPtr pAOI;
  StimulusPtrMMap2::iterator ii;
  for (ii = m_mmapAllAOI2.begin(); ii != m_mmapAllAOI2.end(); ii++) {
    pAOI = (*ii).second;
    ((StimulusImg *) pAOI.get())->Draw(bMem);
  }
  Display_SDL::Flip1(bMem);

  return 0;
}

StimulusPtr * Template::AOIArg(string s) {
  StimulusPtr * ppStim = NULL;
  ostringstream ostr;
  long idWatch = -1;

  if (s[0] == '@') { // variable search
    for (int i = 7; i < s.size(); i++) {
      if (s[i] == ']') {
	break;
      } else {
	ostr << s[i];
      }
    }
    idWatch = atol(ostr.str().c_str());
    g_pErr->Debug(pastestr::paste("dss"," ",(long) idWatch, " watchID ", ostr.str().c_str()));
    WatchMouse * pwm = (WatchMouse *) FindWatch(idWatch);
    if (pwm) {
      ppStim = &pwm->m_pSelectedAOI;
    } else {
      g_pErr->Report(pastestr::paste("sd", "", "couldn't find watchID ", (long) idWatch));
    }
  } else { // search by name, if not found, return NULL
    pair<StimulusPtrMMap::iterator, StimulusPtrMMap::iterator> ppp;
    ppp = m_mmapAllAOI.equal_range(s);
    if (ppp.first != ppp.second) {
      ppStim = &((*ppp.first).second);
    } else {
      //g_pErr->Debug(pastestr::paste("ds", " ", m_mmapAllAOI.size(), "elements"));
      g_pErr->Debug(pastestr::paste("ss", "", "!!! couldn't find AOI ", s.c_str()));
    }
    //m_mmapAllAOI
  }

  return ppStim;
}

int Template::Reinsert(StimulusImg * pAOI) {
  //AOIPtr ptrAOI;
  //AOIPtr ptrAOIold;
  StimulusPtrMMap2::iterator pi;
  //AOIPair2 p;
  g_pErr->DFI("Template::Reinsert", m_id, 3);

  int nFound = 0;

  for (pi = m_mmapAllAOI2.begin(); pi != m_mmapAllAOI2.end(); pi++) {
    if (pi->second.get() == pAOI) {
      nFound = 1;
      ((StimulusImg *) pi->second.get())->swap(*pAOI);
      m_mmapAllAOI2.insert(StimulusPair2(((StimulusImg *) pi->second.get())->m_nLayer, pi->second));
      //ptrAOI = pi->second;
      m_mmapAllAOI2.erase(pi);
      break;
    } else {}
  }

  if (!nFound) {
    g_pErr->Report("couldn't find the AOI!!!");
  } else {}

  g_pErr->DFO("Template::Reinsert", m_id, 3);
  return 0;
}

string GetBracketedValue(string s) {
  string sResult("");
  string::size_type p1 = s.find_first_of("[");
  string::size_type p2 = s.find_first_of("]");

  if ( (p2 == string::npos) || (p2 < p1) || (p1 == string::npos)  ) {
  //g_pErr->Report(pastestr::paste("sss", "",
  //			   "Error in argument string '", s.c_str(), "'"));
  } else {
    sResult = s.substr(p1+1, (p2-p1));
  }

  return (sResult);
}

string GetDottedValue(string s) {
  string sResult("");
  string::size_type p1 = s.find_first_of(".");

  if (  (p1 == string::npos) || (p1 == (s.size()-1) )  ) {
    g_pErr->Report(pastestr::paste("sss", "",
				   "Error in argument string '", s.c_str(), "'"));    
  } else {
    sResult = s.substr(p1+1, s.size()-p1);
  }

  return sResult;
}

Operation * Template::FindVar(string s) {
  //string idString("");
  long id;
  string dot("");
  Operation * poResult = NULL;

  id = atol(GetBracketedValue(s).c_str());
  dot = GetDottedValue(s);

  if (s.substr(0,3)=="aoi") {
    StimulusPtrMMapIter pi;
    for (pi = m_mmapAllAOI.begin(); pi != m_mmapAllAOI.end(); pi++) {
      if ((*pi).second->GetID() == id) {
	break;
      } else {}
    }
  } else if (s.substr(0,5)=="mouse") {
    if (dot == "x") {
      poResult = &State::s_nMouseCurX;
    } else if (dot == "y") {
      poResult = &State::s_nMouseCurY;
    } else {
      // TO DO : error
    }
  } else {}
}

Operation Template::NewOplist(string s) {
  g_pErr->DFI("Template::NewOplist", m_id, 3);

  OpChain o(s);

  OpPtrMap::iterator pi;
  for (pi = OpChain::g_mapUnassignedPtrs.begin();
       pi != OpChain::g_mapUnassignedPtrs.end(); pi++) {
    g_pErr->Debug(pastestr::paste("ss", " ", 
			     (*pi).first.c_str(), " "));
    //*((*ii).second) = ;
  }

  g_pErr->DFO("Template::NewOplist", m_id, 3);
  return (Operation) o;
}

int Template::Update() {
  vector<EventPair>::iterator ii;
  EventPtr pEvt;

  if (!GetCurTrial()) {
    g_pErr->Report("Template::Update CurTrial undefined");
  } else {
    //g_pErr->Report("defined");
  }

  //g_pErr->DFI("Template::Update", GetID(), 1);

  for (ii = m_vEventsAsync.begin(); ii != m_vEventsAsync.end(); ii++) {
    pEvt = (*ii).second;
    ((EventAsync *) pEvt.get())->Update();
  }

  //g_pErr->DFO("Template::Update", GetID(), 1);
  //return m_pCurState->Update();
	return 0;
}

int Template::Start() {
  g_pErr->DFI("Template::Start", m_id, 3);
  //AttachState(m_pCurState);
  g_pErr->DFO("Template::Start", m_id, 3);
  m_nStatus = SBX_IN_PROGRESS;
  if (!GetCurTrial()) {
    g_pErr->Report("undefined");
  } else {
   //g_pErr->Report("defined");
  }

  return m_pCurState->Start();
}

/*
string Template::ParseResource(string s) {
  string sr(s);
  g_pErr->DFI("Template::ParseResource", GetName().c_str(), 1);

  if (s.length() > 0) {
    if (s[0] == '$') {
      s.erase(0, 1);
      sr = m_mapResources[s];
      //sr = s;
    } else {
    }
  } else {}

  g_pErr->DFO("Template::ParseResource", GetName().c_str(), 1);
  return sr;
}
*/

/*
int Template::SetResourceMap(ResourceMap m) {
  g_pErr->DFI("Template::SetResourceMap", GetName().c_str(), 1);
  m_mapResources = m;

  g_pErr->DFO("Template::SetResourceMap", GetName().c_str(), 1);
  return 0;
}
*/


vector<EventTimePtr> Template::GetStateTiming() {
  vector<EventTimePtr> v;
  EventTimePtr p1;
  StatePtr pState;

  for (StateMap::iterator ii = m_mapState.begin();
       ii != m_mapState.end(); ii++) {
    pState = (*ii).second;
    if (pState->WasVisited()) {
      for (int i = 0; i < pState->m_vMsBegin.size(); i++) {
	p1 = EventTimePtr(new EventTime(pState->GetID(), (long) pState->m_vMsBegin[i]));
	v.push_back(p1);
      }
    } else {}
  }

  return v;
}

vector<EventTimePtr> Template::GetEventTiming() {
  vector<EventTimePtr> v1;
  StatePtr pState;  
  EventPtr pe;
  EventTimePtr p1;

  for (StateMap::iterator ii = m_mapState.begin();
       ii != m_mapState.end(); ii++) {
    pState = (*ii).second;
    for (EventMap::iterator ei = pState->m_mmapEvent.begin();
	 ei != pState->m_mmapEvent.end(); ei++) {
      pe = (*ei).second;
      if (pe->WasVisited()) {
	for (int i = 0; i < pe->m_vMsExec.size(); i++) {
	  p1 = EventTimePtr(new EventTime(pe->ID(), pe->m_vMsExec[i]));
	  v1.push_back(p1);
	}
      } else {}
    }
    //v1.insert(v1.end(), v2.begin(), v2.end());
    //p1 = EventTimePtr(new EventTime(pState->GetID(), (long) pState->m_msBegin));
    //v.push_back(p1);
  }  

  return v1;
}

int Template::InsertAOIs(StimulusPtrMMap m) {
  g_pErr->DFI("Template::InsertAOIs", GetID(), 1);
  StimulusPtrMMap::iterator ii;

  for (ii = m.begin(); ii != m.end(); ii++) {
    InsertAOI((*ii).first, (*ii).second);
    //m_mmapAllAOI2.insert(StimulusPair2( ((StimulusImg *) (*ii).second.get())->m_nLayer, (*ii).second));
    //m_mmapAllAOI.insert(StimulusPair( (*ii).first, (*ii).second));
  }

  g_pErr->DFO("Template::InsertAOIs", GetID(), 1);
  return 0;
}

int Template::InsertAOI(string strName, StimulusPtr pStim) {
  StimulusImg * pStimBMP = (StimulusImg *) pStim.get();
  ostringstream osstr;

  osstr << strName.c_str() << " " << pStimBMP->m_x1 << " " << pStimBMP->m_y1;

  g_pErr->DFI("Template::InsertAOI", osstr.str().c_str(), 1);
  //m_mmapAllAOI2.insert(StimulusPair2(pStimBMP->m_nLayer, pStim));
  //m_mmapAllAOI.insert(StimulusPair(strName.c_str(), pStim));
  m_mmapAOI.insert(StimulusPair(strName.c_str(), pStim));
  g_pErr->DFO("Template::InsertAOI", osstr.str().c_str(), 1);
  return 0;
}

int Template::AddEventAsync(EventPtr pEvt, long id /* = -1 */) {
  if (id == -1) {
    id = pEvt->ID();
  } else {}

  m_vEventsAsync.push_back(EventPair(id, pEvt));

  return 0;
}

EventPtr Template::FindEvent(long id) {
  StatePtr pState;
  EventPtr pResult;

  for (StateMap::iterator ii = m_mapState.begin(); ii != m_mapState.end(); ii++) {
    pState = (*ii).second;
    g_pErr->Debug(pastestr::paste("sdsd", " ", "StateID", pState->GetID(), "looking for", id));
    pResult = pState->FindEvent(id);
    if (pResult.get()) {
      g_pErr->Debug("Event Found!");      
      break;
    } else {}
  }

  return pResult;
}

long Template::GetSessionID() {
  return Trial::g_pExp->SessionID();
}

long Template::GetTrialID() {
  return Trial::g_pExp->CurrentTrialID();
}

long Template::GetTrialSeq() {
  return 0L;
}

EventPtr Template::FindEventByCmdID(long idCmd) {
  vector<EventPtr> vEvents;
  StatePtr pState;
  EventPtr pResult;

  for (StateMap::iterator ii = m_mapState.begin(); ii != m_mapState.end(); ii++) {
    pState = (*ii).second;
    g_pErr->Debug(pastestr::paste("sdsd", " ", "StateID", pState->GetID(), "looking for", idCmd));
    pResult = pState->FindEventByCmdID(idCmd);
    if (pResult.get()) {
      vEvents.push_back(pResult);
    } else {}
  }

  if (vEvents.size() != 1) {
    g_pErr->Report(pastestr::paste("sdsd", " ",
				   "there were", (long) vEvents.size(), "events found matching idCmd",
				   idCmd));				   
  } else if (vEvents.size() == 0) {
    g_pErr->Report("event not found!");
  } else {
    pResult = vEvents[0];
  }

  return pResult;
}

bool Template::DeviceExists(unsigned long idDev, int n) {
  bool bResult = false;
  pair<ipdmIterator, ipdmIterator> pii;
  ipdmIterator ii;
  pii = Template::s_mmapInputDevPtr.equal_range(idDev);

  if (pii.first == pii.second) {
    bResult = false;
  } else {
    for (ii = pii.first; ii != pii.second; ii++) {
      if (ii->second->GetIndex() == n) {
				bResult = true;
				break;
      } else {}
    }
  }
      
  return bResult;
}

InputDevPtr Template::GetDevice(unsigned long idDev, int n) {
  pair<ipdmIterator, ipdmIterator> pii;
  ipdmIterator ii;
  pii = Template::s_mmapInputDevPtr.equal_range(idDev);
  InputDevPtr pDev;

  for (ii = pii.first; ii != pii.second; ii++) {
    if (ii->second->GetIndex() == n) {
      pDev = ii->second;
      break;
    } else {}
  }

  return pDev;
}

InputDevPtr Template::FindOrCreateInputDev(unsigned long idDev, int nIndex /*=0*/) {
  g_pErr->DFI("Template::FindOrCreateInputDev", 0L, 1);

  InputDevPtr pDev;
  pair<ipdmIterator, ipdmIterator> ii;
  ii = Template::s_mmapInputDevPtr.equal_range(idDev);
  bool bExists = false;

  if (DeviceExists(idDev, nIndex)) {
    pDev = GetDevice(idDev, nIndex);
  } else {
    // create it
    switch (idDev) {
    case SBX_MOUSE_DEV :
      {
				g_pErr->Debug("creating mouse device");
				pDev = InputDevPtr(new Mouse_SDL());
      }
      break;
    case SBX_GAMEPAD_DEV :
      {
				g_pErr->Debug("creating gamepad device");
				pDev = InputDevPtr(new GamePad_SDL(SBX_GAMEPAD_DEV, nIndex));
      }
      break;
    case SBX_SCROLLTRACKGP_DEV :
      {
				g_pErr->Debug("creating periscopic display device");
				pDev = InputDevPtr(new ScrollTrackGP_SDL(SBX_SCROLLTRACKGP_DEV, nIndex));
      }
      break;
    case SBX_AUDIOREC_DEV :
      {
#ifndef WIN32
				g_pErr->Debug("creating audioinput device");
				//pDev = InputDevPtr(new AlsaSoundIn(SBX_AUDIOREC_DEV));
				pDev = InputDevPtr(new SoundInput(SBX_AUDIOREC_DEV));
#endif
      }
      break;
		case SBX_SOCKET_DEV :
			{
				g_pErr->Debug("creating socket device");
				pDev = InputDevPtr(new Socket(SBX_SOCKET_DEV, nIndex));
			}
			break;
    default:
      {
				g_pErr->Report("error in Template::FindOrCreateInputDev");
      }
      break;
    }
    Template::s_mmapInputDevPtr.insert(pair<unsigned long, InputDevPtr>(idDev, pDev));
  }

  g_pErr->DFO("Template::FindOrCreateInputDev", 0L, 1);
  return pDev;
}

Trial * Template::GetCurTrial() {
  Trial * pCurTrial = NULL;

  if (m_pItemCell) {
    pCurTrial = m_pItemCell->GetCurTrial();
  } else {
    g_pErr->Report("m_pItemCell not defined!");
  }

  return pCurTrial;
}

void Template::RepeatExperiment() {
  g_pErr->DFI("Template::RepeatExperiment", m_strDebug.c_str(), 2);
  if (Template::g_pExperiment!=NULL) {
    g_pExperiment->RepeatExperiment();
  } else {}

  g_pErr->DFO("Template::RepeatExperiment", m_strDebug.c_str(), 2);
}

void Template::IncrementCounter(const char * pcCtr) {
  g_pErr->DFI("Template::IncrementCounter", m_strDebug.c_str(), 2);
  if (Template::g_pExperiment!=NULL) {
    g_pExperiment->IncrementCounter(pcCtr);
  } else {}

  g_pErr->DFO("Template::IncrementCounter", m_strDebug.c_str(), 2);
}

void Template::ResetCounter(const char * pcCtr) {
  g_pErr->DFI("Template::ResetCounter", m_strDebug.c_str(), 2);
  if (Template::g_pExperiment!=NULL) {
    g_pExperiment->ResetCounter(pcCtr);
  } else {}

  g_pErr->DFO("Template::ResetCounter", m_strDebug.c_str(), 2);
}

long Template::GetCounter(const char * pcCtr) {
  long res = 0;

  if (Template::g_pExperiment!=NULL) {
    res = Template::g_pExperiment->GetCounter(pcCtr);
  } else {
    g_pErr->Report("in Template::GetCounter; g_pExperiment undefined!");
  }

  return res;
}

Uint32 Template::GetMSElapsed() {
  return Template::g_pExperiment->GetMSElapsed();
}
