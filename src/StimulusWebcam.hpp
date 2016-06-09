#ifndef EXP_STIMULUSWEBCAM_INCLUDED
#define EXP_STIMULUSWEBCAM_INCLUDED

#include "Stimulus.hpp"
#include "Webcam.hpp"

class StimulusWebcam : public Stimulus {
protected:
	SDL_Thread * m_pThread;
	SDL_Rect m_rect;
	static bool s_bContinue;
	static Uint32 s_msFrameInterval; // number of milliseconds between frames
	Webcam * m_pCam;
	Template * m_pTemplate;
public:
	StimulusWebcam(long id, ArgMMap mmArgs, Template * pTemplate, Webcam * pCam);
	virtual ~StimulusWebcam();
	virtual int Prepare();
	virtual int Action();
	virtual int Finish();
	virtual int Cleanup();
  int static LaunchThread(void * pVoid);
	int ShowFrame(SDL_Surface *);
	int Main();
};

#endif
