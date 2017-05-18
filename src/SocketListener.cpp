#include "SocketListener.hpp"
#include <signal.h>
#include <iostream>
#include "pastestr.hpp"
#include "CmdID.hpp"

bool SocketListener::m_bContinue = false;

static void sigaction_handler(int sig, siginfo_t *siginfo, void * context) {
	g_pErr->Debug("in sigaction handler");
}

SocketListener::SocketListener(InputDevPtr pDev) {
	SocketListener::m_bContinue = true;
	m_pDev = pDev;

	m_pThread = SDL_CreateThread(SocketListener::mainThread, this);
}

SocketListener::~SocketListener() {
	SocketListener::m_bContinue = false;

	/*
	struct sigaction act;

	memset(&act, '\0', sizeof(act));

	act.sa_sigaction = &sigaction_handler;
	act.sa_flags = SA_SIGINFO;

	if (sigaction(SIGINT, &act, NULL) < 0) {
		g_pErr->Report("error sending SIGINT");
	}
	*/

	g_pErr->Debug("waiting for thread to finish...");
	//int nStatus = 0;
	//SDL_WaitThread(m_pThread, &nStatus);
	SDL_KillThread(m_pThread);
	g_pErr->Debug("thread completed...");
}


int SocketListener::mainThread(void * pVoid) {
	SocketListener * pThis;

	pThis = (SocketListener *) pVoid;
	return pThis->Main();
}

int SocketListener::Main() {
	int n = 0;
  static SDL_Event event;
  static SDL_UserEvent userevent;

	while (SocketListener::m_bContinue) {
		std::cout << "in thread" << std::endl;
		//SDL_Delay(800);
		n = ((Socket *) m_pDev.get())->Read();
		g_pErr->Debug("unblocked");		
		if (n <= 0) {
			g_pErr->Debug("ERROR reading from socket");
			SocketListener::m_bContinue = false;
		} else {
			/*
			string strMsg = ((Socket *) m_pDev.get())->PopMessage();
			g_pErr->Debug(pastestr::paste("ss", " ", ">>> just read: ", 
																		strMsg.c_str()));
			g_pErr->Debug("just read");
			*/
			userevent.type = SDL_USEREVENT;
			userevent.code = SBX_WATCH_SOCKET_MSG;
			userevent.data1 = (Socket *) m_pDev.get();
			userevent.data2 = NULL;
			event.type = SDL_USEREVENT;
			event.user = userevent;
			SDL_PushEvent(&event);
		}
	}

	return n;
}
