#include "Display_SDL.hpp"
#include "StimulusBmp.hpp"
#include <stdio.h>

SDL_Surface * Display_SDL::m_pScreen = NULL;

Display_SDL::Display_SDL(long id) : Display(id) {
  m_bSelfAlloc = false;
}

Display_SDL::Display_SDL(SDL_Surface * pSurface) {
  if (pSurface) {
    m_bSelfAlloc = false;
    m_pScreen = pSurface;
    StimulusBmp::SetScreen(m_pScreen);
  } else {}
}

Display_SDL::~Display_SDL() {
  if (m_bSelfAlloc) {
    if (m_pScreen) {
      SDL_FreeSurface(m_pScreen);
    } else {
      m_pScreen = NULL;
    }
    // TODO: delete it
  } else {
  }
}

int Display_SDL::SetColorKey(int r, int g, int b) {
  m_SDL_mapRGB = SDL_MapRGB(m_pScreen->format, r, g, b);
}

int Display_SDL::Draw() {
  Display::Draw();
  //SDL_Flip(m_pScreen);
}

int Display_SDL::CreateScreen(int x0, int y0, int w, int h, Uint32 nFlags) {
  char pc[256] = "SDL_VIDEO_CENTERED=1";
  //sprintf(pc, "SDL_VIDEO_CENTERED", x0, y0);
  //g_pErr->Report("NO!");
  SDL_putenv(pc);
  m_pScreen = SDL_SetVideoMode(w, h, 0, nFlags);
  StimulusBmp::SetScreen(m_pScreen);
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

    //SDL_FillRect(m_pScreen, NULL,
    //SDL_MapRGB(m_pScreen->format, backcol.r, backcol.g, backcol.b));
    SDL_BlitSurface(text, NULL, m_pScreen, &dstrect);
    SDL_FreeSurface(text);
    SDL_Flip(m_pScreen);
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

    SDL_FillRect(m_pScreen, NULL,
		 SDL_MapRGB(m_pScreen->format, backcol.r, backcol.g, backcol.b));
    SDL_BlitSurface(text, NULL, m_pScreen, &dstrect);
    SDL_FreeSurface(text);
    SDL_Flip(m_pScreen);
  }

  TTF_CloseFont(font);

  return 0;
}

int Display_SDL::ClearScreen() {

  if (m_pScreen) {

    SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };

    SDL_FillRect(m_pScreen, NULL,
		 SDL_MapRGB(m_pScreen->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(m_pScreen);

  } else {}

  return 0;
}

int Display_SDL::ClearRegion(int x1, int y1, int x2, int y2) {

  SDL_Rect rect;
  rect.x = x1;
  rect.y = y1;
  rect.w = (x2-x1);
  rect.h = (y2-y1);

  if (m_pScreen) {

    SDL_Color backcol = { 0x00, 0x00, 0x00, 0 };

    SDL_FillRect(m_pScreen, &rect,
		 SDL_MapRGB(m_pScreen->format, backcol.r, backcol.g, backcol.b));
    SDL_Flip(m_pScreen);

  } else {}

  return 0;
}
