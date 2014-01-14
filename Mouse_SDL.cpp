#include "Mouse_SDL.hpp"
#include "Template.hpp"
#include "WatchMouse.hpp"

unsigned int Mouse_SDL::s_x0 = 0;
unsigned int Mouse_SDL::s_y0 = 0;
unsigned int Mouse_SDL::s_xHome = 512;
unsigned int Mouse_SDL::s_yHome = 384;

MousePoint::MousePoint() {
}

MousePoint::MousePoint(Uint32 ms, int x, int y) {
  m_ms = ms; m_x = x; m_y = y;
}

Mouse_SDL::Mouse_SDL(unsigned long idDev, bool bDraw, unsigned int xHome, unsigned int yHome) : InputDev(idDev, 0) {
  g_pErr->DFI("Mouse_SDL::Mouse_SDL", 0L, 3);
  m_bDraw = bDraw;
  m_xLast = 0; m_yLast = 0;
  m_bFirst = 0;
  m_pCursor = NULL;
  //m_pTemplate = pTemplate;
  m_pOld = NULL;
  g_pErr->DFO("Mouse_SDL::Mouse_SDL", 0L, 3);
}

Mouse_SDL::~Mouse_SDL() {
  g_pErr->DFI("Mouse_SDL::~Mouse_SDL", 0L, 3);
  if (m_pCursor) {
    delete m_pCursor;
    m_pCursor = NULL;
  } else {}

  if (m_pOld) {
    SDL_FreeSurface(m_pOld);
    m_pOld = NULL;
  } else {}

  g_pErr->DFO("Mouse_SDL::~Mouse_SDL", 0L, 3);
}

void Mouse_SDL::Prepare() {
  g_pErr->DFI("Mouse_SDL::Prepare", 0L, 3);

  if (m_bDraw) {
    m_xLast = s_xHome; m_yLast = s_yHome;
    if (!m_pCursor) {
      m_pCursor = new StimulusBmp(0, NULL, "mouse", s_xHome, s_yHome, -1, -1, -1);
      //m_pCursor = new StimulusBmp(0, m_pTemplate, "mouse", 512, 384, -1, -1, -1);
      m_pCursor->SetColorkey(128, 0, 255);
    } else {}

    m_pCursor->LoadBMP("mouse.bmp");
    //m_pCursor->ConvertSurface(g_pDisplay->GetSDLSurface());
    m_pCursor->m_CurX.Set(s_xHome);
    m_pCursor->m_CurY.Set(s_yHome);
    m_xLast = s_xHome; m_yLast = s_yHome;

    if (!m_pOld) {
      m_pOld = SDL_DisplayFormat(m_pCursor->GetSurface());
      //const SDL_PixelFormat& fmt = *(g_pDisplay->GetSDLScreen()->format);
      //m_pOld = SDL_CreateRGBSurface(SDL_HWSURFACE, m_pCursor->m_rect.w, m_pCursor->m_rect.h,
      //fmt.BitsPerPixel, fmt.Rmask, fmt.Gmask, fmt.Bmask, fmt.Amask);
    } else {}
  } else {
  }

  Cleanup();
  g_pErr->DFO("Mouse_SDL::Prepare", 0L, 3);
}

void Mouse_SDL::NewPt(Uint32 ms, int x, int y) {
  static int old_x = 0;
  static int old_y = 0;

  if (m_bFirst) {
    if (m_bDraw) {
      old_x = m_xLast; old_y = m_yLast;
      m_xLast += x; m_yLast += y;
      m_vPts.push_back(MousePoint(ms, m_xLast, m_yLast));
      m_pCursor->m_CurX.Set(m_xLast);
      m_pCursor->m_CurY.Set(m_yLast);
      //m_pTemplate->Redraw();
      DrawCursor(old_x, old_y);
    } else {
      m_xLast = Mouse_SDL::s_x0 + x;
      m_yLast = Mouse_SDL::s_y0 + y;
      m_vPts.push_back(MousePoint(ms, m_xLast, m_yLast));
    }
  } else {
    if (!m_bDraw) {
      Mouse_SDL::s_x0 = x;
      Mouse_SDL::s_y0 = y;
      m_xLast = x;
      m_yLast = y;
    } else {}
    // throw out the first sample
    //m_vPts.push_back(MousePoint(ms, m_x0 + x, m_y0 + y));
    m_bFirst = 1;
  }
}

void Mouse_SDL::HandleEvent(SDL_Event * pEvt) {
  if (m_bIsRecording) {
    switch (pEvt->type) {
    case SDL_MOUSEMOTION :
      {
	NewPt(ClockFn(), pEvt->motion.xrel, pEvt->motion.yrel);
	break;
      }
    }
  } else {
    if (!m_bDraw) {
      m_xLast = Mouse_SDL::s_x0 + pEvt->motion.xrel;
      m_yLast = Mouse_SDL::s_y0 + pEvt->motion.yrel;
    } else {}
  }
}

void Mouse_SDL::Cleanup() {
  g_pErr->DFI("Mouse_SDL::Cleanup", 0L, 3);
  m_vPts.clear();
  if (m_bDraw) {
    m_xLast = s_xHome; m_yLast = s_yHome;
  } else {}
  WatchMouse::s_pMouse = NULL;
  g_pErr->DFO("Mouse_SDL::Cleanup", 0L, 3);
}

void Mouse_SDL::Start() {
  WatchMouse::s_pMouse = this;
  //m_rectOld.x = 0; m_rectOld.y = 0;
  //m_rectOld.w = m_pCursor->m_rect.w; m_rectOld.h = m_pCursor->m_rect.h;

  // the first time we need to store the part of the screen in the back buffer
  if (m_bDraw) {
    m_rect.x = m_xLast; m_rect.y = m_yLast;
    m_rect.w = m_pCursor->m_rect.w; m_rect.h = m_pCursor->m_rect.h;
    SDL_BlitSurface(g_pDisplay->GetSDLScreen(), &m_rect, m_pOld, NULL);
    m_pCursor->Draw();
    StimulusBmp::Flip1();
  } else {}

  InputDev::Start();
}

void Mouse_SDL::DrawCursor(int old_x, int old_y) {
  //SDL_BlitSurface(g_pDisplay->GetSDLScreen(), 
  static SDL_Rect r1;

  if (m_bDraw) {
    m_rect.x = old_x; m_rect.y = old_y;

    // erase old one
    SDL_BlitSurface(m_pOld, NULL, g_pDisplay->GetSDLScreen(), &m_rect);  
    //StimulusBmp::Flip();

    // store background before drawing new one
    r1.w = m_rect.w; r1.h = m_rect.h;
    r1.x = m_pCursor->m_CurX.Get();
    r1.y = m_pCursor->m_CurY.Get();
    SDL_BlitSurface(g_pDisplay->GetSDLScreen(), &r1, m_pOld, NULL);  

    // now draw
    m_pCursor->Draw();
    StimulusBmp::Flip1();
  } else {}
}

const char * MousePoint::QueryStr(long idResp) {
  string s1;
  s1.assign(pastestr::paste("dddds", ", ",
			    idResp, (long) m_ms, (long) m_x, (long) m_y, "NULL"));
  return s1.c_str();
}

void Mouse_SDL::SetHome(const char * pcHome) {
  istringstream iss(pcHome);

  iss >> s_xHome >> s_yHome;
}
