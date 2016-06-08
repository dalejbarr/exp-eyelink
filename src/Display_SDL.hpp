#ifndef EXP_DISPLAY_SDL_INCLUDED
#define EXP_DISPLAY_SDL_INCLUDED

#include "Display.hpp"
#include "SDL.h"

class Display_SDL : public Display {
protected:
  static SDL_Surface * s_pScreen;
	static SDL_mutex * s_pScreenMutex;
  Uint32 m_SDL_mapRGB;
  bool m_bSelfAlloc;
public:
  Display_SDL(long id = 0);
  Display_SDL(SDL_Surface * pSurface);
  virtual ~Display_SDL();
  virtual int CreateScreen(int x0, int y0, int w, int h, Uint32 nFlags = 0);
  virtual int SetColorKey(int r, int g, int b);
  virtual int Draw();
  static SDL_Surface * GetScreen();
  static int ClearScreen();
  static int ClearRegion(int x1, int y1, int x2, int y2);
  int Message(const char * pcMessage);
  int MessageXY(int x, int y, const char * pcMessage);
};

#endif
