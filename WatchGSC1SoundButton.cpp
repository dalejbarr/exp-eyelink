#include "WatchGSC1SoundButton.hpp"
#include "Template.hpp"

WatchGSC1SoundButton::WatchGSC1SoundButton(long id, long idNext, ArgMMap mmap, Template * pTemplate) :
  WatchGamePadButton(id, idNext, mmap) {

  g_pErr->DFI("WatchGSC1SoundButton::WatchGSC1SoundButton", 2L, 2);

  string sFileName;
  m_pTemplate = pTemplate;

  if (m_pGamePad->GetIndex() == 1) {
    sFileName.assign("beepD.wav");
  } else {
    sFileName.assign("beepE.wav");
  }

  m_pStim = StimulusPtr(new StimulusWav(0, NULL, sFileName.c_str()));

  g_pErr->DFO("WatchGSC1SoundButton::WatchGSC1SoundButton", 2L, 2);
}

bool WatchGSC1SoundButton::CheckCondition(SDL_Event * pEvt) {

  g_pErr->DFI("WatchGSC1SoundButton::CheckCondition", 2L, 2);

  if (WatchGamePadButton::CheckCondition(pEvt)) {
    // beep
    if (m_pStim.get()) {
      m_vPress.push_back(GSC1SoundButton((int) m_pGamePad->GetIndex(), ClockFn()));
      m_pStim->Action();
    } else {}
  } else {}

  g_pErr->DFO("WatchGSC1SoundButton::CheckCondition", 2L, 2);

  return false;
}

int WatchGSC1SoundButton::Prepare() {

  m_vPress.clear();
  m_pStim->Prepare();

  return 0;
}

int WatchGSC1SoundButton::Cleanup() {
  WatchGamePadButton::Cleanup();
  m_pStim->Finish();
  return 0;
}

WatchGSC1SoundButton::~WatchGSC1SoundButton() {
  Cleanup();
  m_pStim.reset();
}

int WatchGSC1SoundButton::PostTrial() {
  // TO DO: copy over to template
  //RespData rd("GSC1SoundButton", m_vPress);

  //m_pTemplate->m_vRespData.push_back(rd);
  
  g_pErr->DFI("WatchGSC1SoundButton::PostTrial", 0L, 3);

  m_pTemplate->m_vGSC1SoundButton.insert(m_pTemplate->m_vGSC1SoundButton.end(),
					 m_vPress.begin(), m_vPress.end());

  g_pErr->DFO("WatchGSC1SoundButton::PostTrial", 0L, 3);

  return Watch::Finish();
}
