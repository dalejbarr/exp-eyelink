#include "StimulusWebcamCV.hpp"
#include "pastestr.hpp"
#include <boost/lexical_cast.hpp>
//#include "Display_SDL.hpp"
#include "Template.hpp"

bool StimulusWebcamCV::s_bContinue = false;
Uint32 StimulusWebcamCV::s_msFrameInterval = 25;

extern Display_SDL * g_pDisplay;

StimulusWebcamCV::StimulusWebcamCV(long id, ArgMMap mmArgs,
				   Template * pTemplate, WebcamCVPtr pCam) :
  Stimulus(id, pTemplate) {

  g_pErr->DFI("StimulusWebcamCV::StimulusWebcamCV()", 0L, 1);
  
  m_pThread = NULL;
  m_pCam = pCam;
  m_pTemplate = pTemplate;

  pair<ArgIter, ArgIter> pii;
  ArgIter ii;
  
  // m_rect.w = m_pCam->GetWidth();
  m_rect.w = 266;
  pii = mmArgs.equal_range("width");
  if (pii.first != pii.second) {
    ii = pii.first;
    m_rect.w = boost::lexical_cast<int>((*ii).second.c_str());
  } 
  //ArgIter ii;
  
  m_rect.h = 200;
  //m_rect.h = m_pCam->GetHeight();
  pii = mmArgs.equal_range("height");
  if (pii.first != pii.second) {
    ii = pii.first;
    m_rect.h = boost::lexical_cast<int>((*ii).second.c_str());
  } 
  
  // m_x = (int) (g_pDisplay->m_nWidth - m_width) / 2;
  m_rect.x = 0;
  //m_rect.x = (int) (g_pDisplay-> - m_rect.w) / 2;
  pii = mmArgs.equal_range("x");
  if (pii.first != pii.second) {
    ii = pii.first;
    m_rect.x = boost::lexical_cast<int>((*ii).second.c_str());
  }

  g_pErr->Debug("here6");
  
  //m_rect.y = (g_pDisplay->m_nHeight - m_rect.h) / 2;
  m_rect.y = 0;
  pii = mmArgs.equal_range("y");
  if (pii.first != pii.second) {
    ii = pii.first;
    m_rect.y = boost::lexical_cast<int>((*ii).second.c_str());
  }

  g_pErr->DFO("StimulusWebcamCV::StimulusWebcamCV()", 0L, 1);
}

StimulusWebcamCV::~StimulusWebcamCV() {
}

int StimulusWebcamCV::Prepare() {
  g_pErr->DFI("StimulusWebcamCV::Prepare", 0L, 4);

  m_pCam->Initialize();
  s_bContinue = true;

  m_pCam->SetDimensions(m_rect.w, m_rect.h);
  m_pCam->SetOrigin(m_rect.x, m_rect.y);

  //m_pCam->Prepare();
  
  g_pErr->DFO("StimulusWebcamCV::Prepare", 0L, 4);

  return 0;
}

int StimulusWebcamCV::Action() {
  g_pErr->DFI("StimulusWebcamCV::Action", 0L, 4);

  if (m_pThread) {
    g_pErr->Debug("StimulusWebcamCV::Prepare(): Thread already active");
  } else {
    m_pThread = SDL_CreateThread(StimulusWebcamCV::LaunchThread, this);
    g_pErr->DFO("StimulusWebcamCV::Action", 0L, 4);
  }

  return 0;
}

int StimulusWebcamCV::Finish() {
  g_pErr->DFI("StimulusWebcamCV::Finish", 0L, 4);

  s_bContinue = false;
  int nStatus = 0;
  SDL_WaitThread(m_pThread, &nStatus);
  m_pThread = NULL;
	
  g_pErr->DFO("StimulusWebcamCV::Finish", 0L, 4);

  return 0;
}

int StimulusWebcamCV::Cleanup() {	
  g_pErr->DFI("StimulusWebcamCV::Cleanup", 0L, 4);

  s_bContinue = false;
  if (m_pThread) {
    int nStatus = 0;
    SDL_WaitThread(m_pThread, &nStatus);
    m_pThread = NULL;
  }
  m_pCam->DeactivateStreaming();
  m_pCam->Cleanup();

  g_pErr->DFI("StimulusWebcamCV::Cleanup", 0L, 4);

  return 0;
}

int StimulusWebcamCV::LaunchThread(void * pVoid) {
  StimulusWebcamCV * pThis = (StimulusWebcamCV *) pVoid;
	
  return pThis->Main();
}

int StimulusWebcamCV::Main() {
  Uint32 msStart, msDone, msDelay;
  SDL_Surface * pSurf = NULL;
  
  // Activate streaming
  m_pCam->ActivateStreaming();

  g_pErr->Debug(pastestr::paste("sd", "",
				"streaming started on webcam ",
				(long) m_pCam->GetID()));
  
  //while (s_bContinue) {
    msStart = ClockFn();
    // TODO: do work
    pSurf = m_pCam->GrabFrame();
    ShowFrame(pSurf);
    m_pCam->GrabFrame();
    msDone = ClockFn();
    if ((msDone - msStart) <= StimulusWebcamCV::s_msFrameInterval) {
      msDelay = s_msFrameInterval - (msDone - msStart);
    } else {
      msDelay = 0;
    }
    /* g_pErr->Debug(pastestr::paste("sdsd", " ", 
       "elapsed:", (long) (msDone - msStart),
       "delaying:", (long) msDelay)); */
    SDL_Delay(msDelay);
    SDL_SaveBMP(pSurf, "testing.bmp");
    //}

  g_pErr->Debug(pastestr::paste("sd", "",
				"streaming ended on webcam ",
				(long) m_pCam->GetID()));
  
  m_pCam->DeactivateStreaming();

  return 0;
}

int StimulusWebcamCV::ShowFrame(SDL_Surface * pSurface) {
  if (pSurface) {
    SDL_BlitSurface(pSurface, NULL, Display_SDL::LockScreen(), &m_rect);
    Display_SDL::UnlockScreen();
    m_pTemplate->Redraw(0);
  } 

  return 0;
}
