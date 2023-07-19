#ifndef SBX_WEBCAM2_INCLUDED
#define SBX_WEBCAM2_INCLUDED

#include <linux/videodev2.h>
#include <string>
using std::string;

/// #define NB_BUFFER 1
#define DHT_SIZE 420
#define HEADERFRAME1 0xaf

struct vdIn {
  int fd;
  string videodevice;
  // char *status;
  // char *pictName;
  struct v4l2_capability cap;
  struct v4l2_format fmt;
  struct v4l2_buffer buf;
  struct v4l2_requestbuffers rb;
  void *mem;
  unsigned char *tmpbuffer;
  unsigned char *framebuffer;
  bool isstreaming;
  //int grabmethod;
  int width;
  int height;
  int formatIn;
  int formatOut;
  int framesizeIn;
  //int signalquit;
  //int toggleAvi;
  //int getPict;
};

class Webcam2 {
protected:
	vdIn m_vd;
	//int m_fd;
	//int m_type;
	//void * m_buffer_start;
	SDL_Surface * m_frame;
	//struct v4l2_buffer m_bufferinfo;
	//int m_width;
	//int m_height;
	SDL_RWops * m_buffer_stream;
	bool m_bInitialized;
	bool m_bIsVisible;
public:
	int GetWidth(); // { return m_vd.width; };
	int GetHeight(); // { return m_vd.height; };
	bool IsVisible(); // { return m_bIsVisible; };
	Webcam2(const char * pDev, int width = -1, int height = -1);
	virtual ~Webcam2();
	int Initialize();
	int ActivateStreaming();
	int DeactivateStreaming();
	SDL_Surface * GrabFrame();
};

#endif
