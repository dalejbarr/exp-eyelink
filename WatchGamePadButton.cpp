#include "WatchGamePadButton.hpp"
#include "pastestr.hpp"
#include "Template.hpp"


WatchGamePadButton::WatchGamePadButton(long id, long idNext, ArgMMap mmap) : Watch(id, idNext) {
  g_pErr->DFI("WatchGamePadButton::WatchGamePadButton", id, 4);

  pair<ArgIter, ArgIter> pii = mmap.equal_range("Button");
  ArgIter ii;

  m_pGamePad = NULL;

  m_vButton.clear();
  int nButton = EXP_ANYGAMEPADBUTTON;
  int nIndex = 0;

  pii = mmap.equal_range("GamePadIndex");
  if (pii.first == pii.second) {
    // only gamepad
    if (SDL_NumJoysticks() != 1) {
      g_pErr->Report(pastestr::paste("sdsds", " ", "looking for", 1L, "joysticks:", (long) SDL_NumJoysticks(), "found"));
    } else {
      m_pGamePad = ((GamePad_SDL *) Template::FindOrCreateInputDev(SBX_GAMEPAD_DEV, 0).get());
      if (!m_pGamePad) {
	g_pErr->Report("couldn't initialize gamepad!!");
      } else {}
    }
  } else {
    // specific gamepad
    if (SDL_NumJoysticks() == 0) {
      g_pErr->Report("no gamepads found!!");
    } else {
      from_string(nIndex, (pii.first)->second, std::dec);
      m_pGamePad = ((GamePad_SDL *) Template::FindOrCreateInputDev(SBX_GAMEPAD_DEV, nIndex).get());
    }
  }

  m_vButton.clear();
  pii = mmap.equal_range("Button");
  if (pii.first == pii.second) {
    g_pErr->Debug("watching for GamePad ANYBUTTON down");
    // leave m_vButton empty
  } else {
    for (ii = pii.first; ii != pii.second; ii++) {
      from_string(nButton, ii->second, std::dec);
      m_vButton.push_back(nButton);
      g_pErr->Debug(pastestr::paste("sd", " ", "gamepad watching for", ((long) nButton)));
    }
  }

  /*
  pii = mmap.equal_range("State");
  if (pii.first == pii.second) {
    m_nState = SDL_PRESSED;
  } else {
    ii = pii.first;
    if ((*ii).second=="up") {
      m_nState = SDL_RELEASED;
    } else {
      if ((*ii).second=="down") {
	m_nState = SDL_PRESSED;
      } else {
	g_pErr->Report(pastestr::paste("sd", "", "error parsing argument 'State' for WatchID ", m_id));
      }
    }
    g_pErr->Debug(pastestr::paste("dsd", "", GetID(), " watch is looking for state ", m_nState));
  }
  */

  g_pErr->DFO("WatchGamePadButton::WatchGamePadButton", m_id, 4);
}

WatchGamePadButton::~WatchGamePadButton() {
  m_vButton.clear();
}

bool WatchGamePadButton::TargetButton(Uint8 nButton /* = EXP_ANYGAMEPADBUTTON */) {
  bool bResult = false;

  if (m_vButton.size() == 0) {
    bResult = true;
  } else {
    for (int i = 0; i < m_vButton.size(); i++) {
      if ( ((Uint8) m_vButton[i]) == nButton ) {
	bResult = true;
	break;
      } else {}
    }
  }

  return bResult;
}

bool WatchGamePadButton::CheckCondition(SDL_Event * pEvt) {
  // if we get this far, a button has been pressed; just have to check which one
  bool bResult = false;  


  if ( CheckWhich(pEvt->jbutton.which) ) {

    if (m_vButton.size() == 0) {
      bResult = true;
    } else {
      bResult = TargetButton((*pEvt).jbutton.button);
    }

  } else {}

  return bResult;
}

bool WatchGamePadButton::CheckWhich(Uint8 nGamePadUsed) {
  bool bResult = false;

  int nIndex = m_pGamePad->GetIndex();

  if (nIndex == nGamePadUsed) {
    bResult = true;
  } else {}

  return bResult;
}
