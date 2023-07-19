#ifndef SOCKETLISTENER_INCLUDED
#define SOCKETLISTENER_INCLUDED

#include "Socket.hpp"

// the point of this class is to listen on a given inet socket channel
// and throw up events whenever a message is received

class SocketListener {
protected:
	InputDevPtr m_pDev;
	SDL_Thread * m_pThread;
public:
	static bool m_bContinue;
	SocketListener(InputDevPtr pDev);
	int static mainThread(void * pVoid);
	int Main();
	int CheckForMissedMessages();
	int Signal();
	virtual ~SocketListener();
};

#endif
