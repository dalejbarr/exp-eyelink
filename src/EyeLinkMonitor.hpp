#ifndef SBX_EYELINKMONITOR_INCLUDED
#define SBX_EYELINKMONITOR_INCLUDED

#define SBX_EYELINK_SAMPLE_INTERVAL_MS 20
#define SBX_EYELINK_SAMPLE_THRESH 5

#include <atomic>
#include "InputDev.hpp"
#include "CircularStack.hpp"

typedef std::pair<int, int> xy_coord;
typedef std::pair<xy_coord, xy_coord> bbox;
typedef std::pair<bbox, long> bbpair;

class EyeLinkMonitor : public InputDev {
protected:
  SDL_Thread * m_pThread;
  std::atomic<bool> m_abContinue;
  std::vector<bbpair> m_bbpairs;
public:
  EyeLinkMonitor(unsigned long idDev, int nIndex = 0);
  void RegisterAOI(int x1, int y1, int x2, int y2, long id);
  void ClearAOIs();
  bool IsPointInAOI(int x, int y, bbox bb);
  void StartMonitoring();
  void StopMonitoring();
  static long s_idLastWatch;
  int static main(void *pVoid);
  int Main();
  int Signal(long id);
  
  // overridden functions
  virtual ~EyeLinkMonitor();
  virtual void Prepare();
  virtual void Start();
  virtual void Stop();
};

#endif
