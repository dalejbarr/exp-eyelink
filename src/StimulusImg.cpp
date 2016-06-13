#include "StimulusImg.hpp"
#include "global.hpp"
#include "pastestr.hpp"
#include <algorithm>
#include <SDL_image.h>

#define FTYPE_BMP 1
#define FTYPE_JPG 2

// Display_SDL * StimulusImg::s_pDisplay = NULL;

StimulusImg::StimulusImg(long id, Template * pTemplate,
			 long idCmd, ArgMMap mmArgs) :
  Stimulus(id, pTemplate) {

  if (!Display_SDL::GetMemScreen() && Display_SDL::GetScreen()) {
    // only initialize the memory surface once
		Display_SDL::InitMemSurface();
  } else {}

  // parse out all common arguments
  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("Resource");
  ArgIter ii;

  ii = pii.first;
  m_sResource = (*ii).second;

  g_pErr->DFI("StimulusImg::StimulusImg(mmArgs)", m_sResource.c_str(), 4);

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

  g_pErr->DFO("StimulusImg::StimulusImg(mmArgs)", m_sResource.c_str(), 4);  
}

StimulusImg::StimulusImg(long id, Template * pTemplate, const char * pName, 
			 int x1, int y1, int x2, int y2, int nLayer) :
  Stimulus(id, pTemplate) {
  g_pErr->DFI("StimulusImg::StimulusImg", pName, 3);
  m_sResource.assign(pName);
  if ((m_sResource == "") && (x2 == -1) && (y2 == -1)) {
    g_pErr->Report("can't have blank AOI Name and missing x2,y2 coords!!");
  } else {}
  m_x1 = x1; m_y1 = y1; m_x2 = x2; m_y2 = y2;
  m_CurX.Set(x1);
  m_CurY.Set(y1);
  m_nLayer = nLayer;
  Initialize();

  g_pErr->DFO("StimulusImg::StimulusImg", pName, 3);
}

void StimulusImg::Initialize() {
  m_rect.x = m_x1;
  m_rect.y = m_y1;
  m_rect.w = m_x2-m_x1; m_rect.h = m_y2-m_y1;
  m_CurX.Set(m_x1);
  m_CurY.Set(m_y1);

  m_bColorKey = 0;
  m_bHighlight = 0;
  m_nHighlightWidth = 2;
  m_nHR = 255; m_nHG = 255; m_nHB = 255;
  m_r = 0; m_g = 0; m_b = 0;

  m_pSurface = NULL;
}

StimulusImg::~StimulusImg() {
  g_pErr->DFI("StimulusImg::~StimulusImg", m_sResource.c_str(), 3);
  g_pErr->DFO("StimulusImg::~StimulusImg", m_sResource.c_str(), 3);
}

int StimulusImg::Prepare() {
  Stimulus::Prepare();
	Display_SDL::InitMemSurface();
  // load it in
  g_pErr->DFI("StimulusImg::Prepare", m_sResource.c_str(), 3);
  if (m_sFileName == "") {
    g_pErr->Debug("blank");
    return 1;
  } else {}

  string s1 = Stimulus::GetFilenameFromResource(m_sResource.c_str());
  Load(s1.c_str());

  g_pErr->DFO("StimulusImg::Prepare", m_sFileName.c_str(), 3);
  return 0;
}

int StimulusImg::Finish() {
  g_pErr->DFI("StimulusImg::Finish", m_sResource.c_str(), 3);
  
  if (m_pSurface) {
    SDL_FreeSurface(m_pSurface);
  } else {}
  m_pSurface = NULL;

  Stimulus::Finish();

  g_pErr->DFO("StimulusImg::Finish", m_sResource.c_str(), 3);
  return 0;
}

int StimulusImg::Draw(bool bMem /*=0*/) {
  // g_pErr->DFI("StimulusImg::Draw", m_sFileName.c_str(), 3);
	SDL_Surface * pSurf = NULL;
  SDL_Rect r1;
  //r1.x = m_rect.x; r1.y = m_rect.y;

  if (m_pSurface) {
		if (bMem) {
			// TODO: lock memory surface too
			pSurf = Display_SDL::LockMemScreen();
		} else {
			pSurf = Display_SDL::LockScreen();
		}

    if (m_bHighlight) {
      r1.w = m_rect.w + 2*m_nHighlightWidth;
      r1.h = m_rect.h + 2*m_nHighlightWidth;  
      r1.x = m_rect.x - m_nHighlightWidth;
      r1.y = m_rect.y - m_nHighlightWidth;
      SDL_FillRect(pSurf, &r1, SDL_MapRGB(pSurf->format, m_nHR, m_nHG, m_nHB));
    } else {
      r1.x = m_rect.x = m_CurX.Get();
      r1.y = m_rect.y = m_CurY.Get();
			r1.w = m_rect.w; 
			r1.h = m_rect.h;
    }

    SDL_BlitSurface(m_pSurface, NULL, pSurf, &m_rect);
		if (bMem) {
			Display_SDL::UnlockMemScreen();
		} else {
			Display_SDL::UnlockScreen();
		}    

  } else {}

  // g_pErr->DFO("StimulusImg::Draw", m_sFileName.c_str(), 3);
  return 0;
}

int StimulusImg::Action() {
  g_pErr->DFI("StimulusImg::Action", m_sResource.c_str(), 3);

  // update position (if necessary)
  m_rect.x = m_CurX.Get(); //*m_px;
  m_rect.y = m_CurY.Get(); //*m_py;
  g_pErr->Debug(pastestr::paste("dd", " ", (long) m_CurX.Get(), (long) m_CurY.Get()));

  if (!m_pSurface) {
    g_pErr->Report("error in StimulusImg::Action()");
    return 1;
  } else {}

  Draw();
  Display_SDL::Flip1();

  g_pErr->DFO("StimulusImg::Action", m_sResource.c_str(), 3);
  return 0;
}


int StimulusImg::SetColorkey(int r, int g, int b) {
  m_r = r; m_g = g; m_b = b; m_bColorKey = 1;
}

int StimulusImg::InAOI(Uint16 x, Uint16 y) {
  int nResult = 0;

  if ( (x >= m_CurX.Get()) && (x <= (m_CurX.Get() + m_rect.w) ) &&
       (y >= m_CurY.Get()) && (y <= (m_CurY.Get() + m_rect.h) ) ) {
    nResult = 1;
  } else {}

  return nResult;
}

int StimulusImg::SetLayer(int nLayer) {
  g_pErr->DFI("StimulusImg::SetLayer", m_id, 5);
  m_nLayer = nLayer;
  g_pErr->DFO("StimulusImg::SetLayer", m_id, 5);

  return 0;
}

void StimulusImg::ResetLoc() {
  g_pErr->DFI("StimulusImg::ResetLoc", m_id, 5);
  m_CurX.Set(m_x1);
  m_CurY.Set(m_y1);
  //m_px = &m_CurX;
  //m_py = &m_CurY;
  g_pErr->DFO("StimulusImg::ResetLoc", m_id, 5);
}

void StimulusImg::Highlight(const char * pHinfo) {
  istringstream istr(pHinfo);
  g_pErr->DFI("StimulusImg::Highlight", m_id, 5);
  m_bHighlight = 1;
  istr >> m_nHighlightWidth;
  istr >> m_nHR; istr >> m_nHG; istr >> m_nHB;
  //g_pErr->Debug(m_sFileName.c_str());
  g_pErr->DFO("StimulusImg::Highlight", m_id, 5);
}

int StimulusImg::Load(string s1) {
  SDL_Surface * psTemp;

  if (m_pSurface) {
    g_pErr->Debug("error surface already defined: erasing");
    SDL_FreeSurface(m_pSurface);
    //return 0;
  } else {}

	//int ftype = CheckFileType(s1.c_str());
	// TODO: do something?
	psTemp = IMG_Load(s1.c_str());
	if (psTemp == NULL) {
		g_pErr->Report(pastestr::paste("ssss", " ", "could not load file", s1.c_str(), ": ", IMG_GetError()));
	}

  m_pSurface = SDL_DisplayFormatAlpha(psTemp);
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
										 SDL_MapRGB(Display_SDL::LockScreen()->format, m_r, m_g, m_b) );  // here
 		Display_SDL::UnlockScreen();
  } else {}

  return 0;
}

int StimulusImg::DrawRect(int x1, int y1, int x2, int y2, int r, int g, int b) {
  SDL_Rect r1;
  r1.x = x1; r1.y = y1;
  r1.w = (x2-x1);
  r1.h = (y2-y1);

  if (!m_pSurface) {
    g_pErr->Report("surface not initialized!");
  } else {
    SDL_FillRect(m_pSurface, &r1, SDL_MapRGB(Display_SDL::LockScreen()->format, r, g, b));
		Display_SDL::UnlockScreen();
  }

  return 0;
}


int StimulusImg::Erase(bool bMem) {
  SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };
  
  if (bMem) {
		SDL_Surface * pScreen = Display_SDL::LockMemScreen();
    SDL_FillRect(pScreen, &m_rect,
								 SDL_MapRGB(pScreen->format, backcol.r, backcol.g, backcol.b));
		Display_SDL::UnlockMemScreen();
  } else {
		SDL_Surface * pScreen = Display_SDL::LockScreen();
    SDL_FillRect(pScreen, &m_rect,
								 SDL_MapRGB(pScreen->format, backcol.r, backcol.g, backcol.b));
		Display_SDL::UnlockScreen();
  }

  return 0;
}

SDL_Surface * StimulusImg::LoadImage(string s1) {
	return NULL;
}
