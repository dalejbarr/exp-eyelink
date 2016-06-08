#ifndef EXP_TEMPLATE_INCLUDED
#define EXP_TEMPLATE_INCLUDED

#include <string>
using std::string;

#include <sstream>
using std::istringstream;

#include <map>
using std::map;

#include <boost/smart_ptr.hpp>
#include <boost/bimap.hpp>

#include <map>
using std::multimap;

#include "State.hpp"
#include "StimulusImg.hpp"
#include "Watch.hpp"
#include "InputDev.hpp"
#include "EventTime.hpp"
#include "RespData.hpp"
// #include "GSC1RespObj.hpp"

#include <set>
using std::set;

typedef map<long, StatePtr> StateMap;
typedef boost::bimap<StimulusPtr, string> VarMap;
typedef set<StimulusPtr> setStimulus;
typedef map<string, Operation *> OpMap;
typedef map<string, string> ResourceMap;
typedef pair<string, string> MsgPair;
typedef map<long, MsgPair> MsgMap;
typedef pair<long, EventPtr> EventPair;
//typedef boost::shared_ptr<RespObj> RespObjPtr;

class ItemCell;
class Trial;
class Experiment;

class Template
{
protected:
  //int m_nStates;
  string m_strName;
  string m_strDebug;
  //WatchMap m_mmapWatch;

  long m_id;
  long m_lgTimeout;
  State * m_pCurState;
  ResourceMap m_mapResources;
  vector<EventPair> m_vEventsAsync;
  ItemCell * m_pItemCell;

public:	
  StimulusPtrMMap m_mmapAOI; 
  StimulusPtrMMap m_mmapAllAOI; // all AOIs for a given trial, keyed by AOIname
  StimulusPtrMMap2 m_mmapAllAOI2; // all AOIs for a given trial, keyed by Layer
  MsgMap m_msgMap;
  StateMap m_mapState;
  static InputDevPtrMMap s_mmapInputDevPtr;
  static Experiment * g_pExperiment;
  int m_nStatus;
  vector<RespData> m_vRespData;
  // If I knew c++ properly, I wouldn't need the following lines
  //vector<GSC1SoundButton> m_vGSC1SoundButton;
  //vector<GSC1Toggle> m_vGSC1Toggle;
  //vector<GSC1Cursor> m_vGSC1Cursor;
  //vector<GSC1Locs> m_vGSC1Stars;
  //vector<GSC1Locs> m_vGSC1Flags;

  Template(long id, long timeout, const char * pcName);
  virtual ~Template();

  inline void SetCurItemCell(ItemCell * pic) { m_pItemCell = pic; };

  inline long GetID() { return m_id; };
  int LoadStates();
  int LoadAOIs();
  int Cleanup();
  int AddAOIs(StimulusPtrMMap m, bool bTemplateAlso = true);
  int InsertAOIs(StimulusPtrMMap m);
  int InsertAOI(string strName, StimulusPtr pStim);
  int Prepare();
  int PrepareAOIs();
  int Redraw(bool bMem = 0);
  int Redraw(setStimulus setExcluding, bool bMem = 0);
  int Run();
  int Finish();
  int HandleEvent(SDL_Event * pEvt);
  int AttachState(State * pState);
  int Start();
  int Update();
  inline void StoreMsg(Uint32 ms, MsgPair msg) { m_msgMap[ms] = msg; }
  int AddEventAsync(EventPtr pEvt, long id = -1);
  EventPtr FindEvent(long id);
  EventPtr FindEventByCmdID(long idCmd);
  long GetSessionID();
  long GetTrialID();
  long GetTrialSeq(); // get number of current trial in the sequence of all trials
  static bool DeviceExists(unsigned long idDev, int n = 0);

  //int SetResourceMap(ResourceMap m);
  static InputDevPtr FindOrCreateInputDev(unsigned long idDev, int nIndex = 0);
  static InputDevPtr GetDevice(unsigned long idDev, int nIndex = 0);
  //StimulusImg ** AOIArg(string s);
  StimulusPtr * AOIArg(string s);

  Watch * FindWatch(long id);
  Operation * FindVar(string s);

  Operation NewOplist(string s);
  int Reinsert(StimulusImg * pAOI);
  vector<EventTimePtr> GetStateTiming();
  vector<EventTimePtr> GetEventTiming();

  Trial * GetCurTrial();
  void RepeatExperiment();
  void IncrementCounter(const char * pcCtr);
  void ResetCounter(const char * pcCtr);
  long GetCounter(const char * pcCtr);
  Uint32 GetMSElapsed();
};

typedef boost::shared_ptr<Template> TemplatePtr;

#include "global.hpp"
#include "pastestr.hpp"
#include "StimulusImg.hpp"

template<class t> 
void LoadAOI(t * pClass, const char * pcLink, Template * pTemplate) {
  StimulusPtr pAOI;
  string s;
  string sTmp;
  pair<ArgIter, ArgIter> pii;
  ArgIter ii;
  int x2, y2;

  string q = pastestr::paste("sdsss", "", "\
SELECT AOI.AOIID, Name, Resource, x1, y1, Layer \n\
FROM AOI \n\
WHERE ID=", pClass->GetID(), " AND Link='", pcLink, "' \n\
ORDER BY Layer, AOI.AOIID ASC");

  string qArgs = pastestr::paste("sdsss", "", "\
SELECT AOIArgs.AOIID, AOIArgs.ArgID, AOIArgs.Info \n\
FROM AOIArgs INNER JOIN AOI ON AOI.AOIID=AOIArgs.AOIID \n\
WHERE AOI.ID=", pClass->GetID(), " AND AOI.Link='", pcLink, "' \n\
ORDER BY AOIArgs.AOIID, AOIArgs.ArgID");

  mID2mmArgs id2args = g_prsStim->GetID2Args(qArgs.c_str());
  ArgMMap mmArgs;

  g_prsStim->Exec(q);

  while (g_prsStim->RowReady()) {

    if (g_prsStim->Get(2)) {
      if (g_prsStim->Get(2)[1] == '$') {
	g_pErr->Report(pastestr::paste("ss", "",
				       "variables not allowed in AOIItem table\n\
(offending line: AOIID=", g_prsStim->Get(0)));
      } else {
	sTmp = string(g_prsStim->Get(2));
	//s.assign(CheckSlashes(g_prsStim->Get(2)));
	s.assign(CheckSlashes(sTmp.c_str()));
      }
    } else {
      s.assign("");
    }
    g_pErr->Debug(s.c_str());

    x2 = -1; y2 = -1;
    long id = atol(g_prsStim->Get(0));
    mmArgs = id2args[id];
    pii = mmArgs.equal_range("x2");
    if (pii.first != pii.second) {
      x2 = atoi((*pii.first).second.c_str());
    } else {}

    pii = mmArgs.equal_range("y2");
    if (pii.first != pii.second) {
      y2 = atoi((*pii.first).second.c_str());
    } else {}

    pAOI = StimulusPtr(new StimulusImg(id, pTemplate,
				  s.c_str(),
				  atoi(g_prsStim->Get(3)),
				  atoi(g_prsStim->Get(4)),
				  x2,
				  y2,
				  atoi(g_prsStim->Get(5)) ) );

    pii = mmArgs.equal_range("COLOR_KEY");
    if (pii.first != pii.second) {
      int r, g, b;
      istringstream iss((*pii.first).second);
      iss >> r >> g >> b;
      ((StimulusImg *) pAOI.get())->SetColorkey(r, g, b);      
    } else {}
    
    pClass->m_mmapAOI.insert(StimulusPair(g_prsStim->Get(1), pAOI));

    g_prsStim->MoveNext();
  }  
}


#endif
