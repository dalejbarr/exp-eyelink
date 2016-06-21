#include "WatchKey.hpp"
#include "global.hpp"
#include "pastestr.hpp"

WatchKey::WatchKey(long id, long idNext, ArgMMap mmap) : Watch(id, idNext) {
  g_pErr->DFI("WatchKey::WatchKey", id, 4);
  ArgIter ii;

  pair<ArgIter, ArgIter> pii;

  pii = mmap.equal_range("Key_Code");
  if (pii.first != pii.second) {
    ii = pii.first;
		string str((*ii).second.c_str());
    from_string<int>(m_nKey, str, std::dec);
    g_pErr->Debug(pastestr::paste("ssd", " ", "watching for", (*ii).second.c_str(), ((long) m_nKey)));
  } else {
		pii = mmap.equal_range("Key");
		if (pii.first == pii.second) {
			g_pErr->Debug("watching for ANYKEY");
			m_nKey = -1;
		} else {
			ii = pii.first;
			m_nKey = atoi((*ii).second.c_str());
			g_pErr->Debug(pastestr::paste("ssd", " ", "watching for", (*ii).second.c_str(), ((long) m_nKey)));
		}
	}

  g_pErr->DFO("WatchKey::WatchKey", m_id, 4);
}
