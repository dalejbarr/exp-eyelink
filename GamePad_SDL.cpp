#include "GamePad_SDL.hpp"
#include "pastestr.hpp"

GamePad_SDL::GamePad_SDL(unsigned long idDev, int nIndex) : InputDev(idDev, nIndex) {
  g_pErr->DFI("GamePad_SDL::GamePad_SDL", 0L, 1);
  m_bRecMode = false;
  //m_nIndex = nIndex;
  m_stick = SDL_JoystickOpen(nIndex);
  if (m_stick == NULL) {
    g_pErr->Report("Game controller not found: did you plug one in?");
  } else {}
  m_bIsRecording = false;
  g_pErr->DFO("GamePad_SDL::GamePad_SDL", 0L, 1);
}

GamePad_SDL::~GamePad_SDL() {
  g_pErr->DFI("GamePad_SDL::~GamePad_SDL", 0L, 1);
  Dealloc();
  g_pErr->DFO("GamePad_SDL::~GamePad_SDL", 0L, 1);
}

void GamePad_SDL::Dealloc() {
  g_pErr->DFI("GamePad_SDL::Dealloc", 0L, 1);
  if (m_stick) {
    SDL_JoystickClose(m_stick);
    m_stick = NULL;
  } else {}
  g_pErr->DFO("GamePad_SDL::Dealloc", 0L, 1);
}

void GamePad_SDL::Prepare() {
  m_bIsRecording = false;
  //m_vpEvents.clear();
  InputDev::Prepare();
}

void GamePad_SDL::Start() {
  m_bIsRecording = false;
  if (m_bRecMode) {
    m_bIsRecording = true;
  } else {}
  InputDev::Start();
}

void GamePad_SDL::Stop() {
  m_bIsRecording = false;
}

void GamePad_SDL::HandleEvent(SDL_Event * pEvt) {
  static RespObj gp;
  static SDL_JoyButtonEvent * pJEvt = NULL;

  if (pEvt->type == SDL_JOYBUTTONDOWN) {
    g_pErr->DFI("GamePad_SDL::HandleEvent", (long) m_nIndex, 1);
    if (m_bIsRecording) {
      pJEvt = (SDL_JoyButtonEvent *) pEvt;
      if (pJEvt->which == m_nIndex) {
	gp.m_nIndex = m_nIndex;
	gp.m_nButton = pJEvt->button;
	gp.m_Msec = ClockFn();
	g_pErr->Debug(pastestr::paste("sddd", " ", "button", (long) gp.m_nIndex,
				      (long) gp.m_nButton, (long) gp.m_Msec));
	m_respData.AddEvent(gp);
	Process(gp);
      } else {}
    } else {} // do nothing
    g_pErr->DFO("GamePad_SDL::HandleEvent", (long) m_nIndex, 1);
  } else {}
}

void GamePad_SDL::Process(RespObj gp) {
}
