#ifndef SOUNDINPUT_INCLUDED
#define SOUNDINPUT_INCLUDED

#include "InputDev.hpp"

class SoundInput : public InputDev {
public:
  SoundInput(unsigned long idDev);
  virtual ~SoundInput();
	virtual void Initialize();
	virtual void Destroy();
  virtual void Prepare() = 0;
  virtual void Start() = 0;
  virtual void Stop() = 0;
};

#endif
