#define ALSA_PCM_NEW_HW_PARAMS_API

#include <SDL/SDL_thread.h>
#include <alsa/asoundlib.h>
#include <fstream>

#include <string>
#include <vector>
#include "InputDev.hpp"

using std::string;
using std::ios;

class AlsaSoundIn : public InputDev {
protected:
  char * m_pBuffer;
  snd_pcm_t * m_pHandle;
  snd_pcm_uframes_t m_frames;
  int m_size;
  std::fstream m_outfile;
  SDL_Thread * m_thread;
public:
  static bool s_bIsRecording;
  static bool s_bIsActive;
  static bool s_bIsInitialized;
  AlsaSoundIn(unsigned long idDev);
  virtual ~AlsaSoundIn();
  virtual void Prepare();
  virtual void Start();
  virtual void Stop();
  int Main();
};
