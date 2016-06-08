#ifndef EXP_STIMULUSWEBCAM_INCLUDED
#define EXP_STIMULUSWEBCAM_INCLUDED

#include "Stimulus.hpp"
#include "Webcam.hpp"

class StimulusWebcam : public Stimulus {
protected:
	SDL_Thread * m_pThread;
	static bool s_bContinue;
	Webcam * m_pCam;
public:
	StimulusWebcam(long id, Template * pTemplate, Webcam * pCam);
	virtual ~StimulusWebcam();
	virtual int Prepare();
	virtual int Action();
	virtual int Finish();
	virtual int Cleanup();
  int static LaunchThread(void * pVoid);
	int Main();
};

#endif
