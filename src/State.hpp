#ifndef ST_INCLUDED
#define ST_INCLUDED

#define EXP_EVENT_TIMER_RESOLUTION 3

#include <string>
using std::string;

#include <map>
using std::map;
using std::multimap;
using std::pair;

#include <boost/smart_ptr.hpp>

#include "Event.hpp"
#include "Watch.hpp"
#include "global.hpp"

typedef multimap<long, EventPtr> EventMap;
typedef pair<EventMap::iterator, EventMap::iterator> EventMapIter;

class Template;
//class EventTime;
class EventGrabAOI;

class State
{
 protected:
  long m_id;
  string m_strName;
  string m_strDebug;
  long m_lSeq;
  Uint32 m_nTimeout;
	static SDL_Thread * s_pThread;
	static bool s_bContinue;
  //bool m_bVisited;

  EventGrabAOI * m_pEvtMove;
  int m_nWatchesLocked;
  EventMap::iterator m_pCurEvent;

 public:
  EventMap m_mmapEvent;
  vector<Uint32> m_vMsBegin;

  static int s_nMouseX;
  static int s_nMouseY;
  static OpInt s_nMouseCurX;
  static OpInt s_nMouseCurY;

	static bool s_bFinished;
	static bool s_bTimedOut;
  
  WatchMap m_mmapWatch;
  State(long id, const char * pcName, long seq);
  virtual ~State();

  int Load(Template * pTemplate);
  int FinalSweep(Template * pTemplate); // do this after all states are loaded and before first trial
  int LoadEvents(Template * pTemplate);
  int LoadWatches(Template * pTemplate);
  int Prepare();
  int Start();
  int Main();
  int Run();
  int Finish();
  int PostTrial(); // when the trial ends
  EventPtr FindEvent(long id);
  EventPtr FindEventByCmdID(long id);

  static Uint32 timeoutFn(Uint32 interval, void *param);
	static int main(void * pVoid);
  Watch * HandleEvent(SDL_Event * pEvt, Template * pThis);
  Watch * ProcessMouseButton(SDL_Event * pEvt);
  inline long GetSeq() { return m_lSeq; }
  inline WatchMap GetWatches() { return m_mmapWatch; }
  inline long GetID() { return m_id; }
  inline bool WasVisited() { return (m_vMsBegin.size()>=1); }

  ArgMMap GetArgs(string strTable, long id);

  int Cleanup();
  void LockWatches() { m_nWatchesLocked = 1; }
  void UnlockWatches() { m_nWatchesLocked = 0; }
  int WatchesLocked() { return m_nWatchesLocked; }
};

typedef boost::shared_ptr<State> StatePtr;

#endif
