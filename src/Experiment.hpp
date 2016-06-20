// Experiment.h: interface for the Experiment class.
//
//////////////////////////////////////////////////////////////////////

#ifndef EXP_IDLE
#define EXP_IDLE 0
#define EXP_IN_PROGRESS 1
#define EXP_COMPLETED 2
#define EXP_ABORTED 3
#define EXP_CALIBRATE 4
#define EXP_TRIAL_READY 5

#include "Template.hpp"
#include "ItemCell.hpp"
#include "Trial.hpp"
#include "Stimulus.hpp"
#include "SocketListener.hpp"
#include "Webcam2.hpp"

class listordinfo {
public:
  long m_idItemCell;
  long m_idBlock;
  long m_idPhase;
  long m_ord;
  listordinfo(long idItemCell=0L, long idBlock=0L, long idPhase=0L, long ord=0L);
  inline void assign(long idItemCell, long idBlock, long idPhase, long ord) {
    m_idItemCell = idItemCell; m_idBlock = idBlock; m_idPhase = idPhase; m_ord = ord;
  }
  inline bool operator==(const listordinfo& rhs) const {
    return (m_idItemCell == rhs.m_idItemCell) && (m_idBlock == rhs.m_idBlock) && (m_idPhase == rhs.m_idPhase) && (m_ord == rhs.m_ord);
  }
};

/*
class trialtodo {
public:
  long m_icid;
  long m_ord;
  trialtodo(long icid=0L, long ord=0L);
  inline bool operator<(const trialtodo& rhs) const {
    return m_ord < rhs.m_ord;
  }
  inline bool operator==(const trialtodo& rhs) const {
    return (m_icid == rhs.m_icid) && (m_ord == rhs.m_ord);
  }
  inline bool operator!=(const trialtodo& rhs) const {
    return (m_icid != rhs.m_icid) || (m_ord != rhs.m_ord);
  }
  inline void assign(long icid, long ord) {
    m_icid = icid; m_ord = ord;
  }
};
*/

//#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/smart_ptr.hpp>
typedef boost::shared_ptr<Trial> TrialPtr;

#include <string>
using std::string;

#include <vector>
using std::vector;

#include <map>
using std::map;
using std::pair;

#include <set>
using std::set;

typedef map<long, TemplatePtr> TemplateMap;
typedef map<long, ItemCellPtr> ItemCellMap;
typedef map<long, TrialPtr> TrialMap;
typedef map<string, long> CounterMap;

class Experiment
{
 protected:
  bool m_bRefreshNeeded;
  long m_id;
  long m_idSession;
  long m_idList;
  //set<trialtodo> m_setTrialsTodo;
  long m_idSubj; // to do: allow for multiple subjects per session
  int m_nTemplates;
  int m_nItems;
  int m_nTrials;
  TemplateMap m_mapTemplate;
  ItemCellMap m_mapItemCell;
  TrialMap m_mapTrial;
  CounterMap m_mapCounter;
  TrialMap::iterator m_itTrial;
  Trial * m_pCurTrial;
  string m_strDriver;
  int m_nTrialsCompleted;

  vector<long> Sequence(const char * q1, const char * q2);
  string PhaseQuery(long, int);
  string ItemQuery(long, int);

public:
  static unsigned int g_nSeed;
  Uint32 m_msExpBegin;
  bool m_bResume;

  //Experiment(int argc, char * argv[]);
  //Experiment(const char * expdb, const char * dbType, const char * pcMode = "auto");
  Experiment();
  virtual ~Experiment();
  int InitializeDB(const char * expdb, const char * dbType);
  int InitializeExp(const char * pcMode, bool bResume = false);
  void InitReport();

  static StimulusMap s_mapStimulus;
  static DeviceMMap s_mmapDevice;
	static SocketListener * s_pSockListener;
	static Webcam2 * s_pCam;

  static Device * FindOrCreateDevice(long, long);
  static Stimulus * FindOrCreateStimulus(long id, 
					 Template * pTemplate);
  static int SetDisplay(SDL_Surface * pSurface);

  int StartSession();
  int LoadDevices();
	int LoadConfiguration(unsigned long eid = 0);
  int LoadTemplates();
  int LoadItems();
  vector<listordinfo> SequenceItems();
  int Prepare(SDL_Surface * pDisplay = NULL, bool bFullScreen = 0);
  Trial * NextTrial();
  inline Trial * GetCurrentTrial() {return m_pCurTrial;}
  int Cleanup();
  int Run();
  long CurrentItemID();
  long GetSessionID();

  int PrepareNextTrial();
  int StartTrial();
  int Update();
  int PollEvent();
  int FinishTrial();
  int UpdateSessionStatus(const char * pc);

  long AutoselectList();
  int StoreTrialData();
  long LoadOrCreateSubjects();
  int DeleteSession(long idSession);
  inline long SessionID() { return m_idSession; }
  int Message(const char * pcMessage);
  int WaitKey();
  inline long CurrentTrialID() { return m_pCurTrial->ID(); };
  string GetResponseDir();
  void CreateOrPurgeResponseDir(bool bResume = false);
  void CreateTrialObjects(vector<listordinfo> vlItems, int nBegin=1);
  void FillTrialsTodo(vector<listordinfo> vlItems);
  void PurgeTrialsTodo();
  vector<listordinfo> LoadTrialsTodo();
  string GetFileName();
  void RepeatExperiment();
  void IncrementCounter(const char * pcCtr);
  void ResetCounter(const char * pcCtr);
  long GetCounter(const char * pcCtr);
  Uint32 GetMSElapsed();
	Uint32 GetTrialBegin();
  int Loop();
};

#endif
