#ifndef EXP_WATCHKEY_INCLUDED
#define EXP_WATCHKEY_INCLUDED

#include "Watch.hpp"
#include "global.hpp"

class WatchKey : public Watch {
protected:
  int m_nKey;
public:
  WatchKey(long id, long idNext, ArgMMap mmap);
  inline int GetKey() { return m_nKey; }
};

#endif
