#include "StimulusWebcam.hpp"
#include "pastestr.hpp"
#include <boost/lexical_cast.hpp>
#include "Display_SDL.hpp"
#include "Template.hpp"

bool StimulusWebcam::s_bContinue = false;
Uint32 StimulusWebcam::s_msFrameInterval = 25;

extern Display_SDL * g_pDisplay;

StimulusWebcam::StimulusWebcam(long id, ArgMMap mmArgs, Template * pTemplate, Webcam2 * pCam) : 
	Stimulus(id, pTemplate) {
	m_pThread = NULL;
	m_pCam = pCam;
	m_pTemplate = pTemplate;

	pair<ArgIter, ArgIter> pii;
  ArgIter ii;

	// m_rect.w = m_pCam->GetWidth();
	m_rect.w = 1280;
	pii = mmArgs.equal_range("width");
	if (pii.first != pii.second) {
		ii = pii.first;
		m_rect.w = boost::lexical_cast<int>((*ii).second.c_str());
	} 
  //ArgIter ii;

	m_rect.h = 800;
	//m_rect.h = m_pCam->GetHeight();
	pii = mmArgs.equal_range("height");
	if (pii.first != pii.second) {
		ii = pii.first;
		m_rect.h = boost::lexical_cast<int>((*ii).second.c_str());
	} 

	// m_x = (int) (g_pDisplay->m_nWidth - m_width) / 2;
	m_rect.x = -1;
	pii = mmArgs.equal_range("x");
	if (pii.first != pii.second) {
		ii = pii.first;
		m_rect.x = boost::lexical_cast<int>((*ii).second.c_str());
	}

	m_rect.y = -1;
	pii = mmArgs.equal_range("y");
	if (pii.first != pii.second) {
		ii = pii.first;
		m_rect.y = boost::lexical_cast<int>((*ii).second.c_str());
	} 
}

StimulusWebcam::~StimulusWebcam() {
}

int StimulusWebcam::Prepare() {
  g_pErr->DFI("StimulusWebcam::Prepare", 0L, 4);

	m_pCam->Initialize();
	s_bContinue = true;

	if (!g_pDisplay) {
		g_pErr->Report("display does not exist!");
	}

	if (m_rect.x == -1) {
		m_rect.x = (int) ((g_pDisplay->m_nWidth - m_pCam->GetWidth()) / 2);
		m_rect.w = m_pCam->GetWidth();
		g_pErr->Debug(pastestr::paste("sddd", " ", "x:", (long) m_pCam->GetWidth(),
																	(long) g_pDisplay->m_nWidth,
																	(long) m_rect.w));
	}

	if (m_rect.y == -1) {
		m_rect.y = (int) ((g_pDisplay->m_nHeight - m_pCam->GetHeight()) / 2);
		m_rect.h = (int) m_pCam->GetHeight();
		g_pErr->Debug(pastestr::paste("sddd", " ", "y:", (long) m_rect.y,
																	(long) g_pDisplay->m_nHeight,
																	(long) m_rect.h));
	}

  g_pErr->DFO("StimulusWebcam::Prepare", 0L, 4);

	return 0;
}

int StimulusWebcam::Action() {
  g_pErr->DFI("StimulusWebcam::Action", 0L, 4);

	if (m_pThread) {
		g_pErr->Debug("StimulusWebcam::Prepare(): Thread already active");
	} else {
		m_pThread = SDL_CreateThread(StimulusWebcam::LaunchThread, this);
		// TODO delay until webcam visible
		//while(!m_pCam->IsVisible()) SDL_Delay(20);
		g_pErr->DFO("StimulusWebcam::Action", 0L, 4);
	}

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
  g_pErr->DFI("StimulusWebcam::Cleanup", 0L, 4);

	s_bContinue = false;
	if (m_pThread) {
		int nStatus = 0;
		SDL_WaitThread(m_pThread, &nStatus);
		m_pThread = NULL;
	}
	m_pCam->DeactivateStreaming();

  g_pErr->DFI("StimulusWebcam::Cleanup", 0L, 4);

	return 0;
}

int StimulusWebcam::LaunchThread(void * pVoid) {
	StimulusWebcam * pThis = (StimulusWebcam *) pVoid;
	
	return pThis->Main();
}

int StimulusWebcam::Main() {
	Uint32 msStart, msDone, msDelay;

	// Activate streaming
	m_pCam->ActivateStreaming();

	while (s_bContinue) {
		msStart = ClockFn();
		// TODO: do work
		ShowFrame(m_pCam->GrabFrame());
		msDone = ClockFn();
		if ((msDone - msStart) <= StimulusWebcam::s_msFrameInterval) {
			msDelay = s_msFrameInterval - (msDone - msStart);
		} else {
			msDelay = 0;
		}
		// g_pErr->Debug(pastestr::paste("sdsd", " ", 
		// "elapsed:", (long) (msDone - msStart),
		// "delaying:", (long) msDelay));
		SDL_Delay(msDelay);
	}

	// m_pCam->DeactivateStreaming();

	return 0;
}

int StimulusWebcam::ShowFrame(SDL_Surface * pSurface) {
	SDL_Surface * pSurf = NULL;

  if (pSurface) {
    SDL_BlitSurface(pSurface, NULL, Display_SDL::LockScreen(), &m_rect);
		Display_SDL::UnlockScreen();
		m_pTemplate->Redraw(0);
  } 

  return 0;
}
