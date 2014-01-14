#include "WatchGamePadMove.hpp"
#include "pastestr.hpp"
#include "Template.hpp"


WatchGamePadMove::WatchGamePadMove(long id, long idNext, ArgMMap mmap) : Watch(id, idNext) {
  g_pErr->DFI("WatchGamePadMove::WatchGamePadMove", id, 4);

  pair<ArgIter, ArgIter> pii;
  ArgIter ii;

  m_pGamePad = NULL;

  int nAxis = 0;
  int nDir = 0;
  int nIndex = 0;

  m_nAxis = (Uint8) 0;
  m_nDir = (Uint8) 0;

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

  pii = mmap.equal_range("Direction");
  if (pii.first == pii.second) {
    g_pErr->Debug("watching for GamePad ANYMOVEMENT");
    // leave m_vButton empty
  } else {
    // TODO ; process direction
    for (ii = pii.first; ii != pii.second; ii++) {
      //from_string(nState, ii->second, std::dec);
      //m_vButton.push_back(nButton);
      //g_pErr->Debug(pastestr::paste("sd", " ", "gamepad watching for", ((long) nButton)));
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

  g_pErr->DFO("WatchGamePadMove::WatchGamePadMove", m_id, 4);
}

WatchGamePadMove::~WatchGamePadMove() {
  //m_vButton.clear();
}

bool WatchGamePadMove::CheckCondition(SDL_Event * pEvt) {
  // if we get this far, a joystick has received movement
  // just need to figure out which joystick and then process the direction
  bool bResult = false;  

  if ( CheckWhich(pEvt->jbutton.which) ) {
    if (abs(pEvt->jaxis.value) >= 8000) {
      bResult = true;
    } else {
    }
  } else {}

  return bResult;
}

bool WatchGamePadMove::CheckWhich(Uint8 nGamePadUsed) {
  bool bResult = false;

  int nIndex = m_pGamePad->GetIndex();

  if (nIndex == nGamePadUsed) {
    bResult = true;
  } else {}

  return bResult;
}
