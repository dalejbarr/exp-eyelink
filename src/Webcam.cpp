#include "global.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "Webcam.hpp"

Webcam::Webcam(int width = 320, int height = 240) {
	if ((m_fd = open("/dev/video0", O_RDWR)) < 0) {
		g_pErr->Report("error opening webcam device");
	}

	// get device capabilities
	struct v4l2_capability cap;
	if (ioctl(m_fd, VIDIOC_QUERYCAP, &cap) < 0) {
		g_pErr->Report("error with VIDIOC_QUERYCAP");
	}

	if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
		g_pErr->Report("The device does not handle single-planar video capture.\n");
	}

	// choose format
	struct v4l2_format format;
	format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
	format.fmt.pix.width = width;
	format.fmt.pix.height = height;
	
	if(ioctl(m_fd, VIDIOC_S_FMT, &format) < 0) {
    g_pErr->Report("Error with VIDIOC_S_FMT");
	}

	// inform about future buffers
	struct v4l2_requestbuffers bufrequest;
	bufrequest.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufrequest.memory = V4L2_MEMORY_MMAP;
	bufrequest.count = 1;
 
	if(ioctl(m_fd, VIDIOC_REQBUFS, &bufrequest) < 0) {
		g_pErr->Report("error with VIDIOC_REQBUFS");
	}

	// allocate memory
	struct v4l2_buffer bufinfo;	
	memset(&bufinfo, 0, sizeof(bufinfo));
	
	bufinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	bufinfo.memory = V4L2_MEMORY_MMAP;
	bufinfo.index = 0;
 
	if(ioctl(m_fd, VIDIOC_QUERYBUF, &bufinfo) < 0){
		g_pErr->Report("error with VIDIOC_QUERYBUF");
	}

	// map memory 
	m_buffer_start = mmap(
														 NULL,
														 bufinfo.length,
														 PROT_READ | PROT_WRITE,
														 MAP_SHARED,
														 m_fd,
														 bufinfo.m.offset
														 );

	if (m_buffer_start == MAP_FAILED) {
		g_pErr->Report("error mapping memory for webcam");
	}
 
	memset(m_buffer_start, 0, bufinfo.length);

	// OK READY TO START RECEIVING FRAMES
	// struct v4l2_buffer m_bufferinfo;
	memset(&m_bufferinfo, 0, sizeof(m_bufferinfo));
 
	m_bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	m_bufferinfo.memory = V4L2_MEMORY_MMAP;
	m_bufferinfo.index = 0; /* Queueing buffer index 0. */
 
	// Activate streaming
	m_type = m_bufferinfo.type;

	if(ioctl(m_fd, VIDIOC_STREAMON, &m_type) < 0){
    g_pErr->Report("VIDIOC_STREAMON");
	}	
}

Webcam::~Webcam() {
	// Deactivate streaming
	if (m_fd) {
		if(ioctl(m_fd, VIDIOC_STREAMOFF, &m_type) < 0){
			g_pErr->Report("VIDIOC_STREAMOFF");
		}

		close(m_fd);
	} else {}
}
