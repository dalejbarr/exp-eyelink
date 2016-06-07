#include "SoundInput.hpp"
#include "pastestr.hpp"

SoundInput::SoundInput(unsigned long ulDev) : InputDev(ulDev) {
	g_pErr->Debug("¬¬ INITIALIZING SoundInput ¬¬");
	Initialize();
}

SoundInput::~SoundInput() {
	g_pErr->Debug("¬¬ DESTROYING SoundInput ¬¬");
	Destroy();
}

// Interleaved buffers
int SoundInput::input(void * /*outputBuffer*/, void *inputBuffer, unsigned int nBufferFrames,
											double /*streamTime*/, RtAudioStreamStatus /*status*/, void *data) {
  InputData *iData = (InputData *) data;

  // Simply copy the data to our allocated buffer.
  unsigned int frames = nBufferFrames;
  if ( iData->frameCounter + nBufferFrames > iData->totalFrames ) {
    frames = iData->totalFrames - iData->frameCounter;
    iData->bufferBytes = frames * iData->channels * sizeof( MY_TYPE );
  } else {}

  unsigned long offset = iData->frameCounter * iData->channels;
  memcpy( iData->buffer+offset, inputBuffer, iData->bufferBytes );
  iData->frameCounter += frames;

  if ( iData->frameCounter >= iData->totalFrames ) return 2;
  return 0;
}

void SoundInput::Initialize() {
  //unsigned int channels, fs, bufferFrames, device = 0, offset = 0;
  g_pErr->DFI("SoundInput::Initialize", (long) 0, 1);

  //std::cout << m_adc.getDeviceCount() << std::endl;
  if ( m_adc.getDeviceCount() < 1 ) {
    g_pErr->Report("No audio devices found!");
    return;
  } else {}

  m_nDevice = 0;
  if (!g_pConfig->GetConfigInt("Audio_Input_Device_ID", &m_nDevice)) {
    // TODO get default device
    g_pErr->Debug("no device specified in EXPConfig ('Audio_Input_Device_ID'); using default");
  } else {}

  m_framesPerSec = 22050;
  if (!g_pConfig->GetConfigInt("Audio_Sampling_Rate", &m_framesPerSec)) {
    g_pErr->Debug("no 'Audio_Sampling_Rate' specified in EXPConfig; defaulting to 22050");
  } else {}

  m_nChannels = 1;
  if (!g_pConfig->GetConfigInt("Audio_Input_Channels", &m_nChannels)) {
    g_pErr->Debug("no 'Audio_Input_Channels' specified in EXPConfig; defaulting to 1 (mono)");
  } else {}

  m_data.buffer = 0;

  //m_nDevice = 3;
  m_nOffset = 0;
  m_nMaxSecsToRecord = 30;

  // let rtaudio print warnings to stderr
  m_adc.showWarnings( true );

  m_nBufferFrames = 512;
  if (m_nDevice == 0) {
    m_inputParams.deviceId = m_adc.getDefaultInputDevice();
  } else {
    m_inputParams.deviceId = m_nDevice;
  }
  m_inputParams.nChannels = m_nChannels;
  m_inputParams.firstChannel = m_nOffset;
  g_pErr->Debug(pastestr::paste("sdsd", "", "nchannels ", (long) m_inputParams.nChannels, " ", (long) m_nChannels));

  g_pErr->DFO("SoundInput::Initialize", (long) 0, 1);
}

void SoundInput::Destroy() {
  if (m_adc.isStreamOpen()) { 
		m_adc.closeStream();
	} else {}
  if (m_data.buffer) {
		free( m_data.buffer );	
	} else {}
}

void SoundInput::Prepare() {
	// TODO: store sound file name
	g_pErr->DFI("SoundInput::Prepare", (long) 0, 1);
  InputDev::ParseFilename(m_sOutputFile.c_str());
  InputDev::Prepare();

	g_pErr->Debug(pastestr::paste("sdsd", "", "nchannels ", (long) m_inputParams.nChannels, " ", (long) m_nChannels));

  try {
    m_adc.openStream( NULL, &m_inputParams, SOUNDINPUT_FORMAT, 
											m_framesPerSec, &m_nBufferFrames, &SoundInput::input, (void *)&m_data );
  }
  catch ( RtAudioError& e ) {
		g_pErr->Report(e.getMessage());
  }

  m_data.bufferBytes = m_nBufferFrames * m_nChannels * sizeof( MY_TYPE );
  m_data.totalFrames = (unsigned long) (m_framesPerSec * m_nMaxSecsToRecord);
  m_data.frameCounter = 0;
  m_data.channels = m_nChannels;
	
  m_totalBytes = m_data.totalFrames * m_nChannels * sizeof( MY_TYPE );

  // Allocate the entire data buffer
	if (!m_data.buffer) {
		m_data.buffer = (MY_TYPE*) malloc( m_totalBytes );
	} else {}
  if ( m_data.buffer == 0 ) {
		g_pErr->Report("Memory allocation error ... quitting!");
  }	else {}
	
	g_pErr->DFO("SoundInput::Prepare", (long) 0, 1);
}

void SoundInput::Start() {
  try {
	  m_adc.startStream();
  }
  catch ( RtAudioError& e ) {
		g_pErr->Report(e.getMessage());
  }
}

void SoundInput::Stop() {
	double db = m_adc.getStreamTime();
	m_adc.abortStream();
	double da = m_adc.getStreamTime();
	std::cout << "\n\n\nSTREAM TIME: " << db << " " << da << "\n\n\n";
  unsigned long tbytes = m_totalBytes;
  unsigned long tframes = (unsigned long) m_framesPerSec * m_nMaxSecsToRecord;

	if (db < ((double) m_nMaxSecsToRecord)) {  
  	tframes = (unsigned long) (m_framesPerSec * db);
    tbytes = tframes * m_nChannels * sizeof( MY_TYPE );  
		std::cout << "tbytes = " << tbytes << std::endl;
  } else {}

  if (m_adc.isStreamOpen()) { 
		m_adc.closeStream();
	} else {}

	writeWAVData(m_sOutputFile.c_str(), m_data.buffer, tbytes, m_framesPerSec, m_nChannels);
}
