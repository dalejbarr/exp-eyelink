#include "PupilLabsTracker.hpp"
#include "Recordset.hpp"
#include "ReportError.hpp"
#include "Experiment.hpp"
#include "pastestr.hpp"
#include <chrono>
#include <thread>

extern ReportError * g_pErr;
extern Recordset * g_prsResp;
extern Experiment * g_pExperiment;

PupilLabsTracker::PupilLabsTracker(string sURL) :
  HttpRestAPI::HttpRestAPI()
{ 
  SetURL(sURL);
  
  if (!IsReachable()) {
    string s1 = "Could not reach PupilLabsTracker on '" +
      HttpRestAPI::GetURL() + "'";
    g_pErr->Report(s1.c_str());
  }
  
  StopAndSaveRecording(); // just in case
  m_bIsRecording = false;
  
  CheckDBTable();
}

PupilLabsTracker::~PupilLabsTracker() {
  StopAndSaveRecording();
}

long PupilLabsTracker::SendEvent(string strMessage) {
  string msg2;

  msg2 = pastestr::paste("sd", "_", strMessage.c_str(),
			 g_pExperiment->CurrentTrialID());

  string msg3 = "{\"name\" : \"" + msg2 + "\"}";
  g_pErr->Debug(msg3.c_str());
  
  long http_code = HttpPOST(msg3, "api/event");

  long id = g_pExperiment->GetSessionID();
  string pload = "'" + GetPayload() + "'";
  
  string strFields =
    pastestr::paste("sdds", ", ",
		    "NULL", g_pExperiment->GetSessionID(),
		    g_pExperiment->CurrentTrialID(),
		    pload.c_str());
		    
  g_prsResp->Insert("PupilLabsTracker", strFields.c_str());

  return http_code;
}

bool PupilLabsTracker::IsReachable() {
  string s1 = "";
  long result = Status(s1);
  bool bResult = false;

  if (result == 200) {
    bResult = true;
  }
  
  return bResult;
}

long PupilLabsTracker::Status(string& pStatus) {
  long result = HttpGET("api/status");

  pStatus.assign(GetPayload().c_str());
  g_pErr->Debug(pStatus.c_str());
  
  return result;
}

bool PupilLabsTracker::StartRecording() {
  // only start if we're not already recording

  if (!m_bIsRecording) {
    long http_code = HttpRestAPI::HttpPOST("", "api/recording:start");

    g_pErr->Debug("Warming up Pupil Labs Tracker (3 second wait)...");
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    g_pErr->Debug("Done.");

    if (http_code == 200) {
      m_bIsRecording = true;
      long id = g_pExperiment->GetSessionID();
      string pload = "'" + GetPayload() + "'";
  
      string strFields =
	pastestr::paste("sdss", ", ",
			"NULL", g_pExperiment->GetSessionID(),
			"NULL", pload.c_str());
  
      if (g_prsResp) {
	g_prsResp->Insert("PupilLabsTracker", strFields.c_str());
      } else {
	g_pErr->Report("'g_prsResp' was undefined");
      }      
    } else {
      g_pErr->Debug(pastestr::paste("sd", "", "http code: ", http_code));
      g_pErr->Debug(GetPayload().c_str());
      g_pErr->Report("Couldn't start recording on the PupilLabs Neon.");
    }

  }
  
  return m_bIsRecording;
}

bool PupilLabsTracker::StopAndSaveRecording() {
  long http_code = HttpRestAPI::HttpPOST("", "api/recording:stop_and_save");

  if (m_bIsRecording && (http_code == 200)) {
    m_bIsRecording = false;
    long id = g_pExperiment->GetSessionID();
    string pload = "'" + GetPayload() + "'";
  
    string strFields =
      pastestr::paste("sdss", ", ",
		      "NULL", g_pExperiment->GetSessionID(),
		      "NULL", pload.c_str());
      
    if (g_prsResp) {
      g_prsResp->Insert("PupilLabsTracker", strFields.c_str());
    } else {
      g_pErr->Report("'g_prsResp' was undefined");
    }
  }
  
  return (m_bIsRecording == false);
}

bool PupilLabsTracker::CheckDBTable() {
  // check whether PupilLabsTracker table exists
  // if not, create the table
  int result = 0;
  
  if (!g_prsResp) {
    g_pErr->Report("'g_prsResp' was not defined");
  }
  
  if (!g_prsResp->TableExists("PupilLabsTracker")) {
    g_pErr->Debug("'PupilLabsTracker' table missing from database... creating...");
    string s = "\n PLID INTEGER PRIMARY KEY AUTOINCREMENT,\n SessionID INTEGER,\n TrialOrder INTEGER,\n \n Data TEXT";
    
    result = g_prsResp->CreateTable("PupilLabsTracker", s.c_str());
  } else {
    // we need to delete all of the entries from the current session
    long id = g_pExperiment->GetSessionID();
    string qry = pastestr::paste("sd", "",
				 "DELETE FROM PupilLabsTracker WHERE SessionID=",
				 id);
    g_prsResp->Exec(qry.c_str());
  }

  return true;
}

bool PupilLabsTracker::IsRecording() {
  return m_bIsRecording;
}
