#ifndef SBX_INCLUDED_HTTPRESTAPI
#define SBX_INCLUDED_HTTPRESTAPI

#include <string>
#include <curl/curl.h>

using std::string;

class HttpRestAPI {
protected:
  static bool s_bInitialized;
  CURL * m_pHandle;
  string m_strURL;
  bool m_bDataReceived;
  string m_strPayload;
public:
  HttpRestAPI();
  virtual ~HttpRestAPI();
  int SetURL(string sURL);
  string GetURL();
  string GetPayload();
  long HttpPOST(string msg, string endpoint = "");
  long HttpGET(string endpoint);
  string EscapeDoubleQuotes(string input);
  static size_t ReceiveDataCallback(void *buffer,
				    size_t sz,
				    size_t n,
				    void *f);
  size_t ReceiveData(void *buffer, size_t sz, size_t n);
};

#endif
