#include "PASoundIn.hpp"
#include "Stimulus.hpp"

#include <iostream>

using std::cout;

// initialize static variables
bool PASoundIn::g_bInitialized = false;
PaStreamParameters PASoundIn::m_inputParameters;
PaStream * PASoundIn::m_stream = NULL;
SAMPLE * PASoundIn::m_pRecordedSamples = NULL;
unsigned long PASoundIn::m_ulFrameIx = 0;
unsigned long PASoundIn::m_ulMaxFrameIx = 0;

static int recordCallback( const void *inputBuffer, void *outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void *userData ) {

  // get the sound buffer
  PASoundIn * data = (PASoundIn *)userData;
  const SAMPLE *rptr = (const SAMPLE*)inputBuffer;
  SAMPLE *wptr = &data->m_pRecordedSamples[data->m_ulFrameIx];
  unsigned long framesLeft = data->m_ulMaxFrameIx - data->m_ulFrameIx;
  unsigned long framesToCalc;
  unsigned long i;
  int finished;

  (void) outputBuffer; /* Prevent unused variable warnings. */
  (void) timeInfo;
  (void) statusFlags;
  (void) userData;

  if( framesLeft < framesPerBuffer ) {
    framesToCalc = framesLeft;
    finished = paComplete;
  } else {
    framesToCalc = framesPerBuffer;
    finished = paContinue;
  }

  if( inputBuffer == NULL ) {
    for( i=0; i<framesToCalc; i++ ) {
      *wptr++ = SAMPLE_SILENCE;  /* left */
    }
  } else {
    for( i=0; i<framesToCalc; i++ ) {
      *wptr++ = *rptr++;  /* left */
    }
  }
  data->m_ulFrameIx += framesToCalc;
  return finished;
}

PASoundIn::PASoundIn(unsigned long ulDev) : InputDev(ulDev, 0) {
  PaError err = paNoError;
  m_stream = NULL;
  //m_strResource.assign("test.raw");
  
  unsigned long i;

  if (!PASoundIn::g_bInitialized) {
    //m_sOutputFile.assign("temp.raw");
    m_ulMaxFrameIx = NUM_SECONDS * SAMPLE_RATE; 
    m_pRecordedSamples = new SAMPLE[m_ulMaxFrameIx];
    if( m_pRecordedSamples == NULL ) {
      // TODO: do something!
    } else {}

    err = Pa_Initialize();
    // if( err != paNoError ) goto done; TODO: DO something!

    m_inputParameters.device = Pa_GetDefaultInputDevice(); // default input device 
    if (m_inputParameters.device == paNoDevice) {
      // TODO: DO SOMETHING!
    } else {}
    m_inputParameters.channelCount = 1;                    // mono input
    m_inputParameters.sampleFormat = PA_SAMPLE_TYPE;
    m_inputParameters.suggestedLatency = Pa_GetDeviceInfo( m_inputParameters.device )->defaultLowInputLatency;
    m_inputParameters.hostApiSpecificStreamInfo = NULL;

    PASoundIn::g_bInitialized = true;

  } else {}
}

PASoundIn::~PASoundIn() {
  if (m_pRecordedSamples) {
    delete [] m_pRecordedSamples;
    m_pRecordedSamples = NULL;
  } else {}
}

void PASoundIn::Prepare() {

  g_pErr->DFI("PASoundIn::Prepare", 0L, 3);
  PaError err = paNoError;
  unsigned long i;

  InputDev::ParseFilename(m_sOutputFile.c_str());
  //string s1 = g_pCurItemCell->GetFilenameFromResource(m_sOutputFile);
  //m_sOutputFile = s1;

  for (i = 0; i < m_ulMaxFrameIx; i++) {
    m_pRecordedSamples[i] = SAMPLE_SILENCE;
  }

  m_ulFrameIx = 0;

  err = Pa_OpenStream(
		      &m_stream,
		      &m_inputParameters,
		      NULL,                  // &outputParameters
		      SAMPLE_RATE,
		      FRAMES_PER_BUFFER,
		      paClipOff,      // we won't output out of range samples so don't bother clipping them 
		      recordCallback,
		      this );
  //if( err != paNoError ) goto done; TODO: DO SOMETHING!
#ifdef _LINUX_PLATFORM
  PaAlsa_EnableRealtimeScheduling( m_stream, true );
#endif
  InputDev::Prepare();
  g_pErr->DFO("PASoundIn::Prepare", 0L, 3);
}

void PASoundIn::Start() {
  PaError err = paNoError;

  err = Pa_StartStream( m_stream );
  // if( err != paNoError ) goto done;
  g_pErr->Debug("=== Now recording!! ===");

  InputDev::Start();

  /*
  while( ( err = Pa_IsStreamActive( m_stream ) ) == 1 )
    {
      Pa_Sleep(1000);
      cout << m_ulFrameIx << "\n";
      cout.flush();
    }
  */
  //if( err < 0 ) goto done;
  
  //if( err != paNoError ) goto done;
}

void PASoundIn::Stop() {
  g_pErr->Debug("==== RECORDING STOPPED ====");
  PaError err = paNoError;
  err = Pa_CloseStream( m_stream );

  InputDev::Stop();
  Save();
}

int PASoundIn::Save() {
  // TODO save file
  cout << "saving..." << std::endl;

  FILE  *fid;
  fid = fopen(m_sOutputFile.c_str(), "wb");
  if( fid == NULL ) {
    cout << "Could not open file.\n";
  } else {
    fwrite( m_pRecordedSamples, sizeof(SAMPLE), m_ulFrameIx, fid );
    fclose( fid );
  }
}
