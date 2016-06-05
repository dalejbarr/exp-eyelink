#include "WatchSocketMsg.hpp"
#include "pastestr.hpp"
#include "Template.hpp"
#include <boost/lexical_cast.hpp>


WatchSocketMsg::WatchSocketMsg(long id, long idNext, ArgMMap mmap) : 
	Watch(id, idNext) {
  g_pErr->DFI("WatchSocketMsg::WatchSocketMsg", id, 4);

	pair<ArgIter, ArgIter> pii;
	m_strMsg.assign("");

	m_bAny = true;
  pii = mmap.equal_range("Message");
  if (pii.first != pii.second) {
		m_strMsg.assign((pii.first)->second);
		m_bAny = false;
	} else {}

  g_pErr->DFO("WatchSocketMsg::WatchSocketMsg", m_id, 4);
}

WatchSocketMsg::~WatchSocketMsg() {
}

bool WatchSocketMsg::CheckCondition(SDL_Event * pEvt) {
  // if we get this far, a message has been received; just have to check which one
  bool bResult = false;
	string strTarget = Stimulus::GetResourceString(m_strMsg.c_str());

	if (m_bAny) {
		bResult = true;
	} else {
		Socket * pSock = (Socket *) pEvt->user.data1;
		string strMsg(pSock->PopMessage());
		if (strMsg == strTarget) {
			bResult = true;
		} else {
			g_pErr->Debug(pastestr::paste("sssss", "", 
																		"tgt: '", strTarget.c_str(), "' versus '",
																		strMsg.c_str(), "'"));
		}
	}
	/*
  if ( CheckWhich(pEvt->jbutton.which) ) {

    if (m_vButton.size() == 0) {
      bResult = true;
    } else {
      bResult = TargetButton((*pEvt).jbutton.button);
    }

  } else {}
	*/

  return bResult;
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
