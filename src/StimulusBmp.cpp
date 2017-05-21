#include "StimulusBmp.hpp"
#include "global.hpp"
#include "pastestr.hpp"

SDL_Surface * StimulusBmp::m_pScreen = NULL;
SDL_Surface * StimulusBmp::m_pMemSurface = NULL;

StimulusBmp::StimulusBmp(long id, Template * pTemplate,
			 long idCmd, ArgMMap mmArgs) :
  Stimulus(id, pTemplate) {

  if (!m_pMemSurface && m_pScreen) {
    // only initialize the memory surface once
    InitMemSurface();
  } else {}

  // parse out all common arguments
  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("Resource");
  ArgIter ii;

  ii = pii.first;
  m_sResource = (*ii).second;

  g_pErr->DFI("StimulusBmp::StimulusBmp(mmArgs)", m_sResource.c_str(), 4);

  pii = mmArgs.equal_range("x"); ii = pii.first;
  m_x1 = atoi((*ii).second.c_str());
  m_CurX.Set(m_x1);

  pii = mmArgs.equal_range("y"); ii = pii.first;
  m_y1 = atoi((*ii).second.c_str());
  m_CurY.Set(m_y1);

  // now do specific arguments
  pii = mmArgs.equal_range("Layer"); 
  if (pii.first != pii.second) {
    ii = pii.first; m_nLayer = atoi((*ii).second.c_str());
  } else {
    m_nLayer = 1;
  }

  Initialize();

  pii = mmArgs.equal_range("ColorKey"); 
  if (pii.first != pii.second) {
    ii = pii.first;
    int r, g, b;
    istringstream iss((*pii.first).second);
    iss >> r >> g >> b;
    g_pErr->Debug(pastestr::paste("sddd", " ", "set colorkey", (long) r, (long) g, (long) b));
    SetColorkey(r, g, b);      
    //g_pErr->Report(pastestr::paste("ddd", " ", (long) r, (long) g, (long) b));
    //m_nLayer = atoi((*ii).second.c_str());
  } else {
    //m_nLayer = 1;
  }


  g_pErr->DFO("StimulusBmp::StimulusBmp(mmArgs)", m_sResource.c_str(), 4);  
}

StimulusBmp::StimulusBmp(long id, Template * pTemplate, const char * pName, 
			 int x1, int y1, int x2, int y2, int nLayer) :
  Stimulus(id, pTemplate) {
  g_pErr->DFI("StimulusBmp::StimulusBmp", pName, 3);
  m_sResource.assign(pName);
  if ((m_sResource == "") && (x2 == -1) && (y2 == -1)) {
    g_pErr->Report("can't have blank AOI Name and missing x2,y2 coords!!");
  } else {}
  m_x1 = x1; m_y1 = y1; m_x2 = x2; m_y2 = y2;
  m_CurX.Set(x1);
  m_CurY.Set(y1);
  m_nLayer = nLayer;

  Initialize();
  
  g_pErr->DFO("StimulusBmp::StimulusBmp", pName, 3);
}

void StimulusBmp::Initialize() {
  m_rect.x = m_x1;
  m_rect.y = m_y1;
  m_rect.w = m_x2-m_x1; m_rect.h = m_y2-m_y1;
  m_CurX.Set(m_x1);
  m_CurY.Set(m_y1);
  //m_px = &m_CurX;
  //m_py = &m_CurY;

  m_bColorKey = 0;
  m_bHighlight = 0;
  m_nHighlightWidth = 2;
  m_nHR = 255; m_nHG = 255; m_nHB = 255;
  m_r = 0; m_g = 0; m_b = 0;

  m_pSurface = NULL;
}

StimulusBmp::~StimulusBmp() {
  g_pErr->DFI("StimulusBmp::~StimulusBmp", m_sResource.c_str(), 3);
  g_pErr->DFO("StimulusBmp::~StimulusBmp", m_sResource.c_str(), 3);

  if (m_pMemSurface) {
    SDL_FreeSurface(m_pMemSurface);
    m_pMemSurface = NULL;
  } else {}
}

int StimulusBmp::Prepare() {
  Stimulus::Prepare();
  InitMemSurface();
  // load it in
  g_pErr->DFI("StimulusBmp::Prepare", m_sResource.c_str(), 3);
  if (m_sFileName == "") {
    g_pErr->Debug("blank");
    return 1;
  } else {}

  string s1 = Stimulus::GetFilenameFromResource(m_sResource.c_str());
  LoadBMP(s1.c_str());

  g_pErr->DFO("StimulusBmp::Prepare", m_sFileName.c_str(), 3);
  return 0;
}

int StimulusBmp::Finish() {
  g_pErr->DFI("StimulusBmp::Finish", m_sResource.c_str(), 3);
  
  if (m_pSurface) {
    SDL_FreeSurface(m_pSurface);
  } else {}
  m_pSurface = NULL;

  Stimulus::Finish();

  g_pErr->DFO("StimulusBmp::Finish", m_sResource.c_str(), 3);
  return 0;
}

int StimulusBmp::Draw(bool bMem /*=0*/) {
  //g_pErr->DFI("StimulusBmp::Draw", m_sFileName.c_str(), 3);
  SDL_Rect r1;
  SDL_Surface * pSurf = m_pScreen;
  r1.w = m_rect.w; r1.h = m_rect.h;
  r1.x = m_rect.x; r1.y = m_rect.y;

  if (bMem) {
    pSurf = m_pMemSurface;
  } else {}

  if (m_pSurface) {
    if (m_bHighlight) {
      r1.w = m_rect.w + 2*m_nHighlightWidth;
      r1.h = m_rect.h + 2*m_nHighlightWidth;  
      r1.x = m_rect.x - m_nHighlightWidth;
      r1.y = m_rect.y - m_nHighlightWidth;
      SDL_FillRect(m_pScreen, &r1, SDL_MapRGB(m_pScreen->format, 
					      m_nHR, m_nHG, m_nHB));
    } else {
      r1.x = m_rect.x = m_CurX.Get();
      r1.y = m_rect.y = m_CurY.Get();
    }
    
    SDL_BlitSurface(m_pSurface, NULL, pSurf, &m_rect);
  } else {}
  return 0;
}

int StimulusBmp::Action() {
  g_pErr->DFI("StimulusBmp::Action", m_sResource.c_str(), 3);

  // update position (if necessary)
  m_rect.x = m_CurX.Get(); //*m_px;
  m_rect.y = m_CurY.Get(); //*m_py;
  g_pErr->Debug(pastestr::paste("dd", " ", (long) m_CurX.Get(), (long) m_CurY.Get()));

  if (!m_pSurface) {
    g_pErr->Report("error in StimulusBmp::Action()");
    return 1;
  } else {}

  Draw();
  StimulusBmp::Flip1();

  g_pErr->DFO("StimulusBmp::Action", m_sResource.c_str(), 3);
  return 0;
}

void StimulusBmp::Flip1(bool bMem) {
  if (!m_pScreen) {
    g_pErr->Report("SDL screen not initialized.");
    return;
  } else {}

  if (bMem) {
    if (StimulusBmp::m_pMemSurface) {
      SDL_Flip(StimulusBmp::m_pMemSurface);
    } else {
      g_pErr->Debug("memory surface not initialized");
    }
  } else {
    SDL_Flip(StimulusBmp::m_pScreen);
  }
}

void StimulusBmp::SetScreen(SDL_Surface * pSurface) {
  StimulusBmp::m_pScreen = pSurface;
}

int StimulusBmp::SetColorkey(int r, int g, int b) {
  m_r = r; m_g = g; m_b = b; m_bColorKey = 1;
}

int StimulusBmp::InAOI(Uint16 x, Uint16 y) {
  int nResult = 0;

  if ( (x >= m_CurX.Get()) && (x <= (m_CurX.Get() + m_rect.w) ) &&
       (y >= m_CurY.Get()) && (y <= (m_CurY.Get() + m_rect.h) ) ) {
    nResult = 1;
  } else {}

  return nResult;
}

int StimulusBmp::SetLayer(int nLayer) {
  g_pErr->DFI("StimulusBmp::SetLayer", m_id, 5);
  m_nLayer = nLayer;
  g_pErr->DFO("StimulusBmp::SetLayer", m_id, 5);

  return 0;
}

void StimulusBmp::ResetLoc() {
  g_pErr->DFI("StimulusBmp::ResetLoc", m_id, 5);
  m_CurX.Set(m_x1);
  m_CurY.Set(m_y1);
  //m_px = &m_CurX;
  //m_py = &m_CurY;
  g_pErr->DFO("StimulusBmp::ResetLoc", m_id, 5);
}

void StimulusBmp::Highlight(const char * pHinfo) {
  istringstream istr(pHinfo);
  g_pErr->DFI("StimulusBmp::Highlight", m_id, 5);
  m_bHighlight = 1;
  istr >> m_nHighlightWidth;
  istr >> m_nHR; istr >> m_nHG; istr >> m_nHB;
  //g_pErr->Debug(m_sFileName.c_str());
  g_pErr->DFO("StimulusBmp::Highlight", m_id, 5);
}

/*
Oplist * StimulusBmp::GetAttr(string s) {
  if (s == "x1") {
    return &m_CurX;
  } else if (s == "y1") {
    return &m_CurY;
  }
  return NULL;
}
*/

void StimulusBmp::InitMemSurface() {

  g_pErr->DFI("StimulusBmp::InitMemSurface", 4, 0);
  if (!StimulusBmp::m_pMemSurface) {
    g_pErr->Debug("initializing memory surface");
    if (m_pScreen) {
      StimulusBmp::m_pMemSurface = SDL_ConvertSurface(m_pScreen, m_pScreen->format, m_pScreen->flags);
      if (!StimulusBmp::m_pMemSurface) {
	g_pErr->Report("StimulusBmp::InitMemSurface, couldn't initialize");
      } else {}
    } else {
      g_pErr->Report("In StimulusBmp::InitMemSurface; couldn't initialize memory surface");
    }
  } else {}
  g_pErr->DFO("StimulusBmp::InitMemSurface", 4, 0);

}

void StimulusBmp::FlipMemoryToScreen() {
  SDL_BlitSurface(m_pMemSurface, NULL, m_pScreen, NULL);
  SDL_Flip(m_pScreen);
}

int StimulusBmp::LoadBMP(string s1) {

  SDL_Surface * psTemp;

  if (m_pSurface) {
    g_pErr->Debug("error surface already defined: erasing");
    SDL_FreeSurface(m_pSurface);
    //return 0;
  } else {}

  psTemp = SDL_LoadBMP(s1.c_str());
  if (psTemp == NULL) {
    g_pErr->Report(pastestr::paste("ss", "",
				   "Unable to load bitmap ",
				   s1.c_str()));
  } else {}

  m_pSurface = SDL_DisplayFormat(psTemp);
  SDL_FreeSurface(psTemp);

  m_rect.x = m_CurX.Get();
  m_rect.y = m_CurY.Get();
  m_rect.w = m_pSurface->w;
  m_rect.h = m_pSurface->h;

  if (m_x2 == -1) {
    m_x2 = m_rect.x + m_pSurface->w;
    m_rect.w = m_pSurface->w;
  } else {}

  if (m_y2 == -1) {
    m_y2 = m_rect.y + m_pSurface->h;
    m_rect.h = m_pSurface->h;
  } else {}

  if (m_bColorKey) {
		g_pErr->Debug(pastestr::paste("sddd", " ", "colorkey", (long) m_r, (long) m_g, (long) m_b));
    SDL_SetColorKey( m_pSurface, SDL_SRCCOLORKEY | SDL_RLEACCEL,
		     SDL_MapRGB(StimulusBmp::m_pScreen->format, m_r, m_g, m_b) );
  } else {}

  return 0;
}

int StimulusBmp::DrawRect(int x1, int y1, int x2, int y2, int r, int g, int b) {
  SDL_Rect r1;
  r1.x = x1; r1.y = y1;
  r1.w = (x2-x1);
  r1.h = (y2-y1);

  if (!m_pSurface) {
    g_pErr->Report("surface not initialized!");
  } else {
    SDL_FillRect(m_pSurface, &r1, SDL_MapRGB(StimulusBmp::m_pScreen->format, r, g, b));
  }

  return 0;
}

void StimulusBmp::ClearScreen(bool bMem /* = false */) {
  SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };

  if (bMem) {
    SDL_FillRect(m_pMemSurface, NULL,
		 SDL_MapRGB(m_pMemSurface->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(m_pMemSurface);
  } else {
    SDL_FillRect(m_pScreen, NULL,
		 SDL_MapRGB(m_pScreen->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(m_pScreen);
  }
}

int StimulusBmp::Erase(bool bMem) {
  SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };
  
  if (bMem) {
    SDL_FillRect(m_pMemSurface, &m_rect,
		 SDL_MapRGB(m_pMemSurface->format, backcol.r, backcol.g, backcol.b));
    //SDL_Flip(m_pMemSurface);
  } else {
    SDL_FillRect(m_pScreen, &m_rect,
		 SDL_MapRGB(m_pScreen->format, backcol.r, backcol.g, backcol.b));
    //SDL_Flip(m_pScreen);
  }

  return 0;
}
