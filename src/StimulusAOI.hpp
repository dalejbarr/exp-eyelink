#ifndef EXP_STIMULUSAOI_INCLUDED
#define EXP_STIMULUSAOI_INCLUDED

#include <string>
using std::string;

#include <set>
using std::set;

#include <boost/smart_ptr.hpp>

#include "Stimulus.hpp"

class Template;

class StimulusAOI : public Stimulus {
protected:
  int m_x1, m_y1, m_x2, m_y2;

public:
  StimulusAOI();
  StimulusAOI(long id, Template * pTemplate, long idCmd, ArgMMap mmArgs);
  ~StimulusAOI();
  int Cleanup(void);
};

typedef boost::shared_ptr<StimulusAOI> StimulusAOIPtr;
typedef set<StimulusAOIPtr> StimAOISet;

#endif
