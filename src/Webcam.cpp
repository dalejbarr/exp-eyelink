#include "global.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "Webcam.hpp"
#include "pastestr.hpp"
#include "Display_SDL.hpp"

extern Display_SDL * g_pDisplay;

Webcam::Webcam() {
	g_pErr->DFI("Webcam::Webcam", 0L, 3);

	m_width = -1;
	m_height = -1;
	m_bInitialized = false;
	m_buffer_stream = NULL;
	m_frame = NULL;
	m_bIsVisible = false;
	m_bIsStreaming = false;
 
	g_pErr->DFO("Webcam::Webcam", 0L, 3);
}

Webcam::~Webcam() {
	g_pErr->DFI("Webcam::~Webcam", 0L, 3);

	// Deactivate streaming
	if (m_fd) {
		DeactivateStreaming();
		close(m_fd);
	} else {}
	
	if (m_frame) {
		SDL_FreeSurface(m_frame);
		m_frame = NULL;
	}

	if (m_buffer_stream) {
		SDL_RWclose(m_buffer_stream);
		m_buffer_stream = NULL;
	}

	g_pErr->DFO("Webcam::~Webcam", 0L, 3);
}

int Webcam::Initialize() {
	if (!m_bInitialized) {

		m_bIsVisible = false;
		m_bInitialized = true;
		m_bIsStreaming = false;

		m_width = g_pDisplay->m_nWidth;
		m_height = g_pDisplay->m_nHeight;

		if ((m_fd = open("/dev/video0", O_RDWR)) < 0) {
			g_pErr->Report("error opening webcam device");
		}

		// get device capabilities
		struct v4l2_capability cap;
		memset (&cap, 0, sizeof (struct v4l2_capability));
		if (ioctl(m_fd, VIDIOC_QUERYCAP, &cap) < 0) {
			g_pErr->Report("error with VIDIOC_QUERYCAP");
		}

		if (!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE)) {
			g_pErr->Report("The device does not handle single-planar video capture.\n");
		}

		if (!(cap.capabilities & V4L2_CAP_STREAMING)) {
			g_pErr->Report("webcam does not support streaming i/o");
		} else {
			/*
			if (!(cap.capabilities & V4L2_CAP_READWRITE)) {
				g_pErr->Report("webcam does not support read i/o");
			}
			*/
		}

		// choose format
		struct v4l2_format format;
		format.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		format.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
		// format.fmt.pix.pixelformat = V4L2_PIX_FMT_YUYV;
		//format.fmt.pix.pixelformat = V4L2_PIX_FMT_H264;
		format.fmt.pix.width = m_width;
		format.fmt.pix.height = m_height;
	
		if(ioctl(m_fd, VIDIOC_S_FMT, &format) < 0) {
			g_pErr->Report("Error with VIDIOC_S_FMT");
		}

		if (m_width != format.fmt.pix.width) {
			g_pErr->Debug(pastestr::paste("sdsd", " ",
																		"warning: requested webcam width was", (long) m_width,
																		"closest acceptable was", (long) format.fmt.pix.width));
			m_width = format.fmt.pix.width;
		}
		if (m_height != format.fmt.pix.height) {
			g_pErr->Debug(pastestr::paste("sdsd", " ",
																		"warning: requested webcam height was", (long) m_height,
																		"closest acceptable was", (long) format.fmt.pix.height));
			m_height = format.fmt.pix.height;
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

		m_type = m_bufferinfo.type;
	}
}

int Webcam::ActivateStreaming() {
	if (!m_bIsStreaming) {
		if(ioctl(m_fd, VIDIOC_STREAMON, &m_type) < 0){
			g_pErr->Report("VIDIOC_STREAMON");
		} else {
			m_bIsStreaming = true;
		}
	}

	return 0;
}

int Webcam::DeactivateStreaming() {
	// Deactivate streaming
	if (m_bIsStreaming) {
		if(ioctl(m_fd, VIDIOC_STREAMOFF, &m_type) < 0){
			g_pErr->Report("Error with VIDIOC_STREAMOFF");
		} else {
			m_bIsStreaming = false;
	}

	m_bIsVisible = false;

	return 0;
}

SDL_Surface * Webcam::GrabFrame() {
	// Put the buffer in the incoming queue.
	if(ioctl(m_fd, VIDIOC_QBUF, &m_bufferinfo) < 0){
		g_pErr->Report("Webcam::GrabFrame: VIDIOC_QBUF");
	}
 
	// The buffer's waiting in the outgoing queue.
	if(ioctl(m_fd, VIDIOC_DQBUF, &m_bufferinfo) < 0){
		g_pErr->Report("Webcam::GrabFrame: VIDIOC_QBUF");
	}

	/*
#define HEADERFRAME1 0xaf

    memcpy (vd->tmpbuffer, vd->mem[vd->buf.index], HEADERFRAME1);
    memcpy (vd->tmpbuffer + HEADERFRAME1, dht_data, DHT_SIZE);
    memcpy (vd->tmpbuffer + HEADERFRAME1 + DHT_SIZE,
	    vd->mem[vd->buf.index] + HEADERFRAME1,
	    (vd->buf.bytesused - HEADERFRAME1));
	 */
 
	// Create a stream based on our buffer.
	m_buffer_stream = SDL_RWFromMem(m_buffer_start, m_bufferinfo.length);
 
	// Create a surface using the data coming out of the above stream.
	if (!IMG_isJPG(m_buffer_stream)) {
		g_pErr->Report("webcam frame not valid JPG format");
	}

	m_frame = IMG_Load_RW(m_buffer_stream, 0);
	if (m_frame == NULL) {
		g_pErr->Report(pastestr::paste("ss", "", "couldn't grab frame: ", IMG_GetError()));
	} else {
		m_bIsVisible = true;
	}

	return m_frame;
}
