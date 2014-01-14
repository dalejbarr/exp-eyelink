#ifndef EXP_WATCHMOUSEMOVE_INCLUDED
#define EXP_WATCHMOUSEMOVE_INCLUDED

#include "Watch.hpp"

class WatchMouseMove : public Watch {
protected:
public:
  WatchMouseMove(long id, long idNext, ArgMMap mmap) : Watch(id, idNext) {}
  virtual bool CheckCondition(SDL_Event * pEvt) { return 1; }
};

#endif
