#ifndef EXP_STIMULUS_INCLUDED
#define EXP_STIMULUS_INCLUDED
#define EXP_STIMULUS_AOI 1
#define EXP_STIMULUS_BITMAP 2

#include <string>
using std::string;

#include <map>
using std::map;
using std::multimap;
using std::pair;

#include <boost/smart_ptr.hpp>

#include "Device.hpp"
#include "global.hpp"
#include "Operation.hpp"

class Template;
class ItemCell;

class Stimulus {
protected:
  Device * m_pDev;
  long m_lgDevType;
  long m_lgDevInstance;
  long m_id;
  Template * m_pTemplate;
  string m_sResource;
  string m_sFileName;

public:
  Stimulus(long id, Template * pTemplate);
  virtual ~Stimulus();

  static ItemCell * g_pCurItemCell;

  long GetDeviceType() { return m_lgDevType; }
  long GetDeviceInstanceID() {return m_lgDevInstance;}
  virtual int Prepare();
  virtual int Action();
  virtual int Finish() { return 0; }
  virtual int Cleanup() { return 0; }
  static string GetResourceString(string s1);
  string GetFilenameFromResource(string s1);
  inline int GetID() { return m_id; }
  inline string GetResource() { return m_sResource; }
  //virtual Operation * GetAttr(string s) { return NULL; }
};

typedef boost::shared_ptr<Stimulus> StimulusPtr;
typedef map<string, StimulusPtr> StimulusMap;
typedef multimap<string, StimulusPtr> StimulusPtrMMap;  // stimuli sorted by name
typedef multimap<int, StimulusPtr> StimulusPtrMMap2;  // stimulus sorted by layer
typedef pair<string, StimulusPtr> StimulusPair;
typedef pair<int, StimulusPtr> StimulusPair2;
typedef multimap<string, StimulusPtr>::iterator StimulusPtrMMapIter;
typedef multimap<int, StimulusPtr>::iterator StimulusPtrMMap2Iter;

//typedef multimap<int, AOIPtr> pAOIMMap2; // all AOIs, sorted by layer
//typedef boost::shared_ptr<StimulusBmp> AOIPtr;
//typedef pair<string, AOIPtr> AOIPair;
//typedef pair<int, AOIPtr> AOIPair2;

//typedef multimap<string, AOIPtr> pAOIMMap; // all AOIs, sorted by name
//typedef multimap<string, AOIPtr>::iterator pAOIMMapIter;


#endif
