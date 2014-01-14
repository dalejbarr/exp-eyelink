#ifndef EVENT_INCLUDED
#define EVENT_INCLUDED

#define SBX_EVENT_COMPLETED 600

#include <boost/smart_ptr.hpp>

#include "Stimulus.hpp"
#include "Device.hpp"
#include "global.hpp"

class Template;

class Event
{
protected:
  long m_id;
  long m_idCmd;
  Uint32 m_msec;
  //bool m_bVisited;
  //Stimulus * m_pStim;
  //Stimulus ** m_ppStim;
  StimulusPtr m_pStim;
  StimulusPtr * m_ppStim;
  Template * m_pTemplate;
  
public:
  //StimulusPtr * m_ppStim;
  vector<Uint32> m_vMsExec;
  Event(long id, long msec);
  Event(long id, long msec, StimulusPtr pStim);
  Event(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate = NULL);
  virtual ~Event();

  virtual int Prepare();
  virtual int Action();
  virtual int Cleanup();
  virtual int Finish();
  virtual int FinalSweep(Template * pTemplate);
  virtual int PostTrial();

  inline Uint32 ID() { return m_id; }
  inline Uint32 Msec() { return m_msec; }
  inline Uint32 CmdID() { return m_idCmd; }
  //inline Uint32 MsecExec() { return m_msecExec; }
  //inline bool WasVisited() { return m_bVisited; }
  inline bool WasVisited() { return (m_vMsExec.size()>=1); }
};

typedef boost::shared_ptr<Event> EventPtr;

#endif
