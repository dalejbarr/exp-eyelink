#include "Display_SDL.hpp"
#include "StimulusImg.hpp"
#include <stdio.h>

SDL_Surface * Display_SDL::s_pScreen = NULL;
SDL_mutex * Display_SDL::s_pScreenMutex = NULL;
SDL_Surface * Display_SDL::s_pMemScreen = NULL;
SDL_mutex * Display_SDL::s_pMemMutex = NULL;

SDL_Surface * Display_SDL::GetScreen() {
	return Display_SDL::s_pScreen;
}

SDL_Surface * Display_SDL::GetMemScreen() {
	return Display_SDL::s_pMemScreen;
}

SDL_Surface * Display_SDL::LockScreen() {
	SDL_Surface * pSurface = NULL;
	if (SDL_LockMutex(s_pScreenMutex) == 0) {
		pSurface = Display_SDL::GetScreen();
	} else {
		g_pErr->Report("failed to lock screen mutex!");
	}

	return pSurface;
}

int Display_SDL::UnlockScreen() {
	int n = 0;

	n = SDL_UnlockMutex(s_pScreenMutex);
	if (n != 0) {
		g_pErr->Report("failed to unlock screen mutex!");
	}

	return n;
}

SDL_Surface * Display_SDL::LockMemScreen() {
	SDL_Surface * pSurface = NULL;
	if (SDL_LockMutex(s_pMemMutex) == 0) {
		pSurface = Display_SDL::GetMemScreen();
	} else {
		g_pErr->Report("failed to lock mem screen mutex!");
	}

	return pSurface;
}

int Display_SDL::UnlockMemScreen() {
	int n = 0;

	n = SDL_UnlockMutex(s_pMemMutex);
	if (n != 0) {
		g_pErr->Report("failed to unlock screen mutex!");
	}

	return n;
}

Display_SDL::Display_SDL(long id) : MyDisplay(id) {
  m_bSelfAlloc = false;
}

Display_SDL::Display_SDL(SDL_Surface * pSurface) {
  if (pSurface) {
    m_bSelfAlloc = false;
    Display_SDL::s_pScreen = pSurface;
    // StimulusImg::SetScreen(this);
  } else {}
	Display_SDL::s_pScreenMutex = SDL_CreateMutex();
	Display_SDL::s_pMemMutex = SDL_CreateMutex();
}

Display_SDL::~Display_SDL() {
  if (m_bSelfAlloc) {
    if (Display_SDL::GetScreen()) {
      SDL_FreeSurface(Display_SDL::s_pScreen);
    } else {}
		Display_SDL::s_pScreen = NULL;
  }
	if (Display_SDL::s_pScreenMutex) {
		SDL_DestroyMutex(Display_SDL::s_pScreenMutex);
		Display_SDL::s_pScreenMutex = NULL;
	}

  if (Display_SDL::s_pMemScreen) {
    SDL_FreeSurface(Display_SDL::s_pMemScreen);
    Display_SDL::s_pMemScreen = NULL;
  } else {}
	if (Display_SDL::s_pMemMutex) {
		SDL_DestroyMutex(Display_SDL::s_pMemMutex);
		Display_SDL::s_pMemMutex = NULL;
	}
}

int Display_SDL::SetColorKey(int r, int g, int b) {
  m_SDL_mapRGB = SDL_MapRGB(Display_SDL::GetScreen()->format, r, g, b);
}

int Display_SDL::Draw() {
  MyDisplay::Draw();
}

int Display_SDL::CreateScreen(int x0, int y0, int w, int h, Uint32 nFlags) {
  char pc[256] = "SDL_VIDEO_CENTERED=1";
  SDL_putenv(pc);
  Display_SDL::s_pScreen = SDL_SetVideoMode(w, h, 0, nFlags);
	m_nWidth = w;
	m_nHeight = h;
  // StimulusImg::SetScreen(this);
  m_bSelfAlloc = true;
}

int Display_SDL::MessageXY(int x, int y, const char * pcMessage) {
  int ptsize = 24;
  TTF_Font * font = NULL;
  SDL_Color forecol = { 0xFF, 0xFF, 0xFF, 0 };
  SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };
  SDL_Surface * text = NULL;
  SDL_Rect dstrect;

  font = TTF_OpenFont("seguibk.ttf", ptsize);
  if (font == NULL) {
    g_pErr->Report("couldn't open font file seguibk.ttf");
  } else {}

  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
  text = TTF_RenderText_Solid(font, pcMessage, forecol);

  if (text == NULL) {
    g_pErr->Report("error rendering font");
  } else {
    dstrect.x = (int) (x - (text->w / 2));
    dstrect.y = (int) (y - (text->h / 2));
    dstrect.w = text->w;
    dstrect.h = text->h;

    SDL_BlitSurface(text, NULL, Display_SDL::GetScreen(), &dstrect);
    SDL_FreeSurface(text);
    SDL_Flip(Display_SDL::GetScreen());
  }

  TTF_CloseFont(font);

  return 0;
}

int Display_SDL::Message(const char * pcMessage) {

  int ptsize = 24;
  TTF_Font * font = NULL;
  SDL_Color forecol = { 0xFF, 0xFF, 0xFF, 0 };
  SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };
  SDL_Surface * text = NULL;
  SDL_Rect dstrect;

  font = TTF_OpenFont("seguibk.ttf", ptsize);
  if (font == NULL) {
    g_pErr->Report("couldn't open font file seguibk.ttf");
  } else {}

  TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
  text = TTF_RenderText_Solid(font, pcMessage, forecol);

  if (text == NULL) {
    g_pErr->Report("error rendering font");
  } else {
		// TODO : figure out center of screen based on screen width/height
    dstrect.x = (int) ((1024 - text->w) / 2);
    dstrect.y = (int) ((768 - text->h) / 2);
    dstrect.w = text->w;
    dstrect.h = text->h;

    SDL_FillRect(Display_SDL::GetScreen(), NULL,
								 SDL_MapRGB(Display_SDL::GetScreen()->format, 
														backcol.r, backcol.g, backcol.b));
    SDL_BlitSurface(text, NULL, Display_SDL::GetScreen(), &dstrect);
    SDL_FreeSurface(text);
    SDL_Flip(Display_SDL::GetScreen());
  }

  TTF_CloseFont(font);

  return 0;
}

void Display_SDL::ClearScreen(bool bMem /* = false */) {
  SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };

  if (bMem) {
		SDL_Surface * pSurface = Display_SDL::LockMemScreen();
    SDL_FillRect(pSurface, NULL,
								 SDL_MapRGB(pSurface->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(pSurface);
		Display_SDL::UnlockMemScreen();
  } else {
		SDL_Surface * pScreen = Display_SDL::LockScreen();
    SDL_FillRect(pScreen, NULL,
								 SDL_MapRGB(pScreen->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(pScreen);
		Display_SDL::UnlockScreen();
  }
}

int Display_SDL::ClearRegion(int x1, int y1, int x2, int y2) {

  SDL_Rect rect;
  rect.x = x1;
  rect.y = y1;
  rect.w = (x2-x1);
  rect.h = (y2-y1);

  if (Display_SDL::GetScreen()) {

    SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };

    SDL_FillRect(Display_SDL::GetScreen(), &rect,
		 SDL_MapRGB(Display_SDL::GetScreen()->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(Display_SDL::GetScreen());

  } else {}

  return 0;
}


void Display_SDL::InitMemSurface() {
	SDL_Surface * pScreen = NULL;

  g_pErr->DFI("Display_SDL::InitMemSurface", 4, 0);
  if (!Display_SDL::s_pMemScreen) {
    g_pErr->Debug("initializing memory surface");
    if (pScreen = Display_SDL::LockScreen()) {
      Display_SDL::s_pMemScreen = SDL_ConvertSurface(pScreen, 
																											pScreen->format, 
																											pScreen->flags);
			Display_SDL::UnlockScreen();
      if (!Display_SDL::s_pMemScreen) {
				g_pErr->Report("Display_SDL::InitMemSurface, couldn't initialize");
       } else {}
    } else {
      g_pErr->Report("In Display_SDL::InitMemSurface; couldn't initialize memory surface");
    }
  } else {}
  g_pErr->DFO("Display_SDL::InitMemSurface", 4, 0);
}

void Display_SDL::FlipMemoryToScreen() {
	SDL_Surface * pScreen = Display_SDL::LockScreen();
  SDL_BlitSurface(Display_SDL::LockMemScreen(), NULL, pScreen, NULL);
  SDL_Flip(pScreen);
	Display_SDL::UnlockScreen();
	Display_SDL::UnlockMemScreen();
}

void Display_SDL::Flip1(bool bMem) {
  if (bMem) {
		SDL_Flip(Display_SDL::LockMemScreen());
		Display_SDL::UnlockMemScreen();
  } else {
    SDL_Flip(Display_SDL::LockScreen());
		Display_SDL::UnlockScreen();
  }
}

/*
int Display_SDL::ClearScreen() {

  if (Display_SDL::GetScreen()) {

    SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };

    SDL_FillRect(Display_SDL::GetScreen(), NULL,
								 SDL_MapRGB(Display_SDL::GetScreen()->format, 
														backcol.r, backcol.g, backcol.b));
    SDL_Flip(Display_SDL::GetScreen());

  } else {}

  return 0;
}
*/
