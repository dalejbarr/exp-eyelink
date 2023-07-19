#ifndef SBX_WEBCAMCV_INCLUDED
#define SBX_WEBCAMCV_INCLUDED

#include "opencv2/opencv.hpp"
#include <string>
using std::string;

typedef boost::shared_ptr<cv::VideoCapture> vcPtr;

class WebcamCV {
protected:
  bool m_bInitialized;
  bool m_bIsVisible;
  bool m_bIsStreaming;
  int m_nDev;
  vcPtr m_pCap;
  SDL_Surface * m_frame;
  SDL_RWops * m_buffer_stream;
  int m_nWidth;
  int m_nHeight;
  int m_x0;
  int m_y0;
public:
  cv::Mat m_frame_small;
  int GetWidth(); // { return m_vd.width; };
  int GetHeight(); // { return m_vd.height; };
  bool IsVisible(); // { return m_bIsVisible; };
  WebcamCV(int nDev);
  virtual ~WebcamCV();
  //virtual int Prepare();
  int Initialize();
  int ActivateStreaming();
  int DeactivateStreaming();
  int Cleanup();
  SDL_Surface * GrabFrame();
  int GetID();
  int SetDimensions(int w, int h);
  int SetOrigin(int x0, int y0);
};

typedef boost::shared_ptr<WebcamCV> WebcamCVPtr;

#endif
