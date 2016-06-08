#include "Display_SDL.hpp"
#include "StimulusImg.hpp"
#include <stdio.h>

SDL_Surface * Display_SDL::s_pScreen = NULL;
SDL_mutex * Display_SDL::s_pScreenMutex = NULL;

SDL_Surface * Display_SDL::GetScreen() {
	return Display_SDL::s_pScreen;
}

Display_SDL::Display_SDL(long id) : Display(id) {
  m_bSelfAlloc = false;
}

Display_SDL::Display_SDL(SDL_Surface * pSurface) {
  if (pSurface) {
    m_bSelfAlloc = false;
    Display_SDL::s_pScreen = pSurface;
    // StimulusImg::SetScreen(this);
  } else {}
	Display_SDL::s_pScreenMutex = SDL_CreateMutex();
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
}

int Display_SDL::SetColorKey(int r, int g, int b) {
  m_SDL_mapRGB = SDL_MapRGB(Display_SDL::GetScreen()->format, r, g, b);
}

int Display_SDL::Draw() {
  Display::Draw();
}

int Display_SDL::CreateScreen(int x0, int y0, int w, int h, Uint32 nFlags) {
  char pc[256] = "SDL_VIDEO_CENTERED=1";
  SDL_putenv(pc);
  Display_SDL::s_pScreen = SDL_SetVideoMode(w, h, 0, nFlags);
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
