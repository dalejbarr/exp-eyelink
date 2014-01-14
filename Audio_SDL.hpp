#ifndef EXP_AUDIO_SDL_INCLUDED
#define EXP_AUDIO_SDL_INCLUDED

#include "Audio.hpp"
#include "SDL.h"
#include "SDL_mixer.h"

class Audio_SDL : public Audio {
protected:
  SDL_Surface * m_pScreen;
  Uint32 m_SDL_mapRGB;
public:
  Audio_SDL(long id = 0);
  virtual ~Audio_SDL();
};

#endif
