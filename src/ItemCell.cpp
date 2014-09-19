#include "ItemCell.hpp"
#include "global.hpp"
#include "Trial.hpp"
#include "InputDev.hpp"

#include "pastestr.hpp"
using pastestr::paste;


ItemCell::ItemCell(long id, TemplatePtr ptt) {
  m_id = id;
  m_pTemplate = ptt;
  m_pCurTrial = NULL;
}

ItemCell::~ItemCell() {
  g_pErr->DFI("ItemCell::~ItemCell", m_id, 2);
  m_mapResources.clear();
  m_mmapAOI.clear();
  m_vEventTiming.clear();
  m_vStateTiming.clear();

  g_pErr->DFO("ItemCell::~ItemCell", m_id, 2);
}

int ItemCell::Load() {
  LoadAOIs();
  LoadResources();

  return 0;
}

int ItemCell::LoadAOIs() {
  g_pErr->DFI("ItemCell::LoadAOIs", m_id, 2);

  //LoadAOI<ItemCell>(this, "item", m_pTemplate.get());
  LoadAOI<ItemCell>(this, "itemcell", m_pTemplate.get());

  g_pErr->DFO("ItemCell::LoadAOIs", m_id, 2);

  return 0;
}

int ItemCell::LoadResources() {
  g_pErr->DFI("ItemCell::LoadResources", m_id, 2);

  string q = paste("sds", " ", "\
SELECT Code, Data \n\
FROM Resource \n\
WHERE ItemCellID=", m_id, "ORDER BY Code");

  g_prsStim->Exec(q);

  //string s;
  while (g_prsStim->RowReady()) {
    m_mapResources[g_prsStim->Get(0)] = g_prsStim->Get(1);
    g_pErr->Debug(pastestr::paste("sss", "",
				  g_prsStim->Get(0), "=", m_mapResources[g_prsStim->Get(0)].c_str()));
    g_prsStim->MoveNext();
  }

  g_pErr->DFO("ItemCell::LoadResources", m_id, 2);

  return 0;
}

int ItemCell::Prepare() {
  m_pTemplate->SetCurItemCell(this);
  m_pTemplate->AddAOIs(m_mmapAOI);
  //m_pTemplate->SetResourceMap(m_mapResources);
  Stimulus::g_pCurItemCell = this;
  InputDev::g_pCurItemCell = this;
  m_pTemplate->Prepare();

  return 0;
}

int ItemCell::Finish() {
  g_pErr->DFI("ItemCell::Finish", m_id, 3);
  // TO DO: store the state + event data.

  if (m_pCurTrial) {
    //g_pErr->Report("defined");
  } else {
    g_pErr->Report("ItemCell::Finish curtrial undefined");
  }

  m_vStateTiming = m_pTemplate->GetStateTiming();
  m_vEventTiming = m_pTemplate->GetEventTiming();
  m_msgMap = m_pTemplate->m_msgMap;
  m_vRespData = m_pTemplate->m_vRespData;

  //m_vGSC1SoundButton = m_pTemplate->m_vGSC1SoundButton;
  //m_vGSC1Toggle = m_pTemplate->m_vGSC1Toggle;
  //m_vGSC1Cursor = m_pTemplate->m_vGSC1Cursor;
  //m_vGSC1Stars = m_pTemplate->m_vGSC1Stars;
  //m_vGSC1Flags = m_pTemplate->m_vGSC1Flags;

  g_pErr->DFO("ItemCell::Finish", m_id, 3);
  return m_pTemplate->Finish();
}

int ItemCell::Run() {
  m_pTemplate->Run();
}

int ItemCell::HandleEvent(SDL_Event * pEvt) {
  return m_pTemplate->HandleEvent(pEvt);
}

int ItemCell::Update() {
  return m_pTemplate->Update();
}

int ItemCell::Start() {
  g_pErr->DFI("ItemCell::Start", m_id, 2);
  return m_pTemplate->Start();
  g_pErr->DFO("ItemCell::Start", m_id, 2);
}

int ItemCell::StoreData(long idTrial) {
  g_pErr->DFI("ItemCell::StoreData", m_id, 3);

  string s1;

  if (m_vStateTiming.size() > 0) {
    g_prsResp->BeginTransaction();
    for (int i = 0; i < m_vStateTiming.size(); i++) {
      s1.assign(pastestr::paste("ddd", ", ", idTrial, m_vStateTiming[i]->m_id,
				m_vStateTiming[i]->m_ms));
      g_prsResp->Insert("StateTiming", s1.c_str());
    }
    g_prsResp->EndTransaction();
  } else {}  

  if (m_vEventTiming.size() > 0) {
    g_prsResp->BeginTransaction();
    for (int i = 0; i < m_vEventTiming.size(); i++) {
      s1.assign(pastestr::paste("ddd", ", ", idTrial, m_vEventTiming[i]->m_id,
				m_vEventTiming[i]->m_ms));
      g_prsResp->Insert("EventTiming", s1.c_str());
    }
    g_prsResp->EndTransaction();
  } else {}  

  if (m_msgMap.size() > 0) {
    g_prsResp->BeginTransaction();
    for (MsgMap::iterator ii = m_msgMap.begin();
	 ii != m_msgMap.end(); ii++) {
      s1.assign(pastestr::paste("dsdsssss", "", idTrial, ", ", (long) (*ii).first, ", '", 
				(*ii).second.first.c_str(), "', '", (*ii).second.second.c_str(), "'"));
      g_prsResp->Insert("Msg", s1.c_str());
    }
    g_prsResp->EndTransaction();
  } else {}

  // TODO pass up response data to trial
  

  /*
  if (m_vGSC1SoundButton.size() > 0) {
    g_prsResp->BeginTransaction();
    for (int i = 0; i < m_vGSC1SoundButton.size(); i++) {
      s1.assign(pastestr::paste("ddd", ", ", idTrial, m_vGSC1SoundButton[i].m_ms, m_vGSC1SoundButton[i].m_id));
      g_prsResp->Insert("GSC1SoundButton", s1.c_str());
    }
    g_prsResp->EndTransaction();
  }

  if (m_vGSC1Toggle.size() > 0) {
    g_prsResp->BeginTransaction();
    for (int i = 0; i < m_vGSC1Toggle.size(); i++) {
      s1.assign(pastestr::paste("ddddd", ", ", 
				idTrial, 
				m_vGSC1Toggle[i].m_ms, m_vGSC1Toggle[i].m_xyCell.m_y,
				m_vGSC1Toggle[i].m_xyCell.m_x, m_vGSC1Toggle[i].m_nFlag));
      g_prsResp->Insert("GSC1Toggle", s1.c_str());
    }
    g_prsResp->EndTransaction();
  }

  if (m_vGSC1Cursor.size() > 0) {
    g_prsResp->BeginTransaction();
    for (int i = 0; i < m_vGSC1Cursor.size(); i++) {
      s1.assign(pastestr::paste("dddd", ", ", 
				idTrial, 
				m_vGSC1Cursor[i].m_ms, m_vGSC1Cursor[i].m_xyCell.m_y,
				m_vGSC1Cursor[i].m_xyCell.m_x));
      g_prsResp->Insert("GSC1Cursor", s1.c_str());
    }
    g_prsResp->EndTransaction();
  }

  if (m_vGSC1Stars.size() > 0) {
    g_prsResp->BeginTransaction();
    for (int i = 0; i < m_vGSC1Stars.size(); i++) {
      s1.assign(pastestr::paste("ddd", ", ", 
				idTrial, 
				m_vGSC1Stars[i].m_nRow,
				m_vGSC1Stars[i].m_nCol));
      g_prsResp->Insert("GSC1Stars", s1.c_str());
    }
    g_prsResp->EndTransaction();
  }

  if (m_vGSC1Flags.size() > 0) {
    g_prsResp->BeginTransaction();
    for (int i = 0; i < m_vGSC1Flags.size(); i++) {
      s1.assign(pastestr::paste("ddd", ", ", 
				idTrial, 
				m_vGSC1Flags[i].m_nRow,
				m_vGSC1Flags[i].m_nCol));
      g_prsResp->Insert("GSC1Flags", s1.c_str());
    }
    g_prsResp->EndTransaction();
  }
*/
  
  g_pErr->DFO("ItemCell::StoreData", m_id, 3);
  return 0;
}

string ItemCell::GetFilenameFromResource(string s1, bool prepend /* = false*/) {

  string sp1; 

  if (prepend) {
    sp1 = "resource/";
  } else {
    sp1 = "";
  }

  if (s1[0] == '$') {
    s1 = sp1 + m_mapResources[s1.substr(1, s1.size()-1)];
  } else {
    s1 = sp1 + s1;
  }

  return s1;
}
