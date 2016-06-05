#ifndef EXP_WATCHSOCKETMSG_INCLUDED
#define EXP_WATCHSOCKETMSG_INCLUDED

#define EXP_ANYGAMEPADBUTTON -1

#include "Watch.hpp"
#include "SDL.h"
//#include "Stimulus.hpp"
//#include "StimulusBmp.hpp"
#include "Socket.hpp"

#include <vector>
using std::vector;

class WatchSocketMsg : public Watch {
protected:
	//unsigned short m_idSocket;
	//Socket * m_pSocket;
	bool m_bAny; // watch for any message at all?
	string m_strMsg;
public:
  WatchSocketMsg(long id, long idNext, ArgMMap mmap);
  virtual ~WatchSocketMsg();
  // inline Uint8 GetState() { return m_nState; }
  virtual bool CheckCondition(SDL_Event * pEvt);
	//virtual bool CheckWhich(Uint8 nGamePadUsed);
  //virtual int Prepare();  
  //virtual int Finish();
};

#endif
