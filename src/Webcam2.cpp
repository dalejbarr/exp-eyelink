#include "global.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>

#include "Webcam2.hpp"
#include "pastestr.hpp"
#include "Display_SDL.hpp"

extern Display_SDL * g_pDisplay;

static unsigned char dht_data[DHT_SIZE] = {
  0xff, 0xc4, 0x01, 0xa2, 0x00, 0x00, 0x01, 0x05, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02,
  0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x01, 0x00, 0x03,
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
  0x0a, 0x0b, 0x10, 0x00, 0x02, 0x01, 0x03, 0x03, 0x02, 0x04, 0x03, 0x05,
  0x05, 0x04, 0x04, 0x00, 0x00, 0x01, 0x7d, 0x01, 0x02, 0x03, 0x00, 0x04,
  0x11, 0x05, 0x12, 0x21, 0x31, 0x41, 0x06, 0x13, 0x51, 0x61, 0x07, 0x22,
  0x71, 0x14, 0x32, 0x81, 0x91, 0xa1, 0x08, 0x23, 0x42, 0xb1, 0xc1, 0x15,
  0x52, 0xd1, 0xf0, 0x24, 0x33, 0x62, 0x72, 0x82, 0x09, 0x0a, 0x16, 0x17,
  0x18, 0x19, 0x1a, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x34, 0x35, 0x36,
  0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
  0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66,
  0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
  0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94, 0x95,
  0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
  0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba, 0xc2,
  0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4, 0xd5,
  0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
  0xe8, 0xe9, 0xea, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9,
  0xfa, 0x11, 0x00, 0x02, 0x01, 0x02, 0x04, 0x04, 0x03, 0x04, 0x07, 0x05,
  0x04, 0x04, 0x00, 0x01, 0x02, 0x77, 0x00, 0x01, 0x02, 0x03, 0x11, 0x04,
  0x05, 0x21, 0x31, 0x06, 0x12, 0x41, 0x51, 0x07, 0x61, 0x71, 0x13, 0x22,
  0x32, 0x81, 0x08, 0x14, 0x42, 0x91, 0xa1, 0xb1, 0xc1, 0x09, 0x23, 0x33,
  0x52, 0xf0, 0x15, 0x62, 0x72, 0xd1, 0x0a, 0x16, 0x24, 0x34, 0xe1, 0x25,
  0xf1, 0x17, 0x18, 0x19, 0x1a, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x35, 0x36,
  0x37, 0x38, 0x39, 0x3a, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4a,
  0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5a, 0x63, 0x64, 0x65, 0x66,
  0x67, 0x68, 0x69, 0x6a, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7a,
  0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x92, 0x93, 0x94,
  0x95, 0x96, 0x97, 0x98, 0x99, 0x9a, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7,
  0xa8, 0xa9, 0xaa, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8, 0xb9, 0xba,
  0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xd2, 0xd3, 0xd4,
  0xd5, 0xd6, 0xd7, 0xd8, 0xd9, 0xda, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7,
  0xe8, 0xe9, 0xea, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa
};

Webcam2::Webcam2(const char * pDev, int width, int height) {
	g_pErr->DFI("Webcam2::Webcam2", 0L, 3);

	m_bInitialized = false;
	m_frame = NULL;
	m_bIsVisible = false;
	m_vd.width = width;
	m_vd.height = height;
	m_vd.isstreaming = false;

	m_vd.videodevice.assign(pDev);
	m_buffer_stream = NULL;
 
	g_pErr->DFO("Webcam2::Webcam2", 0L, 3);
}

Webcam2::~Webcam2() {
	g_pErr->DFI("Webcam2::~Webcam2", 0L, 3);

	if (m_frame) {
		SDL_FreeSurface(m_frame);
		m_frame = NULL;
	}

	if (m_vd.tmpbuffer) {
		delete [] m_vd.tmpbuffer;
		m_vd.tmpbuffer = NULL;
	}

	if (m_vd.framebuffer) {
		delete [] m_vd.framebuffer;
		m_vd.framebuffer = NULL;
	}

  if (m_vd.isstreaming)
    DeactivateStreaming();

	if (m_buffer_stream) {
		SDL_RWclose(m_buffer_stream);		
		m_buffer_stream = NULL;
	}

  // If the memory maps are not released the device will remain opened even
  //     after a call to close(); 
	munmap(m_vd.mem, m_vd.buf.length);
	close(m_vd.fd);

	g_pErr->DFO("Webcam2::~Webcam2", 0L, 3);
}

int Webcam2::Initialize() {
	if (!m_bInitialized) {
		m_bInitialized = true; // only run once
		m_bIsVisible = false;
		if (m_frame) {  // should never happen!
			g_pErr->Debug("*WARNING: in Webcam2::Initialize, m_frame was not null; this should never happen");
			SDL_FreeSurface(m_frame);
		}
		m_frame = NULL;
		
		if (m_vd.width == -1) {
			m_vd.width = g_pDisplay->m_nWidth;
		}

		if (m_vd.height == -1) {
			m_vd.height = g_pDisplay->m_nHeight;
		}

		m_vd.formatIn = V4L2_PIX_FMT_MJPEG;
		
		// initialize
		if ((m_vd.fd = open(m_vd.videodevice.c_str(), O_RDWR)) == -1) {
			g_pErr->Report("ERROR opening V4L interface \n");
		}
		memset(&(m_vd.cap), 0, sizeof (struct v4l2_capability));
		if (ioctl(m_vd.fd, VIDIOC_QUERYCAP, &(m_vd.cap)) < 0) {
			g_pErr->Report("Error opening device: unable to query device.\n");
		}
		if ((m_vd.cap.capabilities & V4L2_CAP_VIDEO_CAPTURE) == 0) {
			g_pErr->Report("Error opening device: video capture not supported.\n");
		}
    if (!(m_vd.cap.capabilities & V4L2_CAP_STREAMING)) {
      g_pErr->Report("webcam does not support streaming i/o");
		}

		// set format in
		memset(&(m_vd.fmt), 0, sizeof (struct v4l2_format));
		m_vd.fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		m_vd.fmt.fmt.pix.width = m_vd.width;
		m_vd.fmt.fmt.pix.height = m_vd.height;
		m_vd.fmt.fmt.pix.pixelformat = m_vd.formatIn;
		m_vd.fmt.fmt.pix.field = V4L2_FIELD_ANY;
		if(ioctl(m_vd.fd, VIDIOC_S_FMT, &(m_vd.fmt)) < 0) {
			g_pErr->Report("Unable to set format.");
		}
		if ((m_vd.fmt.fmt.pix.width != m_vd.width) ||
				(m_vd.fmt.fmt.pix.height != m_vd.height)) {
			g_pErr->Debug(pastestr::paste("sdsd", " ",
																		"format requested unavailable, got width", 
																		(long) m_vd.fmt.fmt.pix.width, 
																		"height", 
																		(long) m_vd.fmt.fmt.pix.height));
			m_vd.width = m_vd.fmt.fmt.pix.width;
			m_vd.height = m_vd.fmt.fmt.pix.height;
		}

		// request buffer
		memset (&(m_vd.rb), 0, sizeof (struct v4l2_requestbuffers));
		m_vd.rb.count = 1;
		m_vd.rb.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		m_vd.rb.memory = V4L2_MEMORY_MMAP;
		if (ioctl(m_vd.fd, VIDIOC_REQBUFS, &(m_vd.rb)) < 0) {
			g_pErr->Report("Unable to allocate buffers");
		}

		// map the buffer
    memset(&(m_vd.buf), 0, sizeof (struct v4l2_buffer));
    m_vd.buf.index = 0;
    m_vd.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    m_vd.buf.memory = V4L2_MEMORY_MMAP;
    if (ioctl(m_vd.fd, VIDIOC_QUERYBUF, &(m_vd.buf)) < 0) {
      g_pErr->Report("Unable to query buffer");
    }
    m_vd.mem = mmap(0 /* start anywhere */ ,
										m_vd.buf.length, PROT_READ, MAP_SHARED, m_vd.fd,
										m_vd.buf.m.offset);
    if (m_vd.mem == MAP_FAILED) {
      g_pErr->Report("Unable to map buffer");
    }

		// Queue the buffer 
		/*
    memset(&(m_vd.buf), 0, sizeof (struct v4l2_buffer));
    m_vd.buf.index = 0;
    m_vd.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    m_vd.buf.memory = V4L2_MEMORY_MMAP;
    if(ioctl(m_vd.fd, VIDIOC_QBUF, &(m_vd.buf)) < 0) {
      g_pErr->Report("Unable to queue buffer");
    }
		*/

		// allocate temporary buffer to hold JPG data
		m_vd.framesizeIn = (m_vd.width * m_vd.height << 1);
    m_vd.tmpbuffer = new unsigned char[m_vd.framesizeIn];
		memset(m_vd.tmpbuffer, 0, m_vd.framesizeIn);
		m_vd.framebuffer = new unsigned char[m_vd.width * (m_vd.height + 8) * 2];		
	}

	return 0;
}


int Webcam2::ActivateStreaming() {
	int ret = 0;

	if (!m_vd.isstreaming) {
		int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		ret = ioctl(m_vd.fd, VIDIOC_STREAMON, &type);
		if (ret < 0) {
			g_pErr->Report("Unable to capture");
		}
		m_vd.isstreaming = true;
	}

  return ret;
}

int Webcam2::DeactivateStreaming() {
	int ret = 0;

	if (m_vd.isstreaming) {
		int type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		ret = ioctl(m_vd.fd, VIDIOC_STREAMOFF, &type);
		if (ret < 0) {
			g_pErr->Report("Unable to stop capture");
		}
		m_vd.isstreaming = false;
		m_bIsVisible = false;
	}

  return ret;
}

SDL_Surface * Webcam2::GrabFrame() {	
  if (!m_vd.isstreaming)
    if (!ActivateStreaming())
      return NULL;

	// Queue the buffer 
	memset(&(m_vd.buf), 0, sizeof (struct v4l2_buffer));
	m_vd.buf.index = 0;
	m_vd.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	m_vd.buf.memory = V4L2_MEMORY_MMAP;
	if(ioctl(m_vd.fd, VIDIOC_QBUF, &(m_vd.buf)) < 0) {
		g_pErr->Report("Unable to queue buffer");
	}

  //memset(&(m_vd.buf), 0, sizeof (struct v4l2_buffer));
  //m_vd.buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  //m_vd.buf.memory = V4L2_MEMORY_MMAP;
  if (ioctl(m_vd.fd, VIDIOC_DQBUF, &(m_vd.buf)) < 0) {
		g_pErr->Report("Unable to dequeue buffer");
  }
	memcpy(m_vd.tmpbuffer, m_vd.mem, HEADERFRAME1);
	memcpy(m_vd.tmpbuffer + HEADERFRAME1, dht_data, DHT_SIZE);
	memcpy(m_vd.tmpbuffer + HEADERFRAME1 + DHT_SIZE,
				 m_vd.mem + HEADERFRAME1,
				 (m_vd.buf.bytesused - HEADERFRAME1));

	m_buffer_stream = SDL_RWFromMem(m_vd.tmpbuffer, m_vd.buf.bytesused + DHT_SIZE);

	if (!IMG_isJPG(m_buffer_stream)) {
		g_pErr->Report("webcam frame not valid JPG format");
	}

	// deallocate before reallocating
	if (m_frame) {
		SDL_FreeSurface(m_frame);
	}

	m_frame = IMG_Load_RW(m_buffer_stream, 0);
	if (m_frame == NULL) {
		g_pErr->Report(pastestr::paste("ss", "", "couldn't grab frame: ", IMG_GetError()));
	} else {
		m_bIsVisible = true;
	}
	/*
	// Create a stream based on our buffer.
 
	// Create a surface using the data coming out of the above stream.

	 */

	//IMG_Load_RW();

	// Put the buffer in the incoming queue.
	/*
	if(ioctl(m_fd, VIDIOC_QBUF, &m_bufferinfo) < 0){
		g_pErr->Report("Webcam2::GrabFrame: VIDIOC_QBUF");
	}
 
	// The buffer's waiting in the outgoing queue.
	if(ioctl(m_fd, VIDIOC_DQBUF, &m_bufferinfo) < 0){
		g_pErr->Report("Webcam2::GrabFrame: VIDIOC_QBUF");
	}
 
	// Create a stream based on our buffer.
	m_buffer_stream = SDL_RWFromMem(m_buffer_start, m_bufferinfo.length);
 
	// Create a surface using the data coming out of the above stream.
	if (!IMG_isJPG(m_buffer_stream)) {
		g_pErr->Report("webcam frame not valid JPG format");
	}

	m_frame = IMG_Load_RW(m_buffer_stream, 0);
	if (m_frame == NULL) {
		g_pErr->Report(pastestr::paste("ss", "", "couldn't grab frame: ", IMG_GetError()));
	}

	*/
	return m_frame;
}

int Webcam2::GetWidth() {
	return m_vd.width;
}

int Webcam2::GetHeight() {
	return m_vd.height;
}

bool Webcam2::IsVisible() {
	return m_bIsVisible;
}
