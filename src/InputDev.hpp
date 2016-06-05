#include "global.hpp"
#include "RespData.hpp"

#ifndef SBX_INPUTDEV_INCLUDED
#define SBX_INPUTDEV_INCLUDED

#define SBX_MOUSE_DEV 1
#define SBX_GAMEPAD_DEV 2
#define SBX_AUDIOREC_DEV 3
#define SBX_SCROLLTRACKGP_DEV 4
#define SBX_SOCKET_DEV 5

class ItemCell;

class InputDev {
protected:
  bool m_bIsRecording;
  unsigned long m_idDev;
  int m_nIndex;
  string m_sOutputFile;
  bool m_bRecMode;
  RespData m_respData;   // for storing recorded response information
public:
  static ItemCell * g_pCurItemCell;
  InputDev(unsigned long idDev, int nIndex = 0);
  virtual ~InputDev();
  inline int GetIndex() { return m_nIndex; };
  inline unsigned long GetDevID() { return m_idDev; };
  inline void SetRecMode(bool bMode) {m_bRecMode = bMode;};
  inline bool RecMode() {return m_bRecMode;};
  inline RespData GetRespData() {return m_respData;};
  virtual void Prepare();
  virtual void Start();
  virtual void Stop();
  virtual void HandleEvent(SDL_Event * pEvt);
  virtual void SetOutputFile(const char * pFile);
  virtual void ParseFilename(const char * pFile);
};

typedef boost::shared_ptr<InputDev> InputDevPtr;
typedef std::multimap<unsigned long, InputDevPtr> InputDevPtrMMap;
typedef std::multimap<unsigned long, InputDevPtr>::iterator ipdmIterator;
//typedef std::pair<unsigned long, InputDevPtr> ipdmPair;

#endif
