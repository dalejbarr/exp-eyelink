#include "portaudio.h"
//#include "sndfile.h"
#include <string>
#include <vector>
#include "InputDev.hpp"

#define SAMPLE_SILENCE  (0.0f)
#define FRAMES_PER_BUFFER 512
#define PA_SAMPLE_TYPE  paFloat32

using std::string;

#ifdef _LINUX_PLATFORM
#include "pa_linux_alsa.h"
#endif

#define NUM_SECONDS 30
#define SAMPLE_RATE (22050)

typedef float SAMPLE;
//typedef std::vector<PASoundBuf *> soundBuffers;

static int recordCallback( const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData );

class PASoundIn : public InputDev {
protected:
  static PaStreamParameters m_inputParameters;
  static PaStream*          m_stream;
  //string             m_strResource;
public:
  static SAMPLE*            m_pRecordedSamples;
  static unsigned long      m_ulFrameIx;
  static unsigned long      m_ulMaxFrameIx;
  static bool g_bInitialized;
  PASoundIn(unsigned long idDev);
  virtual ~PASoundIn();
  virtual void Prepare();
  virtual void Start();
  virtual void Stop();
  virtual int Save();
  //inline void SetResource(const char * pRes) { m_strResource.assign(pRes); }
  //int AttachSoundBuffer(PASoundBuf * pasb);
};
