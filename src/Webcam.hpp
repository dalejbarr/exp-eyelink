#ifndef SBX_WEBCAM_INCLUDED
#define SBX_WEBCAM_INCLUDED

#include <linux/videodev2.h>

class Webcam {
protected:
	int m_fd;
	int m_type;
	void * m_buffer_start;
	SDL_Surface * m_frame;
	struct v4l2_buffer m_bufferinfo;
	int m_width;
	int m_height;
	SDL_RWops * m_buffer_stream;
	bool m_bInitialized;
	bool m_bIsVisible;
	bool m_bIsStreaming;
public:
	inline int GetWidth() { return m_width; };
	inline int GetHeight() { return m_height; };
	inline bool IsVisible() { returm m_bIsVisible; };
	Webcam();
	virtual ~Webcam();
	int Initialize();
	int ActivateStreaming();
	int DeactivateStreaming();
	SDL_Surface * GrabFrame();

};

#endif
