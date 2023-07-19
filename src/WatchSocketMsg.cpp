#include "WatchSocketMsg.hpp"
#include "pastestr.hpp"
#include "Template.hpp"
#include <boost/lexical_cast.hpp>


WatchSocketMsg::WatchSocketMsg(long id, long idNext, ArgMMap mmap) : 
	Watch(id, idNext) {
  g_pErr->DFI("WatchSocketMsg::WatchSocketMsg", id, 4);

  pair<ArgIter, ArgIter> pii;
  m_strMsgTarget.assign("");

  m_bAny = true;
  pii = mmap.equal_range("Message");
  if (pii.first != pii.second) {
    m_strMsgTarget.assign((pii.first)->second);
    m_bAny = false;
  } else {}

  m_strMsgReceived = "";
  
  g_pErr->DFO("WatchSocketMsg::WatchSocketMsg", m_id, 4);
}

WatchSocketMsg::~WatchSocketMsg() {
}

bool WatchSocketMsg::CheckCondition(SDL_Event * pEvt) {
  // if we get this far, a message has been received; just have to check which one
  bool bResult = false;
  string strTarget = Stimulus::GetResourceString(m_strMsgTarget.c_str());

  Socket * pSock = (Socket *) pEvt->user.data1;
  
  if (m_bAny) {
    bResult = true;
    m_strMsgReceived.assign(pSock->PopMessage());
    g_pErr->Debug(pastestr::paste("ss", "", "got the message:",
				  m_strMsgReceived.c_str()));
  } else {
    string strMsg(pSock->PopMessage());
    if (strMsg == strTarget) {
      bResult = true;
    } else {
      g_pErr->Debug(pastestr::paste("sssss", "", 
				    "watching for: '", strTarget.c_str(), "' but got '",
				    strMsg.c_str(), "'"));
    }
  }

  return bResult;
}

string WatchSocketMsg::GetMsgReceived() {
  return m_strMsgReceived;
}

/*
bool WatchSocketMsg::CheckWhich(Uint8 nGamePadUsed) {
  bool bResult = false;

  int nIndex = m_pGamePad->GetIndex();

  if (nIndex == nGamePadUsed) {
    bResult = true;
  } else {}

  return bResult;
}
*/
