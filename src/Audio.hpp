#ifndef EXP_AUDIO_INCLUDED
#define EXP_AUDIO_INCLUDED

#include "Device.hpp"
#include "Stimulus.hpp"

#include <vector>
using std::vector;

class Audio : public Device {
protected:
  vector<Stimulus *> m_vpStim;
public:
  Audio(long id = 0);
  virtual ~Audio();
};

#endif
