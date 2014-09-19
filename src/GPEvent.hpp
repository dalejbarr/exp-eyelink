#ifndef GPEVENT_INCLUDED
#define GPEVENT_INCLUDED

#include "RespObj.hpp"

class GPEvent : public RespObj {
public:
  int m_nIndex;
  int m_nButton;
  GPEvent(int ix = 0, int nButton = 0, unsigned long msec = 0);
  virtual void Print();
};
#endif
