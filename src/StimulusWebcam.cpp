#include "StimulusWebcam.hpp"
#include "pastestr.hpp"

bool StimulusWebcam::s_bContinue = false;

StimulusWebcam::StimulusWebcam(long id, Template * pTemplate, Webcam * pCam) : 
	Stimulus(id, pTemplate) {
	m_pThread = NULL;
	m_pCam = pCam;
}

StimulusWebcam::~StimulusWebcam() {
}

int StimulusWebcam::Prepare() {
	s_bContinue = true;

	return 0;
}

int StimulusWebcam::Action() {
  g_pErr->DFI("StimulusWebcam::Action", 0L, 4);

	if (m_pThread) {
		g_pErr->Report("StimulusWebcam::Prepare(): Thread already active");
	}

	m_pThread = SDL_CreateThread(StimulusWebcam::LaunchThread, this);
  g_pErr->DFO("StimulusWebcam::Action", 0L, 4);

	return 0;
}

int StimulusWebcam::Finish() {
  g_pErr->DFI("StimulusWebcam::Finish", 0L, 4);

	s_bContinue = false;
	int nStatus = 0;
	SDL_WaitThread(m_pThread, &nStatus);
	m_pThread = NULL;
	
  g_pErr->DFO("StimulusWebcam::Finish", 0L, 4);

	return 0;
}

int StimulusWebcam::Cleanup() {
	return 0;
}

int StimulusWebcam::LaunchThread(void * pVoid) {
	StimulusWebcam * pThis = (StimulusWebcam *) pVoid;
	
	return pThis->Main();
}

int StimulusWebcam::Main() {
	// Uint32 ms;
	// static Uint32 msLast = ClockFn();

	while (s_bContinue) {
		// ms = ClockFn();
		// g_pErr->Debug(pastestr::paste("sd", ":", "webcam loop", (long) (ms - msLast)));
		// msLast = ms;
		SDL_Delay(30);
	}

	return 0;
}
