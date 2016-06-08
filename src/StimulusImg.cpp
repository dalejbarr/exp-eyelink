#include "StimulusImg.hpp"
#include "global.hpp"
#include "pastestr.hpp"
#include <algorithm>
#include <SDL_image.h>

#define FTYPE_BMP 1
#define FTYPE_JPG 2

SDL_Surface * StimulusImg::s_pMemSurface = NULL;
SDL_mutex * StimulusImg::s_pMemMutex = NULL;
// Display_SDL * StimulusImg::s_pDisplay = NULL;

StimulusImg::StimulusImg(long id, Template * pTemplate,
			 long idCmd, ArgMMap mmArgs) :
  Stimulus(id, pTemplate) {

  if (!StimulusImg::s_pMemSurface && Display_SDL::GetScreen()) {
    // only initialize the memory surface once
    InitMemSurface();
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

  if (StimulusImg::s_pMemSurface) {
    SDL_FreeSurface(StimulusImg::s_pMemSurface);
    StimulusImg::s_pMemSurface = NULL;
  } else {}
}

int StimulusImg::Prepare() {
  Stimulus::Prepare();
  InitMemSurface();
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
  //g_pErr->DFI("StimulusImg::Draw", m_sFileName.c_str(), 3);
  SDL_Rect r1;
  SDL_Surface * pSurf = Display_SDL::GetScreen();// StimulusImg::s_pScreen;
  r1.w = m_rect.w; r1.h = m_rect.h;
  r1.x = m_rect.x; r1.y = m_rect.y;

  if (bMem) {
    pSurf = StimulusImg::s_pMemSurface;
  } else {}

  if (m_pSurface) {
    if (m_bHighlight) {
      r1.w = m_rect.w + 2*m_nHighlightWidth;
      r1.h = m_rect.h + 2*m_nHighlightWidth;  
      r1.x = m_rect.x - m_nHighlightWidth;
      r1.y = m_rect.y - m_nHighlightWidth;
      SDL_FillRect(Display_SDL::GetScreen(), &r1, SDL_MapRGB(Display_SDL::GetScreen()->format, 
					      m_nHR, m_nHG, m_nHB));
    } else {
      r1.x = m_rect.x = m_CurX.Get();
      r1.y = m_rect.y = m_CurY.Get();
    }
    
    SDL_BlitSurface(m_pSurface, NULL, pSurf, &m_rect);
  } else {}
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
  StimulusImg::Flip1();

  g_pErr->DFO("StimulusImg::Action", m_sResource.c_str(), 3);
  return 0;
}

void StimulusImg::Flip1(bool bMem) {
  if (!Display_SDL::GetScreen()) {
    g_pErr->Report("SDL screen not initialized.");
    return;
  } else {}

  if (bMem) {
    if (StimulusImg::s_pMemSurface) {
      SDL_Flip(StimulusImg::s_pMemSurface);
    } else {
      g_pErr->Debug("memory surface not initialized");
    }
  } else {
    SDL_Flip(Display_SDL::GetScreen());
  }
}

void StimulusImg::SetScreen(Display_SDL * pDisplay) {
  // StimulusImg::s_pDisplay = pDisplay;
  // StimulusImg::s_pScreen = pDisplay->GetScreen();
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

/*
Oplist * StimulusImg::GetAttr(string s) {
  if (s == "x1") {
    return &m_CurX;
  } else if (s == "y1") {
    return &m_CurY;
  }
  return NULL;
}
*/

void StimulusImg::InitMemSurface() {

  g_pErr->DFI("StimulusImg::InitMemSurface", 4, 0);
  if (!StimulusImg::s_pMemSurface) {
    g_pErr->Debug("initializing memory surface");
    if (Display_SDL::GetScreen()) {
      StimulusImg::s_pMemSurface = SDL_ConvertSurface(Display_SDL::GetScreen(), 
																											Display_SDL::GetScreen()->format, 
																											Display_SDL::GetScreen()->flags);
      if (!StimulusImg::s_pMemSurface) {
				g_pErr->Report("StimulusImg::InitMemSurface, couldn't initialize");
       } else {}
    } else {
      g_pErr->Report("In StimulusImg::InitMemSurface; couldn't initialize memory surface");
    }
  } else {}
  g_pErr->DFO("StimulusImg::InitMemSurface", 4, 0);

}

void StimulusImg::FlipMemoryToScreen() {
  SDL_BlitSurface(StimulusImg::s_pMemSurface, NULL, Display_SDL::GetScreen(), NULL);
  SDL_Flip(Display_SDL::GetScreen());
}

int StimulusImg::CheckFileType(const char *pFile) {
	int ftype = 0;
	/*
	string s1;
	s1.assign(pFile);
	
	size_t last_dot = s1.find_last_of('.');
	if ((last_dot == string::npos) || (last_dot == s1.size())) {
		g_pErr->Report(pastestr::paste("sss", " ", "file", s1.c_str(), "missing file extension"));
	}

	string strExtension = s1.substr(last_dot + 1, s1.size() - last_dot);
	std::transform(strExtension.begin(), strExtension.end(), strExtension.begin(), ::tolower);
	g_pErr->Debug(pastestr::paste("ss", "", "file extension was ", strExtension.c_str()));

	if (strExtension == "bmp") {
		ftype = FTYPE_BMP;
	} else if ((strExtension == "jpg") || (strExtension == "jpeg")) {
		ftype = FTYPE_JPG;
	} else {
		g_pErr->Report(pastestr::paste("sssss", "", "error with ", s1.c_str(), 
																	 ": filetype '", strExtension.c_str(), "' not supported"));
	}
	*/

	return ftype;
}

int StimulusImg::Load(string s1) {
  SDL_Surface * psTemp;

  if (m_pSurface) {
    g_pErr->Debug("error surface already defined: erasing");
    SDL_FreeSurface(m_pSurface);
    //return 0;
  } else {}

	int ftype = CheckFileType(s1.c_str());
	// TODO: do something?
	psTemp = IMG_Load(s1.c_str());
	if (psTemp == NULL) {
		g_pErr->Report(pastestr::paste("ss", " ", "could not load file", s1.c_str()));
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
										 SDL_MapRGB(Display_SDL::GetScreen()->format, m_r, m_g, m_b) );  // here
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
    SDL_FillRect(m_pSurface, &r1, SDL_MapRGB(Display_SDL::GetScreen()->format, r, g, b));
  }

  return 0;
}

void StimulusImg::ClearScreen(bool bMem /* = false */) {
  SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };

  if (bMem) {
    SDL_FillRect(StimulusImg::s_pMemSurface, NULL,
		 SDL_MapRGB(StimulusImg::s_pMemSurface->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(StimulusImg::s_pMemSurface);
  } else {
    SDL_FillRect(Display_SDL::GetScreen(), NULL,
		 SDL_MapRGB(Display_SDL::GetScreen()->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(Display_SDL::GetScreen());
  }
}

int StimulusImg::Erase(bool bMem) {
  SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };
  
  if (bMem) {
    SDL_FillRect(StimulusImg::s_pMemSurface, &m_rect,
		 SDL_MapRGB(StimulusImg::s_pMemSurface->format, backcol.r, backcol.g, backcol.b));
  } else {
    SDL_FillRect(Display_SDL::GetScreen(), &m_rect,
		 SDL_MapRGB(Display_SDL::GetScreen()->format, backcol.r, backcol.g, backcol.b));
  }

  return 0;
}

SDL_Surface * StimulusImg::LoadImage(string s1) {
	return NULL;
}
