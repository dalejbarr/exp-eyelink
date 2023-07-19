#ifndef EXP_WATCH_INCLUDED
#define EXP_WATCH_INCLUDED

#include <boost/smart_ptr.hpp>

#include <map>
using std::multimap;
using std::pair;
using std::iterator;

#include <string>
using std::string;

#include "SDL.h"

class Watch  
{
protected:
  long m_id;
  long m_idNext;
  bool m_bSignaled;
public:
  Watch(long id, long idNext) : m_id(id), m_idNext(idNext), m_bSignaled(0) {}
  virtual ~Watch();
  virtual int Cleanup();
	virtual int Activate() { return 0; }
  virtual int Prepare() { m_bSignaled = 0; return 0; }
  virtual int Signal() { m_bSignaled = 1; return 0; }
  inline long GetID() { return m_id; }
  inline long GetNextID() { return m_idNext; }
  virtual int Finish();
  virtual int PostTrial();
  virtual bool CheckCondition(SDL_Event * pEvt = NULL);
};

typedef boost::shared_ptr<Watch> WatchPtr;

typedef multimap<long, WatchPtr> WatchMap;
typedef pair<long, WatchPtr> WatchPair;
typedef pair<WatchMap::iterator, WatchMap::iterator> WatchMapIter;

#endif
