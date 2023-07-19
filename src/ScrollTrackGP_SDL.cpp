#include "ScrollTrackGP_SDL.hpp"
#include "pastestr.hpp"

ScrollTrackGP_SDL::ScrollTrackGP_SDL(unsigned long idDev, int nIndex) : GamePad_SDL(idDev, nIndex) {
  g_pErr->DFI("ScrollTrackGP_SDL::ScrollTrackGP_SDL", 0L, 1);
  m_pDisp = NULL;
  g_pErr->DFO("ScrollTrackGP_SDL::ScrollTrackGP_SDL", 0L, 1);
}

ScrollTrackGP_SDL::~ScrollTrackGP_SDL() {
  g_pErr->DFI("ScrollTrackGP_SDL::~ScrollTrackGP_SDL", 0L, 1);
  if (m_pDisp) {
    delete m_pDisp;
    m_pDisp = NULL;
  } else {}
  g_pErr->DFO("ScrollTrackGP_SDL::~ScrollTrackGP_SDL", 0L, 1);
}

void ScrollTrackGP_SDL::Prepare() {
  if (!m_pDisp) {
    m_pDisp = new StimulusImg(0, NULL, "scrolltrack", 156, 28, -1, -1, -1);
    //m_pDisp->Highlight("2 255 255 255");
  } else {}
  g_pErr->Debug(pastestr::paste("ss", " ", "ScrollTrackGP resource is", 
				m_pDisp->GetFilenameFromResource(m_sResource).c_str()));
  m_pDisp->Load(m_pDisp->GetFilenameFromResource(m_sResource).c_str());

  m_rect.x = 227; m_rect.y = 227; m_rect.w = 260; m_rect.h = 260;
  m_rectOld.x = m_rect.x; m_rectOld.y = m_rect.y;
  m_rectOld.w = m_rect.w; m_rectOld.h = m_rectOld.h;
  m_rectViewPort.x = 382; m_rectViewPort.y = 254;
  m_rectViewPort.w = 260; m_rectViewPort.h = 260;

  GamePad_SDL::Prepare();
}

void ScrollTrackGP_SDL::Start() {  
  SDL_Surface * pScreen = Display_SDL::LockScreen();
  SDL_Rect r2;
  r2.x = m_rectViewPort.x-2; r2.y = m_rectViewPort.y-2;
  r2.w = m_rectViewPort.w+4; r2.h = m_rectViewPort.h+4;
  SDL_FillRect(pScreen, &r2, SDL_MapRGB(pScreen->format, 255, 255, 255));
	Display_SDL::UnlockScreen();
  Redraw(true);
  GamePad_SDL::Start();
}

void ScrollTrackGP_SDL::Redraw(bool bForce) {
  static SDL_Surface * pScreen  =NULL;
  static int distx = 0;
  static int disty = 0;
  static int nsteps = 6;
  static float fx = 0.0f;
  static float fy = 0.0f;
  static SDL_Rect r1;
  static int frameint = 20;

  if ((m_rect.x != m_rectOld.x) || (m_rect.y != m_rectOld.y) || (bForce)) {
    distx = m_rect.x - m_rectOld.x;
    disty = m_rect.y - m_rectOld.y;
    fx = ((float) distx) / nsteps;
    fy = ((float) disty) / nsteps;
    r1.w = m_rect.w; r1.h = m_rect.h;
    for (int i = 1; i <= 6; i++) {
      r1.x = (int) (m_rectOld.x + i*fx);
      r1.y = (int) (m_rectOld.y + i*fy);
			pScreen = Display_SDL::LockScreen();
      SDL_BlitSurface(m_pDisp->GetSurface(), &r1, pScreen, &m_rectViewPort);
			Display_SDL::UnlockScreen();
      Display_SDL::Flip1();
      SDL_Delay(frameint);
    }
  } else {
    // rectangle did not change
  }
  m_rectOld.x = m_rect.x; m_rectOld.y = m_rect.y;
}

void ScrollTrackGP_SDL::Process(RespObj gp) {
  g_pErr->DFI("ScrollTrackGP_SDL::Process", 0L, 1);
  switch (gp.m_nButton) {
  case GP_TOP_BUTTON :
    {
      m_rect.x = 227;
      m_rect.y = 0;
    }
    break;
  case GP_BOTTOM_BUTTON :
    {
      m_rect.x = 227;
      m_rect.y = 454;
    }
    break;
  case GP_LEFT_BUTTON :
    {
      m_rect.x = 0;
      m_rect.y = 227;
    }
    break;
  case GP_RIGHT_BUTTON :
    {
      m_rect.x = 454;
      m_rect.y = 227;
    }
    break;
  }
  Redraw();
  g_pErr->DFO("ScrollTrackGP_SDL::Process", 0L, 1);
}

void ScrollTrackGP_SDL::SetResource(string s) {
  m_sResource = s;
}
