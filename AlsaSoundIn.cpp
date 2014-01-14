#include "AlsaSoundIn.hpp"
#include "Stimulus.hpp"
#include "pastestr.hpp"

#include <iostream>

bool AlsaSoundIn::s_bIsRecording = false;
bool AlsaSoundIn::s_bIsActive = true;
bool AlsaSoundIn::s_bIsInitialized = false;

AlsaSoundIn::AlsaSoundIn(unsigned long ulDev) : InputDev(ulDev, 0) {  
  unsigned long i;

  m_pBuffer = NULL;
  m_thread = NULL;

  if (!AlsaSoundIn::s_bIsInitialized) {
    g_pErr->Debug("=====> Initializing ALSA SOUNDIN");
    int rc;
    snd_pcm_hw_params_t *params;
    unsigned int val;
    int dir;

    // Open PCM device for recording (capture).
    rc = snd_pcm_open(&m_pHandle, "default", SND_PCM_STREAM_CAPTURE, 0);
    if (rc < 0) {
      g_pErr->Report(pastestr::paste("ss", "", "couldn't open pcm device for recording: ",
				     snd_strerror(rc)));
    } else {}

    // Allocate a hardware parameters object. 
    snd_pcm_hw_params_alloca(&params);

    // Fill it in with default values.
    snd_pcm_hw_params_any(m_pHandle, params);

    // Set the desired hardware parameters. 
    // Interleaved mode 
    snd_pcm_hw_params_set_access(m_pHandle, params, SND_PCM_ACCESS_RW_INTERLEAVED);

    // Signed 16-bit little-endian format 
    snd_pcm_hw_params_set_format(m_pHandle, params, SND_PCM_FORMAT_S16_LE);

    // One channel (mono) 
    snd_pcm_hw_params_set_channels(m_pHandle, params, 1);

    // 44100 bits/second sampling rate (CD quality) 
    val = 48000;
    snd_pcm_hw_params_set_rate_near(m_pHandle, params, &val, &dir);

    // Set period size to 32 frames.
    m_frames = 32;
    snd_pcm_hw_params_set_period_size_near(m_pHandle, params, &m_frames, &dir);

    // Write the parameters to the driver 
    rc = snd_pcm_hw_params(m_pHandle, params);
    if (rc < 0) {
      g_pErr->Report(pastestr::paste("ss", "", "unable to set hw parameters: ",
				     snd_strerror(rc)));
    } else {}

    // Use a buffer large enough to hold one period 
    snd_pcm_hw_params_get_period_size(params, &m_frames, &dir);
    m_size = m_frames * 2; // 2 bytes/sample, 1 channel
    m_pBuffer = new char[m_size];

    AlsaSoundIn::s_bIsInitialized = true;
  } else {}
}

AlsaSoundIn::~AlsaSoundIn() {
  if (m_thread) {
    SDL_WaitThread(m_thread, NULL);
    m_thread = NULL;
  } else {}

  if (AlsaSoundIn::s_bIsInitialized) {
    g_pErr->Debug("=====> Closing ALSA SOUNDIN");
    snd_pcm_drain(m_pHandle);
    snd_pcm_close(m_pHandle);
  } else {}

  if (m_pBuffer) {
    delete [] m_pBuffer;
    m_pBuffer = NULL;
  } else {}

  if (m_outfile.is_open()) {
    m_outfile.close();
  } else {}
}

int alsamain(void *pClass) {
  return (((AlsaSoundIn *)pClass)->Main());
}

int AlsaSoundIn::Main() {
  int rc;
  bool bFirst = true;

  g_pErr->DFI("AlsaSoundIn::MAIN", 0L, 3);

  while (s_bIsActive) {
    if (s_bIsRecording) {
      if (bFirst) {
	g_pErr->Debug("\n!! ALSA now recording !!\n");
	bFirst = false;
      } else {}
      rc = snd_pcm_readi(m_pHandle, m_pBuffer, m_frames);
      if (rc == -EPIPE) {
	/* EPIPE means overrun */
	g_pErr->Debug("warning: ALSA overrun occurred");
	snd_pcm_prepare(m_pHandle);
      } else if (rc < 0) {
	g_pErr->Debug(pastestr::paste("ss", "", "error from read: ", 
				      snd_strerror(rc)));
      } else if (rc != (int)m_frames) {
	g_pErr->Debug(pastestr::paste("sds", "", "short read, read ",
				      rc, " frames"));
      }
      m_outfile.write(m_pBuffer, m_size);
    } else {}
  }

  g_pErr->DFO("AlsaSoundIn::MAIN", 0L, 3);

  return (0);
}

void AlsaSoundIn::Prepare() {
  g_pErr->DFI("AlsaSoundIn::Prepare", 0L, 3);

  InputDev::ParseFilename(m_sOutputFile.c_str());
  InputDev::Prepare();

  m_outfile.open(m_sOutputFile.c_str(), ios::out|ios::binary|ios::trunc);
  AlsaSoundIn::s_bIsActive = true;
  AlsaSoundIn::s_bIsRecording = false;

  m_thread = SDL_CreateThread(alsamain, this);
  if ( m_thread == NULL ) {
    g_pErr->Report(pastestr::paste("ss", "", "Unable to create thread:", SDL_GetError()));
  } else {}

  g_pErr->DFO("AlsaSoundIn::Prepare", 0L, 3);
}

void AlsaSoundIn::Start() {

  // TODO: start recording
  s_bIsRecording = true;
  InputDev::Start();
}

void AlsaSoundIn::Stop() {

  AlsaSoundIn::s_bIsRecording = false;
  AlsaSoundIn::s_bIsActive = false;

  SDL_WaitThread(m_thread, NULL);
  g_pErr->Debug("==== RECORDING STOPPED ====");

  m_thread = NULL;

  if (m_outfile.is_open()) {
    m_outfile.close();
  } else {}

  InputDev::Stop();
}
