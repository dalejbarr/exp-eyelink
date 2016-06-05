#ifndef SOCKET_INCLUDED
#define SOCKET_INCLUDED

#include "InputDev.hpp"
#include <queue>

class Socket : public InputDev {
protected:
	string m_strAddr;
	Uint32 m_id;
	unsigned short m_usPort;
	int m_sockfd;
	int m_newsockfd;
	std::queue<string> m_qMessages;
public:
  Socket(unsigned long idDev, int nIndex);
  virtual ~Socket();
	virtual void Initialize();
	virtual void Destroy();
	int ConnectToServer(string strAddress);
	int StartServer(string strPort);
	int SendMessage(string strMsg);
	int Read();
	string PopMessage();
  //virtual void Prepare();
  //virtual void Start();
  //virtual void Stop();
};

#endif
