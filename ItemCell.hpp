#ifndef EXP_ITEMCELL_INCLUDED
#define EXP_ITEMCELL_INCLUDED

#include <map>
using std::map;
using std::multimap;

#include <boost/smart_ptr.hpp>
#include "Template.hpp"
#include "Stimulus.hpp"

typedef map<string, string> ResourceMap;

class Trial;

class ItemCell {
 protected:
  long m_id;
  //StimulusAOIMap m_mapAOI;
  Trial * m_pCurTrial;

 public:
  TemplatePtr m_pTemplate;
  StimulusPtrMMap m_mmapAOI;
  ResourceMap m_mapResources;
  vector<EventTimePtr> m_vStateTiming;
  vector<EventTimePtr> m_vEventTiming;
  vector<RespData> m_vRespData;
  // If I knew c++ properly, I wouldnt need these lines:
  //  vector<GSC1SoundButton> m_vGSC1SoundButton;
  //vector<GSC1Toggle> m_vGSC1Toggle;
  //vector<GSC1Cursor> m_vGSC1Cursor;
  //vector<GSC1Locs> m_vGSC1Stars;
  //vector<GSC1Locs> m_vGSC1Flags;
  //vector<RespObj> m_vRespData;

  MsgMap m_msgMap;

  ItemCell(long id, TemplatePtr ptt);// : m_id(id), m_pTemplate(ptt) {};
  virtual ~ItemCell();

  inline void SetCurTrial(Trial * pTrial) { m_pCurTrial = pTrial; };

  long GetID() { return m_id; };
  int Load();
  int LoadAOIs();
  int LoadResources();
  int Prepare();
  int Run();
  int Finish();
  int HandleEvent(SDL_Event * pEvt);
  int Start();
  int Update();
  int StoreData(long idTrial);
  string GetFilenameFromResource(string s1, bool prepend = false);
  inline Trial * GetCurTrial() {return m_pCurTrial;};
  inline Template * GetTemplate() {return m_pTemplate.get();}
};

typedef boost::shared_ptr<ItemCell> ItemCellPtr;

#endif
