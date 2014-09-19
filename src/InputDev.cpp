#include "InputDev.hpp"
#include "ItemCell.hpp"
#include <iostream>
#include "Experiment.hpp"

extern Experiment * g_pExperiment;

ItemCell * InputDev::g_pCurItemCell = NULL;

InputDev::InputDev(unsigned long idDev, int nIndex) {
  m_bIsRecording = 0;
  m_idDev = idDev;
  m_nIndex = nIndex;
  m_sOutputFile.assign("");
}

InputDev::~InputDev() {
}

void InputDev::Prepare() {
  g_pErr->DFI("InputDev::Prepare", (long) m_idDev, 2);
  m_respData.Clear();
  g_pErr->DFO("InputDev::Prepare", (long) m_idDev, 2);
}

void InputDev::Start() {
  g_pErr->DFI("InputDev::Start", (long) m_idDev, 2);
  m_bIsRecording = 1;
  g_pErr->DFO("InputDev::Start", (long) m_idDev, 2);
}

void InputDev::Stop() {
  g_pErr->DFI("InputDev::Stop", (long) m_idDev, 2);
  m_bIsRecording = 0;
  g_pErr->DFO("InputDev::Stop", (long) m_idDev, 2);
}

void InputDev::HandleEvent(SDL_Event * pEvt) {
}

void InputDev::SetOutputFile(const char * pFile) {
  m_sOutputFile.assign(pFile);
}

void InputDev::ParseFilename(const char * pFile) {

  string dir = g_pExperiment->GetResponseDir();

  if (m_sOutputFile[0] == '$') {
    if (!g_pCurItemCell) {
      g_pErr->Report("InputDev::ParseFilename, g_pCurItemCell not defined!");
    } else {
      m_sOutputFile = dir + "/" + g_pCurItemCell->GetFilenameFromResource(pFile);
    }
  } else {
    m_sOutputFile = dir + "/" + m_sOutputFile;
  }

  g_pErr->Debug(pastestr::paste("ss", "", "saving file to ", m_sOutputFile.c_str()));
}
