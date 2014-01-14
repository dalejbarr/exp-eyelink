#include "Audio_SDL.hpp"

Audio_SDL::Audio_SDL(long id) : Audio(id) {
  g_pErr->DFI("Audio_SDL::Audio_SDL", id, 1);
  if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 ) { 
    g_pErr->Report("could not initialize SDL mixer");
  }  
 g_pErr->DFO("Audio_SDL::Audio_SDL", id, 1);
}

Audio_SDL::~Audio_SDL() {
  g_pErr->DFI("Audio_SDL::~Audio_SDL", m_id, 1);
  Mix_CloseAudio();
  g_pErr->DFO("Audio_SDL::~Audio_SDL", m_id, 1);
}
