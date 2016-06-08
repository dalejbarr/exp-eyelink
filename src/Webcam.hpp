#include <linux/videodev2.h>

class Webcam {
protected:
	int m_fd;
	int m_type;
	void * m_buffer_start;
	struct v4l2_buffer m_bufferinfo;
public:
	Webcam(int width /* = 320*/, int height /*=240*/);
	virtual ~Webcam();
};
