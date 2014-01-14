#include "Stimulus.hpp"

class StimulusDisplay : public Stimulus {
public:
  StimulusDisplay(Device *, Stimulus *);
  virtual int Action();
  virtual int Prepare();
};
