#ifndef SBX_PUPILLABSTRACKER_INCLUDED
#define SBX_PUPILLABSTRACKER_INCLUDED

#include <string>
#include "HttpRestAPI.hpp"
//#include <json.hpp>

using std::string;
//using json = nlohmann::json;

class PupilLabsTracker : public HttpRestAPI {
protected:
  bool m_bIsRecording;
public:
  PupilLabsTracker(string sURL);
  virtual ~PupilLabsTracker();
  bool IsRecording();
  bool IsReachable();
  bool StopAndSaveRecording();
  bool StartRecording();
  long Status(string& pStatus);
  bool CheckDBTable();
  long SendEvent(string strMessage);
};

#endif
