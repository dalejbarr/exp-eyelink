#include "Event.hpp"
#include "global.hpp"
#include "CmdID.hpp"
#include "Template.hpp"
#include "WatchMouse.hpp"
#include "pastestr.hpp"

#include <map>
using std::multimap;
using std::pair;
#include <sstream>
using std::ostringstream;

//extern Uint32 ClockFn();

Event::Event(long id, long msec) {
  m_id = id;
  m_idCmd = 0;
  m_msec = msec;
  m_vMsExec.clear();
  m_pStim.reset();
  m_ppStim = &m_pStim;
  m_pTemplate = NULL;
  //m_bVisited = 0;
}

Event::Event(long id, long msec, StimulusPtr pStim)
{
  m_id = id; m_msec = msec;
  m_idCmd = 0;
  m_msec = msec;
  m_vMsExec.clear();
  m_pStim = pStim;
  m_ppStim = &m_pStim;
  m_pTemplate = NULL;
  //m_bVisited = 0;
}

Event::Event(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate) {
  g_pErr->DFI("Event::Event", id, 2);
  m_id = id; m_msec = msec;
  m_idCmd = idCmd;
  m_vMsExec.clear();
  m_pStim.reset();
  m_ppStim = &m_pStim;
  m_pTemplate = pTemplate;
  //m_bVisited = 0;

  // process any common arguments

  // event-specific arguments
  //switch (idCmd) {
  //}

  g_pErr->DFO("Event::Event", id, 2);
}

Event::~Event()
{
  g_pErr->DFI("Event::~Event", m_id, 4);
  Cleanup();
  g_pErr->DFO("Event::~Event", m_id, 4);
}

int Event::Prepare()
{
  g_pErr->DFI("Event::Prepare", m_id, 4);
  //m_bVisited = 0;
  m_vMsExec.clear();

  if ( (*m_ppStim) ) {
    (*m_ppStim)->Prepare();
  } else {
    /*
    ostringstream ostr;
    ostr << *m_ppStim;
    g_pErr->Report(pastestr::paste("sss", "", 
				   "m_ppStim not defined (was ",
				   ostr.str().c_str(), ")"));
    */
  }

  g_pErr->DFO("Event::Prepare", m_id, 4);
  return 0;
}

int Event::Action()
{
  g_pErr->DFI("Event::Action", m_id, 4);

  //if ( m_idCmd == SBX_EVENT_MOVEAOI ) {
    //g_pErr->Debug("here");
    //((StimulusBmp *) (*m_ppStim).get())->SetpXY(m_px, m_py);
  //} else {}

  if ( (*m_ppStim) ) {
    (*m_ppStim)->Action();
    //g_pErr->Debug(pastestr::paste("sdd", " ", "here", m_id, (*m_ppStim)->GetID() ));
  } else {
    g_pErr->Debug(pastestr::paste("sds", "", "stimulus associated w/EventID=", m_id,
			     " was not defined"));
  }

  m_vMsExec.push_back(ClockFn());
  //m_bVisited = 1;

  g_pErr->DFO("Event::Action", m_id, 4);

  return 0;
}

int Event::Cleanup()
{
  if ( m_pStim ) {
    m_pStim->Cleanup();
    m_ppStim = NULL;
  } else {}
  return 0;
}

int Event::Finish() {
  g_pErr->DFI("Event::Finish", m_id, 4);  

  if ( (*m_ppStim) ) {
    (*m_ppStim)->Finish();
  }

  g_pErr->DFO("Event::Finish", m_id, 4);  

  return 0;
}

int Event::FinalSweep(Template * pTemplate) {
  return 0;
}

int Event::PostTrial() {
  return 0;
}
