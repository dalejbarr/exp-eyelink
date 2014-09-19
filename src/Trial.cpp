#include "Trial.hpp"
#include "global.hpp"
#include "pastestr.hpp"
#include "Template.hpp"
#include "Experiment.hpp"

/*
EventTime::EventTime(unsigned long id, unsigned long ms) {
  m_id = id; m_ms = ms;
}
*/

Experiment * Trial::g_pExp = NULL;

Trial::Trial(long id, long idBlock, long idPhase, ItemCellPtr icp) {
  m_id = id;
  m_idBlock = idBlock;
  m_idPhase = idPhase;
  m_pItemCell = icp;
  m_msBegin = 0;
  m_msEnd = 0;
  m_nTrialStatus = SBX_INITIALIZED;
  m_bVisited = 0;
}

Trial::~Trial() {
  g_pErr->DFI("Trial::~Trial", m_id, 2);
  m_mouseData.clear();
  g_pErr->DFO("Trial::~Trial", m_id, 2);
}

int Trial::Prepare() {
  m_pItemCell->SetCurTrial(this);
  int nResult = m_pItemCell->Prepare();
  m_bVisited = 1;
  m_nTrialStatus = SBX_PREPARED;
  return nResult;
}

int Trial::Run() {
  g_pErr->Report("stop");
  m_nTrialStatus = SBX_IN_PROGRESS;
  m_bVisited = 1;
  int nResult = m_pItemCell->Run();
  return 0;
}

int Trial::Finish() {

  int nResult = 0;

  g_pErr->DFI("Trial::Finish", m_id, 2);

  if (m_nTrialStatus != SBX_FINISHED) {
    m_msEnd = ClockFn();
    nResult = m_pItemCell->Finish();
    m_nTrialStatus = SBX_FINISHED;

    if (Template::DeviceExists(SBX_MOUSE_DEV)) {
      Mouse_SDL * pMDev = (Mouse_SDL *) Template::FindOrCreateInputDev(SBX_MOUSE_DEV).get();
      MPvec md = pMDev->GetMouseData();
      m_mouseData = md;
      g_pErr->Debug(pastestr::paste("ds", " ", (long) md.size(), "mouse samples copied"));

      /*
      MousePoint mp1;
      for (int i = 0; i < md.size(); i++) {
	mp1 = md[i];
	g_pErr->Debug(pastestr::paste("ddd", " ", (long) mp1.m_ms, (long) mp1.m_x, (long) mp1.m_y));
      }
      */
    } else {}
  } else {}

  g_pErr->DFO("Trial::Finish", m_id, 1);

  return 0;
}

int Trial::HandleEvent(SDL_Event * pEvt) {
  return m_pItemCell->HandleEvent(pEvt);
}

int Trial::Update() {
  return m_pItemCell->Update();
}

int Trial::Start() {
  int nResult = 0;
  g_pErr->DFI("Trial::Start", m_id, 2);
  nResult = m_pItemCell->Start();
  m_msBegin = ClockFn();
  m_nTrialStatus = SBX_IN_PROGRESS;
  m_bVisited = 1;
  g_pErr->DFO("Trial::Start", m_id, 2);
  return nResult;
}

int Trial::StoreData(long tnum, long idSess, long idSubj) {
  string s1;

  if (m_bVisited) {

    s1.assign(pastestr::paste("sdddddss", ", ",
			      "NULL", tnum, m_pItemCell->GetID(),
			      idSess, (long) m_msBegin, (long) m_msEnd, 
			      "NULL", "NULL"));

    long idTrial = g_prsResp->InsertGetID("Trial", s1.c_str());

    // create a new response entry
    s1.assign(pastestr::paste("sdd", ", ", "NULL", idSubj, idTrial));
    g_pErr->Debug(s1.c_str());
    long idResp = g_prsResp->InsertGetID("Response", s1.c_str());

    // if we have mouse data
    if (m_mouseData.size() > 0) {
      g_prsStim->BeginTransaction();
      for (int i = 0; i < m_mouseData.size(); i++) {
	g_prsResp->Insert("Mouse", m_mouseData[i].QueryStr(idResp));
      }
      g_prsStim->EndTransaction();
    } else {}

    // TODO: store the data from respData
    g_pErr->Debug(pastestr::paste("sds", " ", "respData has", (long) m_vRespData.size(), "events"));
    for (int i = 0; i < m_vRespData.size(); i++) {
      //m_vRespData[i].Print();
      m_vRespData[i].Store(idResp, g_prsResp);
    }

    // store the state + event data.
    m_pItemCell->StoreData(idTrial);

  } else {}

  return 0;
}

void Trial::SaveRespData(const RespData rd) {
  g_pErr->DFI("Trial::SaveRespData", m_id, 2);
  m_vRespData.push_back(rd);
  g_pErr->Debug(pastestr::paste("ds", " ", (long) rd.Size(), "events saved"));
  g_pErr->DFO("Trial::SaveRespData", m_id, 2);
}
