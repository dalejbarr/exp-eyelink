#ifndef EXP_DISPLAY_INCLUDED
#define EXP_DISPLAY_INCLUDED

#include "Device.hpp"
#include "Stimulus.hpp"

#include <vector>
using std::vector;

class Display : public Device {
protected:
  vector<Stimulus *> m_vpStim;
public:
  int m_nWidth;
  int m_nHeight;
  Display(long id = 0);
  virtual ~Display();
  virtual int SetColorKey(int r, int g, int b) = 0;
  virtual int Draw();
};

#endif
