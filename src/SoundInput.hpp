#ifndef SOUNDINPUT_INCLUDED
#define SOUNDINPUT_INCLUDED

#include "InputDev.hpp"
#include "RtAudio.h"

#define SOUNDINPUT_FORMAT RTAUDIO_SINT16
typedef signed short MY_TYPE;

struct InputData {
  MY_TYPE* buffer;
  unsigned long bufferBytes;
  unsigned long totalFrames;
  unsigned long frameCounter;
  unsigned int channels;
};

class SoundInput : public InputDev {
protected:
  RtAudio m_adc;
  InputData m_data;
	int m_nChannels; // = 1
	int m_framesPerSec;// = 16000;
	int m_nDevice; // = 3;
	int m_nOffset; // = 0;
	int m_nMaxSecsToRecord; // = 32;
  unsigned int m_nBufferFrames; // = 512;
  unsigned long m_totalBytes;
  RtAudio::StreamParameters m_inputParams;
public:
  SoundInput(unsigned long idDev);
  virtual ~SoundInput();
	virtual void Initialize();
	virtual void Destroy();
  virtual void Prepare();
  virtual void Start();
  virtual void Stop();
  static int input(void * /*outputBuffer*/, 
										 void *inputBuffer, 
										 unsigned int nBufferFrames,
										 double /*streamTime*/, 
										 RtAudioStreamStatus /*status*/, 
										 void *data );
};

// this code was written by Josh Parnell
// and downloaded from: 
//  http://joshparnell.com/blog/2013/03/21/how-to-write-a-wav-file-in-c/
#include <fstream>

template <typename T>
void write(std::ofstream& stream, const T& t) {
  stream.write((const char*)&t, sizeof(T));
}

template <typename T>
void writeFormat(std::ofstream& stream) {
  write<short>(stream, 1);
}

template <> inline
void writeFormat<float>(std::ofstream& stream) {
  write<short>(stream, 3);
}

template <typename SampleType>
void writeWAVData(
									char const* outFile,
									SampleType* buf,
									size_t bufSize,
									int sampleRate,
									short channels)
{
	std::ofstream stream(outFile, std::ios::binary);
  stream.write("RIFF", 4);
  write<int>(stream, 36 + bufSize);
  stream.write("WAVE", 4);
  stream.write("fmt ", 4);
  write<int>(stream, 16);
  writeFormat<SampleType>(stream);                                // Format
  write<short>(stream, channels);                                 // Channels
  write<int>(stream, sampleRate);                                 // Sample Rate
  write<int>(stream, sampleRate * channels * sizeof(SampleType)); // Byterate
  write<short>(stream, channels * sizeof(SampleType));            // Frame size
  write<short>(stream, 8 * sizeof(SampleType));                   // Bits per sample
  stream.write("data", 4);
  stream.write((const char*)&bufSize, 4);
  stream.write((const char*)buf, bufSize);
}

#endif
