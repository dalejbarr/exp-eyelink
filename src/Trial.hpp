#ifndef EXP_TRIAL_INCLUDED
#define EXP_TRIAL_INCLUDED

#include "ItemCell.hpp"
#include "Mouse_SDL.hpp"
#include "RespData.hpp"

/*
class EventTime {
public:
  unsigned long m_id;
  unsigned long m_ms;
  EventTime(unsigned long id, unsigned long ms = ClockFn()) ;
};
*/

//typedef boost::shared_ptr<EventTime> EventTimePtr;

class Experiment;

class Trial {
 protected:
  long m_id;
  bool m_bVisited;
  vector<RespData> m_vRespData;

 public:
  static Experiment * g_pExp;
  long m_idBlock;
  long m_idPhase;
  long m_idListOrder;
  long m_idResponse; // TO DO: allow multiple responses for same trial
  ItemCellPtr m_pItemCell;
  vector<MousePoint> m_mouseData;
  //vector<EventTimePtr> m_vEventTime;
  //vector<EventTimePtr> m_vStateTime;
  Uint32 m_msBegin;
  Uint32 m_msEnd;
  int m_nTrialStatus;

  Trial(long id, long idBlock, long idPhase, ItemCellPtr icp);
  virtual ~Trial();
  int Prepare();
  int Run();
  int Finish();
  int HandleEvent(SDL_Event * evt);

  int Start();
  int Update();
  int StoreData(long tnum, long idSess, long idSubj);
  void SaveRespData(const RespData rd);
  inline bool WasVisited() { return m_bVisited; }
  inline long ID() { return m_id; }
};

#endif
