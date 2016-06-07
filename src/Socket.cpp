#include "EXPConfig.hpp"
#include "Socket.hpp"
#include "pastestr.hpp"
#include <boost/lexical_cast.hpp>

#include <netinet/in.h>
#include <netdb.h>
#include <boost/algorithm/string.hpp>

// #include "pastestr.hpp"

// extern EXPConfig * g_pConfig = NULL;

Socket::Socket(unsigned long ulDev, int nIndex) : InputDev(ulDev, nIndex) {
	g_pErr->Debug("¬¬ INITIALIZING Socket ¬¬");
	Initialize();
}

Socket::~Socket() {
	g_pErr->Debug("¬¬ DESTROYING Socket ¬¬");
	Destroy();
}

// when local computer is SOCKET_SERVER
int Socket::StartServer(string strPort) {
	std::size_t space;
	socklen_t clilen;
	struct sockaddr_in cli_addr, serv_addr;

	space = strPort.find(" ");
	if ((space == string::npos)) {
		g_pErr->Report("SOCKET_SERVER specified wrong in EXPConfig (should be 'ID port')");
	}

	m_id = boost::lexical_cast<long>(string(strPort, 0, space));
	m_strAddr.assign("");

	string str(strPort, space + 1, string::npos);
	g_pErr->Debug(pastestr::paste("sss", "", "'", str.c_str(), "'"));
	m_usPort = boost::lexical_cast<unsigned short>(str.c_str());

	g_pErr->Debug(pastestr::paste("sdsds", "", "socket '", (long) m_id,
																"' starting server on port '", 
																(long) m_usPort, "'"));

	m_oldsockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_oldsockfd < 0) 
		g_pErr->Report("ERROR opening socket");
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(m_usPort);
	if (bind(m_oldsockfd, (struct sockaddr *) &serv_addr,
					 sizeof(serv_addr)) < 0) 
		g_pErr->Report("ERROR on binding");
	listen(m_oldsockfd, 5);
	clilen = sizeof(cli_addr);

	std::cout << "- - - WAITING FOR CONNECTION FROM CLIENT - - -" << std::endl;
	m_sockfd = accept(m_oldsockfd, 
											 (struct sockaddr *) &cli_addr, 
											 &clilen);
	std::cout << "-- connection received! --" << std::endl;

	if (m_sockfd < 0) 
		g_pErr->Report("ERROR on accept");

	return(0);
}

// when local computer is SOCKET_CLIENT
int Socket::ConnectToServer(string strAddress) {
	struct sockaddr_in serv_addr;
	struct hostent *server;

	// first parse all the parameters
	std::size_t space;
	std::size_t colon;

	space = strAddress.find(" ");
	colon = strAddress.find(":");

	if ((space == string::npos) || (colon == string::npos)) {
		g_pErr->Report("SOCKET_CLIENT specified wrong in EXPConfig (should be 'ID addr:port)'");
	}

	m_id = boost::lexical_cast<long>(string(strAddress, 0, space));
	m_strAddr.assign(strAddress, space + 1, colon - 2);
	string str(strAddress, colon + 1, string::npos);
	g_pErr->Debug(pastestr::paste("sss", "", "'", str.c_str(), "'"));
	m_usPort = boost::lexical_cast<unsigned short>(str.c_str());

	g_pErr->Debug(pastestr::paste("sdsssds", "", "socket '", (long) m_id,
																"' connecting to '", m_strAddr.c_str(),
																"' on port '", (long) m_usPort, "'"));

	// now, connect to the socket
	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockfd < 0) 
		g_pErr->Report("ERROR opening socket");
	server = gethostbyname(m_strAddr.c_str());
	if (server == NULL) {
		g_pErr->Report("ERROR, no such host");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char *)server->h_addr, 
				(char *)&serv_addr.sin_addr.s_addr,
				server->h_length);
	serv_addr.sin_port = htons(m_usPort);
	if (connect(m_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
		g_pErr->Report("ERROR connecting");
	
	return(0);
}

void Socket::Initialize() {
	//unsigned int channels, fs, bufferFrames, device = 0, offset = 0;
	g_pErr->DFI("Socket::Initialize", (long) 0, 1);
	m_strAddr.assign("");
	m_usPort = 0;
	m_oldsockfd = -1;
	m_sockfd = -1;

	string strSock;
	// TODO: change things so that you can have multiple sockets
	if (g_pConfig->GetConfig("SOCKET_CLIENT", &strSock)) {
		// we are CLIENT: read the ID, address, and open the connection
		//g_pErr->Debug(pastestr::paste("ss", " ", "connecting to server", 
    //																	strSock.c_str()));
		ConnectToServer(strSock);
	} else {
		if (g_pConfig->GetConfig("SOCKET_SERVER", &strSock)) {
			// we are SERVER: start running on given port
			StartServer(strSock);
		} else {
			g_pErr->Report("no SOCKET_CLIENT or SOCKET_SERVER configuration message found in EXPConfig");
		}
	}
	g_pErr->DFO("Socket::Initialize", (long) 0, 1);
}

void Socket::Destroy() {
	g_pErr->DFI("Socket::Destroy", (long) 0, 1);
	if (m_sockfd >= 0) {
		close(m_sockfd);
	}
	if (m_oldsockfd >= 0) {
		close(m_oldsockfd);
	}

	g_pErr->DFO("Socket::Destroy", (long) 0, 1);
}

int Socket::SendMessage(string strMsg) {
	g_pErr->DFI("Socket::SendMessage", (long) 0, 1);

	//g_pErr->Debug(pastestr::paste("ssd", " ", "MSG:", strMsg.c_str(),
	// (long) strMsg.size()));
	int n = write(m_sockfd, strMsg.c_str(), strMsg.size());
	if (n < 0) 
		g_pErr->Report("error writing to socket");

	g_pErr->DFO("Socket::SendMessage", (long) 0, 1);

	return(0);
}

int Socket::Read() {
	char buffer[256];
	int n = -1;
	// warning this function blocks until data is read
	// send SIGINT using sigaction() to unblock
	if (m_sockfd < 0) {
		g_pErr->Report("unable to listen on socket :(");
	}
	bzero(buffer, 256);
	n = read(m_sockfd, buffer, 255);	
	if (n > 0) {
		string strMsg(buffer);
		boost::trim(strMsg);
		m_qMessages.push(strMsg);
	} else {
		g_pErr->Debug(pastestr::paste("sd", " ", "error in socket read",
																	(long) n));
	}

	return n;
}

string Socket::PopMessage() {
	string strMessage = m_qMessages.front();
	m_qMessages.pop();

	return strMessage;
}
