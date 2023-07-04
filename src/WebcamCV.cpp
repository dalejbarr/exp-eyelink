#include "global.hpp"
//#include <sys/types.h>
//#include <sys/stat.h>
//#include <sys/ioctl.h>
//#include <sys/mman.h>
//#include <fcntl.h>

#include "WebcamCV.hpp"
#include "pastestr.hpp"
#include "Display_SDL.hpp"

extern Display_SDL * g_pDisplay;

WebcamCV::WebcamCV(int nDev) {
  g_pErr->DFI("WebcamCV::WebcamCV", 0L, 3);

  m_bInitialized = false;
  m_bIsVisible = false;
  m_nDev = nDev;

  m_frame = NULL;
  m_buffer_stream = NULL;
  m_bIsStreaming = false;
  
  m_pCap.reset();
  
  // m_nWidth = m_nHeight = m_x0 = m_y0 = -1;

  //  cv::namedWindow("Video Footage", cv::WINDOW_AUTOSIZE );
  m_pCap = vcPtr(new cv::VideoCapture(m_nDev));

  if (!m_pCap->isOpened()) {
    g_pErr->Report(pastestr::paste("sd", "",
				   "couldn't open webcam device ",
				   (long) m_nDev));
  }
  
  g_pErr->DFO("WebcamCV::WebcamCV", 0L, 3);
}

WebcamCV::~WebcamCV() {
  g_pErr->DFI("WebcamCV::~WebcamCV", 0L, 3);

  Cleanup();

  g_pErr->DFO("WebcamCV::~WebcamCV", 0L, 3);
}

int WebcamCV::Initialize() {
  if (!m_bInitialized) {
    m_bInitialized = true; // only run once
    m_bIsVisible = false;
  }
  
  return 0;
}


int WebcamCV::ActivateStreaming() {
  int ret = 0;

  g_pErr->DFI("WebcamCV::ActivateStreaming()", 0L, 3);
  m_bIsStreaming = true;
  g_pErr->DFO("WebcamCV::ActivateStreaming()", 0L, 3);
  
  return ret;
}

int WebcamCV::DeactivateStreaming() {
  int ret = 0;

  g_pErr->DFI("WebcamCV::DeactivateStreaming()", 0L, 3);
  m_bIsStreaming = false;
  g_pErr->DFO("WebcamCV::DeactivateStreaming()", 0L, 3);
  
  return ret;
}

SDL_Surface * WebcamCV::GrabFrame() {
  cv::Mat frame_orig;
  // cv::Mat m_frame_small;

  (*m_pCap) >> frame_orig;

  /*
  g_pErr->Debug(pastestr::paste("dsds", "",
				frame_orig.cols, "x",
				frame_orig.rows, " image array"));

  g_pErr->Debug(pastestr::paste("dsdsd", " ",
				frame_orig.total(), "elements of size",
				frame_orig.elemSize(), "=",
				frame_orig.total() *
				frame_orig.elemSize()));
  */

  cv::resize(frame_orig, m_frame_small, cv::Size(), .416, .416);
  std::size_t bufsize = m_frame_small.total() * frame_orig.elemSize();
  
  m_buffer_stream = SDL_RWFromMem((void *) m_frame_small.data, bufsize);

  if (!m_buffer_stream) {
    g_pErr->Report("couldn't allocate m_buffer_stream");
  }

  m_frame = SDL_CreateRGBSurfaceFrom((void *) m_frame_small.data,
				     m_frame_small.cols,
				     m_frame_small.rows,
				     m_frame_small.depth() * m_frame_small.channels(),
				     m_frame_small.cols * m_frame_small.elemSize(),
				     0xff0000, 0x00ff00, 0x0000ff, 0);
  
  //m_frame = IMG_LoadTyped_RW(m_buffer_stream, 0, "JPG");

  if (!m_frame) {
    g_pErr->Report(pastestr::paste("ss", " ",
				  "couldn't allocate frame:",
				  SDL_GetError()));

  }
  
  /*
  g_pErr->Debug(pastestr::paste("dsds", "",
				m_frame_small.cols, "x",
				m_frame_small.rows, " image array"));
  
    m_buffer_stream = SDL_RWFromMem(m_vd.tmpbuffer,
    m_vd.buf.bytesused + DHT_SIZE); */

  
  // cv::imshow( "Footage", frame );

  return m_frame;
}

int WebcamCV::GetWidth() {
  return 0;
}

int WebcamCV::GetHeight() {
  return 0;
}

bool WebcamCV::IsVisible() {
  return m_bIsVisible;
}

int WebcamCV::GetID() {
  return m_nDev;
}

int WebcamCV::SetDimensions(int w, int h) {
  m_nWidth = w;
  m_nHeight = h;

  return 0;
}

int WebcamCV::SetOrigin(int x0, int y0) {
  m_x0 = x0;
  m_y0 = y0;

  return 0;
}

int WebcamCV::Cleanup() {
  m_pCap.reset();

  if (m_frame) {
    SDL_FreeSurface(m_frame);
    m_frame = NULL;
  }
  
  if (m_buffer_stream) {
    SDL_RWclose(m_buffer_stream);		
    m_buffer_stream = NULL;
  }

  return 0;
}
