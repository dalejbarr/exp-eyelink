#include "Stimulus.hpp"
#include "Template.hpp"
#include "ItemCell.hpp"

ItemCell * Stimulus::g_pCurItemCell = NULL;

Stimulus::Stimulus(long id, Template * pTemplate) {
  m_lgDevType = 1; m_lgDevInstance = 1; m_id = id;
  m_pTemplate = pTemplate;
  //m_sFileName.assign("");
  m_sResource.assign("");
  m_sFileName.assign("");
}

Stimulus::~Stimulus() {
}

int Stimulus::Prepare() {
  g_pErr->DFI("Stimulus::Prepare", m_id, 2);
  if (m_pTemplate) {
    m_sFileName = m_sResource; //m_pTemplate->ParseResource(m_sResource);
  } else {
    g_pErr->Debug("template pointer not defined");
  }
  g_pErr->DFO("Stimulus::Prepare", m_id, 2);
  return 0;
}

int Stimulus::Action() { 
  g_pErr->DFI("Stimulus::Action", m_id, 2);
  g_pErr->DFO("Stimulus::Action", m_id, 2);
  return 0; 
}

string Stimulus::GetResourceString(string s1) {
  if (s1[0] == '$') {
    if (!g_pCurItemCell) {
      g_pErr->Report("Stimulus::GetResourceString; g_pCurItemCell undefined!");
    } else {}
    s1 = g_pCurItemCell->m_mapResources[s1.substr(1, s1.size()-1)];
  } else {
    s1 = s1;
  }

  return s1;
}

string Stimulus::GetFilenameFromResource(string s1) {

  if (s1[0] == '$') {
    if (!g_pCurItemCell) {
      g_pErr->Report("Stimulus::Prepare; g_pCurItemCell undefined!");
    } else {}
    s1 = "resource/" + g_pCurItemCell->m_mapResources[s1.substr(1, s1.size()-1)];
  } else {
    s1 = "resource/" + s1;
  }

  return s1;
}
