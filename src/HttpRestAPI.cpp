#include "HttpRestAPI.hpp"
#include "ReportError.hpp"

extern ReportError * g_pErr;
bool HttpRestAPI::s_bInitialized = false;

HttpRestAPI::HttpRestAPI() {
  if (!HttpRestAPI::s_bInitialized) {
    g_pErr->Debug("starting HTTP Rest API");
    curl_global_init(CURL_GLOBAL_ALL);
    HttpRestAPI::s_bInitialized = true;
  }

  m_strURL = "";
  m_bDataReceived = false;
  
  m_pHandle = curl_easy_init();
  if (m_pHandle) {
    curl_easy_setopt(m_pHandle, CURLOPT_WRITEFUNCTION,
		     HttpRestAPI::ReceiveDataCallback);
    curl_easy_setopt(m_pHandle, CURLOPT_WRITEDATA, this);
    g_pErr->Debug("created HTTP handle, initialized callback function");
  } else {
    g_pErr->Report("could not initialize libcurl handle");
  }
}

HttpRestAPI::~HttpRestAPI() {
  if (m_pHandle) {
    g_pErr->Debug("removed HTTP handle");
    curl_easy_cleanup(m_pHandle);
  }
  
  if (HttpRestAPI::s_bInitialized) {
    g_pErr->Debug("stopping HTTP Rest API");
    curl_global_cleanup();
    HttpRestAPI::s_bInitialized = false;
  }
}

int HttpRestAPI::SetURL(string sURL) {
  if (m_pHandle) {
    // TODO: make sure URL doesn't end with '/'
    m_strURL = "http://" + sURL + "/";
  }
  
  return 0;
}

long HttpRestAPI::HttpPOST(string msg, string endpoint /* = "" */) {
  string target = m_strURL + endpoint;
  long http_code = 0;

  string strOut = "HTTP handle points to: " + target;
  g_pErr->Debug(strOut.c_str());

  if (m_pHandle) {
    m_bDataReceived = false;
    m_strPayload = "";
    curl_easy_setopt(m_pHandle, CURLOPT_URL, target.c_str());
    curl_easy_setopt(m_pHandle, CURLOPT_POST, 1L);
    curl_easy_setopt(m_pHandle, CURLOPT_POSTFIELDS, msg.c_str());
    curl_easy_perform(m_pHandle);
    curl_easy_getinfo(m_pHandle, CURLINFO_RESPONSE_CODE, &http_code);
  }

  return http_code;
}

string HttpRestAPI::GetURL() {
  return m_strURL;
}

long HttpRestAPI::HttpGET(string endpoint) {
  string target = m_strURL + endpoint;
  long http_code = 0;
  
  if (m_pHandle) {
    m_bDataReceived = false;
    m_strPayload = "";
    curl_easy_setopt(m_pHandle, CURLOPT_URL, target.c_str());
    curl_easy_setopt(m_pHandle, CURLOPT_HTTPGET, 1L);
    curl_easy_perform(m_pHandle);
    curl_easy_getinfo(m_pHandle, CURLINFO_RESPONSE_CODE, &http_code);
  }

  return http_code;
}

size_t HttpRestAPI::ReceiveDataCallback(void *buffer, size_t sz,
					size_t n, void *f) {
  g_pErr->Debug("inside callback function...");
  return static_cast<HttpRestAPI*>(f)->ReceiveData(buffer, sz, n);
}

size_t HttpRestAPI::ReceiveData(void *buffer, size_t sz, size_t n) {
  g_pErr->Debug("inside member function...");

  m_bDataReceived = true;
  m_strPayload.append((char *) buffer, sz * n);
  //g_pErr->Debug(m_strPayload.c_str());
  
  return sz * n;
}

string HttpRestAPI::GetPayload() {
  return m_strPayload;
}

string HttpRestAPI::EscapeDoubleQuotes(string input) {
  string s = "";
  
  for (char c : input) {
    if (c == '"') {
      s += "\\\"";
    } else {
      s += c;
    }
  }

  return s;
}
