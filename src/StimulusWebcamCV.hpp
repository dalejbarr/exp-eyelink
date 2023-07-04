#ifndef EXP_STIMULUSWEBCAMCV_INCLUDED
#define EXP_STIMULUSWEBCAMCV_INCLUDED

#include "Stimulus.hpp"
#include "WebcamCV.hpp"

class StimulusWebcamCV : public Stimulus {
protected:
  SDL_Thread * m_pThread;
  SDL_Rect m_rect;
  static bool s_bContinue;
  static Uint32 s_msFrameInterval; // number of milliseconds between frames
  WebcamCVPtr m_pCam;
  Template * m_pTemplate;
  int m_nWidth;
  int m_nHeight;
  int m_x0;
  int m_y0;
public:
  StimulusWebcamCV(long id, ArgMMap mmArgs, Template * pTemplate,
		   WebcamCVPtr pCam);
  virtual ~StimulusWebcamCV();
  virtual int Prepare();
  virtual int Action();
  virtual int Finish();
  virtual int Cleanup();
  int static LaunchThread(void * pVoid);
  int SetDimensions(int w, int h);
  int SetOrigin(int x, int y);
  int ShowFrame(SDL_Surface *);
  int Main();
};

#endif
