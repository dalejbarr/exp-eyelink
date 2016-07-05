#include <iostream>
#include <X11/Xlib.h>
using std::cout;
using std::endl;

#include <cstdlib>
using std::atol;

#include <string>
using std::string;

#include <sstream>
using std::ostringstream;

#include <fstream>
using std::ifstream;

#include <vector>
using std::vector;

#include <ctime>

#include <iomanip>

#include "Experiment.hpp"
#include "RecordsetSqlite.hpp"
#include "RecordsetMySQL.hpp"
#include "EXPConfig.hpp"

#include "pastestr.hpp"
using pastestr::paste;

#include "global.hpp"
#include "StimulusImg.hpp"

#include "Display_SDL.hpp"
#include "Audio_SDL.hpp"
#include "InputDev.hpp"
#include "Template.hpp"
#include "sys/stat.h"

using std::stringstream;

Display_SDL * g_pDisplay = NULL;
Audio_SDL * g_pAudio = NULL;

Recordset * g_prsStim = NULL;
Recordset * g_prsResp = NULL;
ReportError * g_pErr = NULL;
EXPConfig * g_pConfig = NULL;

StimulusMap Experiment::s_mapStimulus;
DeviceMMap Experiment::s_mmapDevice;

SocketListener * Experiment::s_pSockListener = NULL;
Webcam2 * Experiment::s_pCam = NULL;

unsigned int Experiment::g_nSeed = 0;

/*
trialtodo::trialtodo(long icid, long ord) {
  assign(icid,ord);
}
*/

listordinfo::listordinfo(long idItemCell, long idBlock, long idPhase, long ord) {
  assign(idItemCell, idBlock, idPhase, ord);
}

void DumpSequence(vector<long> vi);

void Experiment::InitReport() {
  if (g_pErr) {
    g_pErr->Report("attempt to reinit g_pErr");
  } else {
    g_pErr = new ReportError();
  }
}

Experiment::Experiment() {
  InitReport();
  g_pErr->DFI("Experiment::Experiment", 0L, 1);
  m_pCurTrial = NULL;
  m_bRefreshNeeded = false;
  g_pErr->DFO("Experiment::Experiment", 0L, 1);
}

int Experiment::InitializeDB(const char * expdb, const char *dbType) {
  g_pErr->DFI("Experiment::InitializeDB", 0L, 1);

  // create the connection to the database
  std::string fname;
  fname.assign(expdb);
  std::string fname2;

  std::string strDriver = "";
  m_strDriver.assign(dbType);

  if (m_strDriver == "sqlite") {
    // SQLite Version
    g_pErr->Debug("using sqlite3 version");

    // try to open it
    ifstream my_file(fname.c_str(), ifstream::in);
    if (!my_file.good()) { // try again with extension
      fname2 = fname + ".db";
      my_file.open(fname2.c_str(), ifstream::in);
      if (!my_file.good()) { // throw error
	g_pErr->Report(pastestr::paste("sssss", "", "couldn't open file '", fname.c_str(), "' or '",
				       fname2.c_str(), "'."));
      } else {
	fname = fname2;
      }
    } else {}

    // TODO check whether or not database exists

    g_prsStim = (Recordset *) new RecordsetSqlite(g_pErr, 
						  fname.c_str(), 
						  NULL, NULL, NULL, 0, 0);
  } else if (m_strDriver == "MySQL") {
    // MySQL version
    g_prsStim = (Recordset *) new RecordsetMySQL(g_pErr, 
						 fname.c_str(), 
						 "db1.psy.gla.ac.uk",
						 "talklab", "talkpass", 
						 3306, 0);
    //g_pErr->Debug("using mysql version");
  } else {
    g_pErr->Report(pastestr::paste("sss", "", "Database driver ", m_strDriver.c_str(),
				   " not recognized."));
  }

  if (!g_prsStim) {
    g_pErr->Report(pastestr::paste("ssss", "" , m_strDriver.c_str(), 
				   " connection to database ", 
				   fname.c_str(), " not established!!!!"));
  } else {}

  g_pErr->DFO("Experiment::InitializeDB", 0L, 1);
}


int Experiment::InitializeExp(const char * pcMode, bool bResume) {
  g_pErr->DFI("Experiment::InitializeExp", (const char *) NULL);

  g_pErr->Debug(pastestr::paste("ss", "", "mode is ", pcMode));

	if (XInitThreads() == 0) {
		g_pErr->Report("Couldn't initialize threads");
	}

  m_bRefreshNeeded = false;
  m_pCurTrial = NULL;
  m_id = 1;
  m_idSubj = 0;
  vector<listordinfo> vlItems;
  m_bResume = bResume;

  Template::g_pExperiment = this;

  Trial::g_pExp = this;

  // don't use separate connections for response and stimulus
  g_prsResp = g_prsStim;
 
  if (!strcmp(pcMode, "top")) {
    g_prsStim->ExecCount("SELECT MAX(SessionID) from Session WHERE Completion != 'COMPLETED'");
    if (g_prsStim->NumRows() > 0) {
      m_idSession = atol(g_prsStim->Get(0));
    } else {
      g_pErr->Report("No session info found in the database.");
    }
  } else if (!strcmp(pcMode, "auto")) {
    m_idSession = AutoselectList();
  } else {
    // search for a specific ID number
    m_idSession = atol(pcMode);
    if (!bResume) {
      DeleteSession(m_idSession);
    } else {}
  }

  // create path, get rid of any files in the response directory
  CreateOrPurgeResponseDir(bResume);

  if (StartSession()) { 
   g_pErr->Report(paste("sds", " ",
			 "SessionID", m_idSession, "not found in database."));
  } else {}

  if (!bResume) {
    Experiment::g_nSeed = (unsigned int) time(NULL);
    g_prsStim->Exec(pastestr::paste("susd", "",
				    "UPDATE Session SET Seed=", Experiment::g_nSeed, " WHERE SessionID=", GetSessionID()));
    g_pErr->Debug(pastestr::paste("su", "", "established seed of ", Experiment::g_nSeed));
  } else {
    g_prsStim->Exec(pastestr::paste("sd", "", "SELECT Seed FROM Session WHERE SessionID=", 
																		GetSessionID()));
    if (!g_prsStim->RowReady()) {
      g_pErr->Report("error in Experiment::InitializeExp");
    } else {}
    Experiment::g_nSeed = (unsigned int) atoi(g_prsStim->Get(0));
    g_pErr->Debug(pastestr::paste("su", "", "retrieved seed of ", Experiment::g_nSeed));
  }
  srand(Experiment::g_nSeed);

  UpdateSessionStatus("IN_PROGRESS");

	LoadConfiguration();

  LoadOrCreateSubjects();

  //LoadDevices();
  LoadTemplates();
  LoadItems();

  if (!bResume) {
    PurgeTrialsTodo();
    vlItems = SequenceItems();
    FillTrialsTodo(vlItems);
  } else {
    vlItems = LoadTrialsTodo();
  }

  CreateTrialObjects(vlItems);

  m_itTrial = m_mapTrial.begin();  // start at the beginning

  g_pErr->DFO("Experiment::InitializeExp", (const char *) NULL);
}

Experiment::~Experiment()
{
  g_pErr->DFI("Experiment::~Experiment", (const char *) NULL);
  Cleanup();
  g_pErr->DFO("Experiment::~Experiment", (const char *) NULL);

  if (g_pDisplay) {
    delete g_pDisplay;
    g_pDisplay = NULL;
  } else {}

  if (g_pAudio) {
    delete g_pAudio;
    g_pAudio = NULL;
  } else {}

  g_pErr->Debug(".Deleting ReportError.");
  if (g_pErr) {
    delete g_pErr;
    g_pErr = NULL;
  } else {}
}

int Experiment::Cleanup() {
	if (s_pSockListener) {
		delete s_pSockListener;
	}

  g_pErr->Debug(".Deleting Trials.");
  m_mapTrial.clear();

  g_pErr->Debug(".Deleting ItemCells.");
  m_mapItemCell.clear();

  g_pErr->Debug(".Deleting Templates.");
  m_mapTemplate.clear();

  g_pErr->Debug(".Deleting Stimuli.");
  s_mapStimulus.clear();

  g_pErr->Debug(".Deleting Devices.");
  s_mmapDevice.clear();

  g_pErr->Debug(".Deleting Recordsets.");
  if (g_prsStim == g_prsResp) {
    g_prsResp = NULL;
  } else {}

  if (g_prsStim) {
    delete g_prsStim;
    g_prsStim = NULL;
  } else {}

  if (g_prsResp) {
    delete g_prsResp;
    g_prsResp = NULL;
  } else {}

	if (g_pConfig) {
		delete g_pConfig;
	} else {}

  g_pErr->Debug(".Deleting Recordsets.");
  m_mapCounter.clear();
	if (s_pCam) {
		delete s_pCam;
		s_pCam = NULL;
	}

}

int Experiment::LoadTemplates() {

  g_pErr->DFI("Experiment::LoadTemplates", (const char *) NULL);

  std::string q3 = pastestr::paste("sds", " ", "\
SELECT Template.TemplateID, Template.Timeout, Template.Name \n\
FROM Template \n\
INNER JOIN Item ON Template.TemplateID=Item.TemplateID \n\
INNER JOIN ListOrder ON Item.ItemCellID=ListOrder.ItemCellID \n\
INNER JOIN Phase ON ListOrder.PhaseID=Phase.PhaseID \n\
INNER JOIN Block ON Phase.BlockID=Block.BlockID \n\
WHERE ListOrder.ListID=", m_idList, "\n\
GROUP BY Template.TemplateID, Template.Timeout, Template.Name");

  g_prsStim->ExecCount(q3);

  //allocate; will destroy in ~Experiment()
  m_nTemplates = (int) g_prsStim->NumRows();
  if (m_nTemplates > 0) {
    for (int i = 0; i < m_nTemplates; i++) {
      long id = atol(g_prsStim->Get(0));
      if (id == 0) {
	g_pErr->Report("Template 0 not a valid TemplateID.");
      } else {}
      m_mapTemplate[id] = TemplatePtr(new Template(atol(g_prsStim->Get(0)),
						   atol(g_prsStim->Get(1, "0")),
						   g_prsStim->Get(2)));
      g_prsStim->MoveNext();
    }
    g_pErr->Debug(paste("sds", "", "\n", m_nTemplates, 
			" templates retrieved\n"));
  
    map<long, TemplatePtr>::const_iterator it;
    for (it = m_mapTemplate.begin(); it != m_mapTemplate.end(); it++) {
      it->second->LoadAOIs();
      it->second->LoadStates();
    }
  } else {
    g_pErr->Debug("No Templates were loaded!");
  }

  g_pErr->DFO("Experiment::LoadTemplates", (const char *) NULL);
  return 0;
}


int Experiment::StartSession() {
  // get the ListID from the SessionID
  string q1 = paste("sd", "", "SELECT ListID FROM Session WHERE SessionID=", 
		    m_idSession);
  g_prsStim->Exec(q1);
  if (!g_prsStim->RowReady()) {
    return 1;
  }

  // initialize class members for SessionID / ListID
  m_idList = atol(g_prsStim->Get(0));
  g_pErr->Debug(paste("sdsd", " ", "ListID is", m_idList, "for SessionID", m_idSession));

  //srand(time(NULL));

  // change the status of the SessionID
  UpdateSessionStatus("IN PROGRESS");

  return 0;
}

int Experiment::LoadItems() {
  g_pErr->DFI("Experiment::LoadItems", (const char *) NULL);

  string q = paste("sds", "", "\
SELECT Item.ItemCellID, Item.TemplateID \n\
FROM Item \n\
INNER JOIN ListOrder ON Item.ItemCellID=ListOrder.ItemCellID \n\
INNER JOIN Phase ON ListOrder.PhaseID=Phase.PhaseID \n\
INNER JOIN Block ON Phase.BlockID=Block.BlockID \n\
WHERE ListID=", m_idList, "\n\
GROUP BY Item.ItemCellID, Item.TemplateID");

  g_prsStim->ExecCount(q);

  m_nItems = (int) g_prsStim->NumRows();
  ItemCell * pic = NULL;
  //m_vpItemCell = new ItemCell *[m_nItems];
  for (int i = 0; i < m_nItems; i++) {
    pic = new ItemCell(atol(g_prsStim->Get(0)),
		       m_mapTemplate[atol(g_prsStim->Get(1))]);
    m_mapItemCell[atol(g_prsStim->Get(0))] = ItemCellPtr(pic);
						    /*
    m_ppItemCell[i] = new ItemCell(atol(g_prsStim->Get(0)),
				   atol(g_prsStim->Get(1)),
				   m_ppTemplate, m_nTemplates);
						    */
    
    g_prsStim->MoveNext();
  }

  ItemCellMap::iterator ii;
  for (ii = m_mapItemCell.begin(); ii != m_mapItemCell.end(); ii++) {
    ii->second->Load();
  }

  g_pErr->DFO("Experiment::LoadItems", (const char *) NULL);
  return 0;
}

vector<long> Experiment::Sequence(const char * q1, const char * q2) {
  vector<long> vlID1;
  vector<long> vlID2;
  vector<int> vnPos;

  g_prsStim->Exec(q1);
  while (g_prsStim->RowReady()) {
    vlID1.push_back(atol(g_prsStim->Get(0)));
    vnPos.push_back(atoi(g_prsStim->Get(1)));
    g_prsStim->MoveNext();
  }

  g_prsStim->ExecCount(q2);
  while (g_prsStim->RowReady()) {
    vlID2.push_back(atol(g_prsStim->Get(0)));
    g_prsStim->MoveNext();
  }

  int n = vlID1.size() + vlID2.size();
  vector<long> vlID(n, 0);

  int m = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < vlID1.size(); j++) {
      if (vnPos[j] == (i+1)) {
	vlID[i] = vlID1[j];
	break;
      } else {
	if (vnPos[j] > (i+1)) {
	  break;
	} else {}
      }
    }

    if (vlID[i] == 0) {
      vlID[i] = vlID2[m++];
    } else {}
  }

  return vlID;
}

string Experiment::PhaseQuery(long bid, int n) {

  string q = paste("sdsd", " ", "\
SELECT Phase.PhaseID, Phase.OrderConstraint \n\
FROM Phase \n\
INNER JOIN ListOrder ON Phase.PhaseID=ListOrder.PhaseID \n\
WHERE (Phase.BlockID=", bid, ") AND (ListID=", m_idList);

  if (n == 0) {
    q += ") \
AND (Phase.OrderConstraint IS NOT NULL) \n\
GROUP BY Phase.OrderConstraint, Phase.PhaseID";
  } else {
    q += ") \
AND (Phase.OrderConstraint IS NULL) \n\
GROUP BY Phase.PhaseID \n\
ORDER BY ";
    q += g_prsStim->RandCmd();
  }

  return q;
}

string Experiment::ItemQuery(long pid, int n) {
  string q = "SELECT ItemCellID, OrderConstraint FROM ListOrder \n";
  string qw = paste("sdsds", "", "WHERE (PhaseID=", pid, ") AND (ListID=", m_idList, ") ");

  //if (g_prsStim->TableExists("TrialsTodo")) {
  //  q += "JOIN TrialsTodo USING (ItemCellID) \n";
  //  qw += paste("sds", "", "AND (SessionID=", m_idSession, ") ");
  //} else {}

  q += qw;
  
  if (n == 0) {
    q += "AND (OrderConstraint IS NOT NULL) \n\
ORDER BY OrderConstraint";
  } else {
    q += "AND (OrderConstraint IS NULL) \n\
ORDER BY ";
    q += g_prsStim->RandCmd();
  }

  return q;
}

void DumpSequence(vector<long> vi) {

  ostringstream ostr;

  for (int i = 0; i < vi.size(); i++) {
    ostr << vi[i] << ' ';
  }
  ostr << endl;

  g_pErr->Debug(ostr.str());

}

/*
int Experiment::SequenceItems() {

  g_pErr->DFI("Experiment::SequenceItems", (const char *) NULL);

  vector<long> vlBlocks;
  vector<long> vlPhases;
  vector<long> vlItems;

  string q1 = paste("sds", " ", "\
SELECT Block.BlockID, Block.OrderConstraint \n\
FROM Block \n\
INNER JOIN Phase ON Block.BlockID=Phase.BlockID \n\
INNER JOIN ListOrder ON Phase.PhaseID=ListOrder.PhaseID \n\
WHERE (ListID=", m_idList, ") AND (Block.OrderConstraint IS NOT NULL) \n\
GROUP BY Block.OrderConstraint, Block.BlockID");

  string q2 = paste("sdss", " ", "\
SELECT Block.BlockID \n\
FROM Block \n\
INNER JOIN Phase ON Block.BlockID=Phase.BlockID \n\
INNER JOIN ListOrder ON Phase.PhaseID=ListOrder.PhaseID \n\
WHERE (ListID=", m_idList, ") AND (Block.OrderConstraint IS NULL) \n\
GROUP BY Block.BlockID \n\
ORDER BY", g_prsStim->RandCmd());

  vlBlocks = Sequence(q1.c_str(), q2.c_str());
  if (vlBlocks.size() == 0) {
    g_pErr->Report(paste("sds", " ",
			"No blocks found for ListID", m_idList, "."));
  } else {}
  g_pErr->Debug("Block order is:");
  DumpSequence(vlBlocks);

  int m = 0;
  for (int i = 0; i < vlBlocks.size(); i++) {
    vlPhases = Sequence(PhaseQuery(vlBlocks[i], 0).c_str(),
			PhaseQuery(vlBlocks[i], 1).c_str());
    if (vlPhases.size() == 0) {
      g_pErr->Report(paste("sds", " ", 
			   "No phases found for BlockID", vlBlocks[i],
			   "."));
    } else {}
    g_pErr->Debug("Phase order is:");
    DumpSequence(vlPhases);

    for (int j = 0; j < vlPhases.size(); j++) {
      vlItems = Sequence(ItemQuery(vlPhases[j], 0).c_str(), 
			 ItemQuery(vlPhases[j], 1).c_str());
      if (vlItems.size() == 0) {
	g_pErr->Debug(paste("sds", " ",
			     "No items found for PhaseID", 
			     vlPhases[j], "."));
      } else {
	g_pErr->Debug("Item order is:");
	DumpSequence(vlItems);
      }

      for (int k = 0; k < vlItems.size(); k++, m++) {
	map<long, ItemCellPtr>::iterator it;
	it = m_mapItemCell.find(vlItems[k]);
	if (it == m_mapItemCell.end()) {
	  g_pErr->Report("Problem in SequenceItems()");
	} else {}
	m_mapTrial[(long) m+1] = TrialPtr(new Trial(m+1,vlBlocks[i],vlPhases[j],it->second));
      }
    }
  }

  m_itTrial = m_mapTrial.begin();

  g_pErr->Debug(paste("sds", "", "There are ", m_mapTrial.size(), 
		      " trials in all"));
  g_pErr->DFO("Experiment::SequenceItems", (const char *) NULL);

  return 0;
}
*/

vector<listordinfo> Experiment::SequenceItems() {

  g_pErr->DFI("Experiment::SequenceItems", (const char *) NULL);

  vector<long> vlBlocks;
  vector<long> vlPhases;
  vector<listordinfo> vlItems;
  vector<long> vl1;

  listordinfo loi;

  string q1 = paste("sds", " ", "\
SELECT Block.BlockID, Block.OrderConstraint \n\
FROM Block \n\
INNER JOIN Phase ON Block.BlockID=Phase.BlockID \n\
INNER JOIN ListOrder ON Phase.PhaseID=ListOrder.PhaseID \n\
WHERE (ListID=", m_idList, ") AND (Block.OrderConstraint IS NOT NULL) \n\
GROUP BY Block.OrderConstraint, Block.BlockID");

  string q2 = paste("sdss", " ", "\
SELECT Block.BlockID \n\
FROM Block \n\
INNER JOIN Phase ON Block.BlockID=Phase.BlockID \n\
INNER JOIN ListOrder ON Phase.PhaseID=ListOrder.PhaseID \n\
WHERE (ListID=", m_idList, ") AND (Block.OrderConstraint IS NULL) \n\
GROUP BY Block.BlockID \n\
ORDER BY", g_prsStim->RandCmd());

  vlBlocks = Sequence(q1.c_str(), q2.c_str());
  if (vlBlocks.size() == 0) {
    g_pErr->Report(paste("sds", " ",
			"No blocks found for ListID", m_idList, "."));
  } else {}
  g_pErr->Debug("Block order is:");
  DumpSequence(vlBlocks);

  int m = 1;
  for (int i = 0; i < vlBlocks.size(); i++) {
    vlPhases = Sequence(PhaseQuery(vlBlocks[i], 0).c_str(),
			PhaseQuery(vlBlocks[i], 1).c_str());
    if (vlPhases.size() == 0) {
      g_pErr->Report(paste("sds", " ", 
			   "No phases found for BlockID", vlBlocks[i],
			   "."));
    } else {}
    g_pErr->Debug("Phase order is:");
    DumpSequence(vlPhases);

    for (int j = 0; j < vlPhases.size(); j++) {
      vl1 = Sequence(ItemQuery(vlPhases[j], 0).c_str(), ItemQuery(vlPhases[j], 1).c_str());
      for (int k = 0; k < vl1.size(); k++) {
	loi.assign(vl1[k], vlBlocks[i], vlPhases[j], m++);
	vlItems.push_back(loi);
      }
      //vlItems.insert(vlItems.end(), vl1.begin(), vl1.end());

      if (vl1.size() == 0) {
	g_pErr->Debug(paste("sds", " ",
			     "No items found for PhaseID", 
			     vlPhases[j], "."));
      } else {
	g_pErr->Debug("Item order is:");
	DumpSequence(vl1);
      }

    }
  }

  /*
      for (int k = 0; k < vlItems.size(); k++, m++) {
	map<long, ItemCellPtr>::iterator it;
	it = m_mapItemCell.find(vlItems[k]);
	if (it == m_mapItemCell.end()) {
	  g_pErr->Report("Problem in SequenceItems()");
	} else {}
	m_mapTrial[(long) m+1] = TrialPtr(new Trial(m+1,vlBlocks[i],vlPhases[j],it->second));
      }

      m_itTrial = m_mapTrial.begin();

     g_pErr->Debug(paste("sds", "", "There are ", m_mapTrial.size(), 
		      " trials in all"));

   */

  g_pErr->DFO("Experiment::SequenceItems", (const char *) NULL);

  return vlItems;
}

Stimulus * Experiment::FindOrCreateStimulus(long id, Template * pTemplate) {
  Stimulus * pStim = NULL;
  StimulusPtr ptrStim;

  //if (pcFile[0] == '$') {
  //} else {
  /*
    StimulusMap::iterator it = Experiment::s_mapStimulus.find(pcFile);

    if (it == Experiment::s_mapStimulus.end()) {
      switch (id) {
      case EXP_STIMULUS_BITMAP :
	pStim = new StimulusImg();
	break;
      case EXP_STIMULUS_AOI :
	pStim = new StimulusAOI();
	break;
      }
      ptrStim = StimulusPtr(pStim);
      Experiment::s_mapStimulus[pcFile] = ptrStim;
    } else {
      ptrStim = it->second;
    }
    pStim = ptrStim.get();
    //}
    */
  return pStim;
}

Device * Experiment::FindOrCreateDevice(long idDev, long id) {
  g_pErr->DFI("Experiment::FindOrCreateDevice", 0L, 1);

  std::pair<DeviceMMap::iterator, DeviceMMap::iterator> pi;
  pi = Experiment::s_mmapDevice.equal_range(EXP_DEVICE_DISPLAY);

  Device * pDev = NULL;
  DevicePtr pDevPtr;

  if (pi.first != pi.second) {
    for (DeviceMMap::iterator ii = pi.first; ii != pi.second; ii++) {
      if (ii->second->GetID() == id) {
	pDevPtr = ii->second;
	pDev = pDevPtr.get();
	break;
      } else {}
    }
  } else {}

  if (pDev) {
    g_pErr->Debug(paste("sdsds", "", "Device (", idDev, ", ", id, ") found"));    
  } else {
    g_pErr->Debug(paste("sdsds", "", "Device (", idDev, ", ", id, ") not found; creating"));

    switch (idDev) {
    case EXP_DEVICE_DISPLAY :
      {
	pDev = new Device(id);
	pDevPtr = DevicePtr(pDev);
	s_mmapDevice.insert(DevPair(idDev, pDevPtr));
      }
      break;
    }
  }

  g_pErr->DFO("Experiment::FindOrCreateDevice", 0L, 1);
  return NULL;
}

Trial * Experiment::NextTrial() {
  Trial * pTrial = NULL;

  //g_pErr->Debug(pastestr::paste("ds", "", (long) m_mapTrial.size(), " trials"));
  if (m_itTrial != m_mapTrial.end()) {
    pTrial = (*m_itTrial).second.get();
    m_itTrial++;
    //g_pErr->Debug("here");
  } else {
    g_pErr->Debug("END OF TRIALS!");
  }

  return pTrial;
}

int Experiment::Prepare(SDL_Surface * pDisplay /* = null */, bool bFullScreen /* = 0 */) {
  g_pErr->DFI("Experiment::Prepare", 0L, 1);

  g_pDisplay = new Display_SDL(pDisplay);

  if (pDisplay == NULL) {
    g_pErr->Debug("display not initialized");

    //g_pErr->Debug("here");
    const SDL_VideoInfo * sdlVid = NULL;
    sdlVid = SDL_GetVideoInfo();
    g_pDisplay->m_nWidth = sdlVid->current_w;
    g_pDisplay->m_nHeight = sdlVid->current_h;
    g_pErr->Debug(pastestr::paste("sdd", " ", "Display size is", g_pDisplay->m_nWidth, g_pDisplay->m_nHeight));
    if (bFullScreen) {
      g_pDisplay->CreateScreen(0, 0, sdlVid->current_w, sdlVid->current_h, 
			       SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_FULLSCREEN );
    } else {
			string cstr;
			int scrwidth = 1024;
			int scrheight = 768;
			g_pConfig->GetConfigInt("Display_Width", &scrwidth);
			g_pConfig->GetConfigInt("Display_Height", &scrheight);
			g_pDisplay->CreateScreen(0, 0, scrwidth, scrheight, 
															 SDL_DOUBLEBUF | SDL_HWSURFACE);
    }
  } else {}
  g_pErr->Debug("display initialized");

  if ( TTF_Init() < 0 ) {
    g_pErr->Report(pastestr::paste("ss", "", "Couldn't initialize TTF: %s", SDL_GetError()));
    SDL_Quit();
    return(2);
  } else {}
  g_pErr->Debug("ttf initialized");

  if (g_pAudio == NULL) {
    g_pAudio = new Audio_SDL();
  } else {}
  g_pErr->Debug("audio initialized");

  m_pCurTrial = NULL;

  m_msExpBegin = ClockFn();

	int nShowCursor = 1;
	if (g_pConfig->GetConfigInt("Mouse_Cursor", &nShowCursor)) {
		SDL_ShowCursor(nShowCursor);
	}

  g_pErr->DFO("Experiment::Prepare", 0L, 1);

  return 0;
}

int Experiment::Run() {
  // this is the main experiment loop that catches and handles all events
  SDL_Event event;
  Trial * pTrial = NULL;
  char charinfo[1024];

  Prepare();

  pTrial = NextTrial();
  if (!pTrial) {
    g_pErr->Debug("no trials loaded!");
  } else {
    pTrial->Prepare();
    pTrial->Run();
  }

  while (pTrial) {
    SDL_WaitEvent(&event);
    switch (event.type) {
    case SDL_QUIT :
      if (pTrial) {
				pTrial->Finish();
      } else {}
      pTrial = NULL;
      break;
    case SDL_USEREVENT :
      switch (event.user.code) {
      case SBX_FINISHED :
				g_pErr->Debug("trial completed");
				pTrial->Finish();
				pTrial = NextTrial();
				if (pTrial) {
					//cout << pTrial->m_pItemCell->m_pTemplate.m_mmapAOI.size();
					pTrial->Prepare();
					pTrial->Run();
				} else {
					g_pErr->Debug("EXPERIMENT COMPLETED");
				}
				break;
      }
    case SDL_KEYDOWN :
      {
				g_pErr->Debug("here1");
				sprintf(charinfo, "key %s (%d) pressed", SDL_GetKeyName(event.key.keysym.sym), 
								event.key.keysym.sym);
				g_pErr->Debug(charinfo);
      }
      break;
    default :
      if (pTrial) {
				pTrial->HandleEvent(&event);      
      } else {}
    }
  }

  return 0;
}

int Experiment::SetDisplay(SDL_Surface * pSurface) {
  g_pDisplay = new Display_SDL(pSurface);
  return 0;
}

int Experiment::PrepareNextTrial() {
  g_pErr->DFI("Experiment::PrepareNextTrial", 0L, 1);
  int nResult = EXP_TRIAL_READY;
  //trialtodo tt;
  
  /*
  if (m_bRefreshNeeded) {
    g_pErr->Report("############## refresh needed!!");
  } else {}
  */

  //do {
  m_pCurTrial = NextTrial();
  //if (!m_pCurTrial) {
  //break;
  //} else {}
  //tt.assign(m_pCurTrial->m_pItemCell->GetID(), m_pCurTrial->ID());
  //} while (!m_setTrialsTodo.count(tt));

  if (!m_pCurTrial) {
    nResult = EXP_COMPLETED;
  } else {
    m_pCurTrial->Prepare();
  }

  g_pErr->DFO("Experiment::PrepareNextTrial", 0L, 1);
  return nResult;
}

int Experiment::StartTrial() {
  g_pErr->DFI("Experiment::StartNextTrial", 0L, 1);
  if (!m_pCurTrial) {
    g_pErr->Report("trial not ready!");
  } else {}

  m_pCurTrial->Start();

  g_pErr->DFO("Experiment::StartNextTrial", 0L, 1);
  return 0;
}

int Experiment::Update() {
  static int nResultPoll = SBX_NO_EVENT;
  static int nResult = SBX_CONTINUE;

  while( (nResultPoll = PollEvent()) == SBX_EVENT ) ;

  switch(nResultPoll) {
  case SBX_NO_EVENT :
    nResult = SBX_CONTINUE;
    if (m_pCurTrial) {
      m_pCurTrial->Update();
    } else {}
    break;
  case SBX_FINISHED :
    nResult = SBX_FINISHED;
    break;
  case SBX_ABORT_EXPERIMENT :
    nResult = SBX_END_EXPERIMENT;
    break;
  default :
    g_pErr->Report("undefined poll code in Experiment::Update");
    break;
  }

  return nResult;
}

int Experiment::PollEvent() {
  static SDL_Event event;
  static int nResult = SBX_NO_EVENT;
  char charinfo[1024];
  SDL_QuitEvent eq;

  if (SDL_PollEvent(&event)==1) {
    nResult = SBX_EVENT;
    switch (event.type) {
    case SDL_QUIT :
      if (m_pCurTrial) {
				m_pCurTrial->Finish();
      } else {}
      m_pCurTrial = NULL;
      nResult = SBX_ABORT_EXPERIMENT;
      UpdateSessionStatus("ABORTED");
      break;
    case SDL_USEREVENT :
      switch (event.user.code) {
      case SBX_FINISHED :
				g_pErr->Debug("trial completed");
				nResult = SBX_FINISHED;
				break;
      }
      if (m_pCurTrial) {
				m_pCurTrial->HandleEvent(&event);      
      } else {}
      break;
    case SDL_KEYDOWN :
      {
	//sprintf(charinfo, "key %s (%d) pressed", SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.sym);
	if ((event.key.keysym.mod & KMOD_CTRL) && (event.key.keysym.sym==99)) {
	  g_pErr->Debug("break pressed");
	  eq.type = SDL_QUIT;
	  SDL_PushEvent((SDL_Event *) &eq);
	  //SDL_Quit();
	} else {
	  //g_pErr->Debug(charinfo);
	  sprintf(charinfo, "key %s (%d) pressed", SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.sym);
	  g_pErr->Debug(charinfo);
	  if (m_pCurTrial) {
	    m_pCurTrial->HandleEvent(&event);
	  } else {}
	}
      }
      break;
    default :
      if (m_pCurTrial) {
	m_pCurTrial->HandleEvent(&event);      
      } else {}
    }
  } else {
    //g_pErr->Debug("here");
    nResult = SBX_NO_EVENT;
  }

  return nResult;
}

int Experiment::Loop() {
  int nResult = SBX_NO_EVENT;

  while (1) {
    while( (nResult = PollEvent()) == SBX_EVENT ) ;
    if (nResult!=SBX_NO_EVENT) {
      break;
    } else {
      Update();
      SDL_Delay(1);
    }
  }

  return nResult;
}

int Experiment::FinishTrial() {
  int nResult = 0;

  g_pErr->DFI("Experiment::FinishTrial", 0L, 1);
  if (!m_pCurTrial) {
    g_pErr->Debug("in Experiment::FinishTrial; m_pCurTrial is null!");
  } else {
    if (g_prsStim->TableExists("TrialsTodo")) {
      // TODO: delete the trial from the list of trials to run
      g_prsStim->Exec(paste("sdsdsd", "", "DELETE FROM TrialsTodo WHERE SessionID=", m_idSession, " AND ItemCellID=",
			    m_pCurTrial->m_pItemCell->GetID(), " AND Ord=", m_pCurTrial->ID() ));
    } else {}

    nResult = m_pCurTrial->Finish();
  }

  g_pErr->DFO("Experiment::FinishTrial", 0L, 1);
  return nResult;
}

long Experiment::CurrentItemID() {
  return m_pCurTrial->m_pItemCell->GetID();
}

long Experiment::GetSessionID() {
  return m_idSession;
}

long Experiment::AutoselectList() {

  long idSession = 0;
  long idList = 0;

  string q1 = "SUM(IF(SessionID IS_NULL,0,1)) AS N";

  if (m_strDriver == "sqlite") {
    q1 = "SUM(case when SessionID > -1 then 1 else 0 end) AS N";
  } else {}

  string q = "SELECT List.ListID," + q1 + " FROM List LEFT JOIN (SELECT * FROM Session WHERE Completion=='COMPLETED') AS s ON List.ListID=s.ListID WHERE List.Excluded=0 GROUP BY List.ListID ORDER BY N ASC," + g_prsResp->RandCmd() + " LIMIT 1";

  g_prsResp->ExecCount(q.c_str());
  if (g_prsResp->NumRows() > 0) {
    idList = atol(g_prsResp->Get(0));    
  } else {
    g_pErr->Report("No lists available in database (or all lists are marked as excluded).");
  }

  if (idList > 0) {
    string s1;
    s1.assign(pastestr::paste("sddsssss", ", ",
			      "NULL", m_id, idList, g_prsResp->NowFn(),
			      "'NO_DATA'", "NULL", "NULL", "NULL"));

    g_prsResp->Insert("Session", s1.c_str());

    idSession = g_prsResp->LastInsertID("Session");
    g_pErr->Debug(pastestr::paste("sd", "", "SessionID ", idSession));
    //exit(0);
  } else {}
  
  return idSession;
}

int Experiment::UpdateSessionStatus(const char * pc) {
  if (!g_prsResp) {
    return(-1);
  } else {}

  g_prsResp->Exec(pastestr::paste("sssd", "",
		  "UPDATE Session SET Completion='", pc, "' WHERE SessionID=",
				  m_idSession));

  if (!strcmp(pc, "IN_PROGRESS")) {
    // set the time.
    g_prsResp->Exec(pastestr::paste("sssd", "",
				    "UPDATE Session SET Date=", 
				    g_prsResp->NowFn(), " WHERE SessionID=", 
				    m_idSession));
  } else {}

  return 0;
}

int Experiment::StoreTrialData() {
  TrialPtr pTrial;
  int i = 1;
  for (TrialMap::iterator ii = m_mapTrial.begin(); 
       ii != m_mapTrial.end(); ii++) {
    pTrial = (*ii).second;
    if (pTrial->WasVisited()) {
      pTrial->StoreData(i++, m_idSession, m_idSubj);
    } else {}
  }
  //g_prsStim->Exec("BEGIN TRANSACTION");
  //g_prsStim->Exec("COMMIT");
}

long Experiment::LoadOrCreateSubjects() {
  long idSubj = 0;
  g_prsResp->ExecCount(pastestr::paste("sd", "",
				  "SELECT SubjID FROM Subject WHERE SessionID=",
				  m_idSession));
  if (g_prsResp->NumRows() > 1) {
    g_pErr->Debug("WARNING: experiment software not yet set up for multiple subjects");
    idSubj = atol(g_prsResp->Get(0));
  } else if (g_prsResp->NumRows() == 1) {
    idSubj = atol(g_prsResp->Get(0));
  } else {
    // create new subject
    string s1;
    s1.assign(pastestr::paste("sddss", ", ", "NULL", m_idSession, 1, "'X'", "NULL"));
    g_prsResp->Insert("Subject", s1.c_str());

    idSubj = g_prsResp->LastInsertID("Subject");    
    g_pErr->Debug(pastestr::paste("sd", "", "Created new subject entry, id=", idSubj));
  }

  if (idSubj != 0) {
    m_idSubj = idSubj;
    g_pErr->Debug(pastestr::paste("sd", "", "Subject ID is ", idSubj));
  } else {
    g_pErr->Report(pastestr::paste("sd", "", 
				   "couldn't load or create entry in Subject table for SessionID ",
				   m_idSession));
  }

  return idSubj;
}

int Experiment::DeleteSession(long idSession) {
  g_pErr->DFI("Experiment::DeleteSession", "", 1);

  string qsubt;
  qsubt.assign(pastestr::paste("sds", "", " WHERE TrialID IN (SELECT TrialID FROM Trial WHERE SessionID=",
			       idSession, ")"));

  string qsubr;
  qsubr.assign(pastestr::paste("sds", "", " WHERE RespID IN (SELECT RespID FROM Response JOIN Trial USING (TrialID) WHERE SessionID=", idSession, ")"));

  //g_pErr->Debug("here");
  g_prsResp->DeleteWhere("StateTiming", qsubt.c_str());
  g_prsResp->DeleteWhere("EventTiming", qsubt.c_str());
  g_prsResp->DeleteWhere("Msg", qsubt.c_str());
  g_prsResp->DeleteWhere("Mouse", qsubr.c_str());
  g_prsResp->DeleteWhere("Eye", qsubr.c_str());
  g_prsResp->DeleteWhere("Keyboard", qsubr.c_str());
  g_prsResp->DeleteWhere("KeyTrack", qsubr.c_str());
  g_prsResp->DeleteWhere("ResponsesExcluded", qsubr.c_str());
  g_prsResp->DeleteWhere("GamePad", qsubr.c_str());

  string s1 = pastestr::paste("sds", "", " WHERE TrialID IN (SELECT TrialID FROM Trial WHERE SessionID=", idSession, ")");
  g_prsResp->DeleteWhere("Response", s1.c_str());

  s1 = pastestr::paste("sd", "", " WHERE SessionID=", idSession);
  g_prsResp->DeleteWhere("Trial", s1.c_str());

  g_pErr->DFO("Experiment::DeleteSession", "", 1);
  return 0;
}

int Experiment::Message(const char *pcMessage) {

  g_pDisplay->Message(pcMessage);

  return 0;
}

int Experiment::WaitKey() {
  int nDone = 0;
  SDL_Event event;

  while (!nDone) {
    if ( SDL_WaitEvent(&event) < 0 ) {
      g_pErr->Report(pastestr::paste("ss", "", "SDL_PullEvent() error: %s\n",  SDL_GetError()));
      nDone = 1;
      continue;
    }

    switch (event.type) {

    case SDL_KEYDOWN:
      nDone = 1;
      break;
    case SDL_QUIT:
      nDone = 1;
      SDL_PushEvent(&event);
      break;

    default:
      break;
    }

  }

  return 0;
}

int Experiment::LoadDevices() {

  g_pErr->DFI("Experiment::LoadDevices", "", 1);

  string q;
  q.assign(pastestr::paste("sd", "", "SELECT DeviceID, DevIndex FROM Device WHERE DeviceID=", SBX_GAMEPAD_DEV));

  dbData d = g_prsStim->Load(q);
  unsigned long idDev;
  int nIndex;

  if (d.size() > 0) {
    g_pErr->Debug(pastestr::paste("sds", " ", "need", (long) d.size(), "joysticks"));
    if (SDL_NumJoysticks() != d.size()) {
      g_pErr->Report(pastestr::paste("dsds", " ", (long) d.size(), "needed;", (long) SDL_NumJoysticks(), "found"));
    } else {
      for (int i = 0; i < d.size(); i++) {
	from_string(idDev, d[i][0], std::dec);
	from_string(nIndex, d[i][1], std::dec);
	Template::FindOrCreateInputDev(idDev, nIndex);
      }
    }
  } else {
    //g_pErr->Report("No lists available in database (or all lists are marked as excluded).");
  }

  g_pErr->DFO("Experiment::LoadDevices", "", 1);

  return 0;
}

string Experiment::GetResponseDir() {

  stringstream dir;
  stringstream sess;

  sess << GetSessionID();
  dir << "response/s" << string(7-sess.str().size(), '0') << sess.str();
  g_pErr->Debug(dir.str().c_str());

  return(dir.str());
}

void Experiment::CreateOrPurgeResponseDir(bool bResume) {

  g_pErr->DFI("Experiment::CreateOrPurgeResponseDir", "", 1);  

  string dir = GetResponseDir();
  struct stat st;

  // create the "response" path if it's not there
  if (stat("response", &st) < 0) {
    g_pErr->Debug("response directory does not exist... creating.");
    mkdir("response", 0777);
  } else {}

  // create the subdirectory if it doesn't exist
  if (stat(dir.c_str(), &st) < 0) {
    g_pErr->Debug(pastestr::paste("sss", " ", "response subdirectory", dir.c_str(), "does not exist... creating."));
    int res = mkdir(dir.c_str(), 0777);
    if (res == -1) {
      g_pErr->Report("couldn't make response subdirectory!");
    } else {}
  } else {
    // g_pErr->Debug(pastestr::paste("ss", " ", "purging files from", dir.c_str()));
    if (!bResume) {
      // TODO: Purge files
    } else {}
  }

  g_pErr->DFO("Experiment::CreateOrPurgeResponseDir", "", 1);  
}

void Experiment::PurgeTrialsTodo() {
  g_pErr->DFI("Experiment::PurgeTrialsTodo", (const char *) NULL);
  if (g_prsStim->TableExists("TrialsTodo")) {
    g_prsStim->Exec(paste("sd", "", "DELETE FROM TrialsTodo WHERE SessionID=", m_idSession));
  } else {}
  g_pErr->DFO("Experiment::PurgeTrialsTodo", (const char *) NULL);
}

void Experiment::FillTrialsTodo(vector<listordinfo> vlItems) {
  g_pErr->DFI("Experiment::FillTrialsTodo", (const char *) NULL);

  g_prsStim->Exec(paste("sssssssssss", "", "CREATE TABLE IF NOT EXISTS TrialsTodo (SessionID ",  g_prsStim->IDType(), 
			", ItemCellID ", g_prsStim->IDType(), 
			", BlockID ", g_prsStim->IDType(),
			", PhaseID ", g_prsStim->IDType(),
			", Ord ", g_prsStim->IDType(), ")"));

  g_prsStim->BeginTransaction();
  for (int i = 0; i < vlItems.size(); i++) {
    g_prsStim->Exec(paste("sdsdsdsdsds", "", "INSERT INTO TrialsTodo VALUES (", GetSessionID(), 
			  ", ", vlItems[i].m_idItemCell,
			  ", ", vlItems[i].m_idBlock,
			  ", ", vlItems[i].m_idPhase,
			  ", ", (long) i+1, 
			  ")"));
  }
  g_prsStim->EndTransaction();

  //string q = paste("sdsd", "", "INSERT INTO TrialsTodo SELECT ", m_idSession, " AS SessionID, ItemCellID FROM ListOrder WHERE ListID=", m_idList);
  //g_prsStim->Exec(q);

  g_pErr->DFO("Experiment::FillTrialsTodo", (const char *) NULL);
}

vector<listordinfo> Experiment::LoadTrialsTodo() {
  g_pErr->DFI("Experiment::LoadTrialsTodo", (const char *) NULL);  
  set<long>::iterator it;
  //trialtodo tt;
  listordinfo loi;
  vector<listordinfo> vlItems;

  g_prsStim->Exec(paste("sds", "", "SELECT ItemCellID, BlockID, PhaseID, Ord FROM TrialsTodo WHERE SessionID=", 
			m_idSession, " ORDER BY Ord"));
  //m_setTrialsTodo.clear();
  while (g_prsStim->RowReady()) {
    loi.assign(atol(g_prsStim->Get(0)), atol(g_prsStim->Get(1)), atol(g_prsStim->Get(2)), atol(g_prsStim->Get(3)));
    vlItems.push_back(loi);
    g_prsStim->MoveNext();
  }  
  g_pErr->Debug(paste("ds", "", (long) vlItems.size(), " trials remaining."));
  g_pErr->DFO("Experiment::LoadTrialsTodo", (const char *) NULL);  

  return vlItems;
}

string Experiment::GetFileName() {
  int iteration = 0;

  g_prsStim->Exec(pastestr::paste("sssss", "", "CREATE TABLE IF NOT EXISTS SessionRuns (SessionID ",
				  g_prsStim->IDType(), ", RunID ", g_prsStim->IDType(), ")"));

  g_prsStim->ExecCount(pastestr::paste("sd", "", "SELECT RunID FROM SessionRuns WHERE SessionID=",
				       GetSessionID()));

  if (g_prsStim->NumRows()==0) {
    iteration = 1;
  } else {
    if (!g_prsStim->RowReady()) {
      g_pErr->Report("problem with the experiment (Experiment::GetFileName)");
    } else {}
    iteration = atoi(g_prsStim->Get(0)) + 1;
  }

  g_prsStim->Exec(pastestr::paste("sdsds", "",
				  "INSERT INTO SessionRuns VALUES (", GetSessionID(), 
				  ", ", iteration, ")"));

  ostringstream ostr;

  ostr << "P";
  ostr << std::setw(3) << std::setfill('0') << GetSessionID();
  ostr << "-" << std::setw(2) << iteration << ".EDF";

  return ostr.str();
}

void Experiment::CreateTrialObjects(vector<listordinfo> vlItems, int nBegin) {
  g_pErr->DFI("Experiment::CreateTrialObjects", (const char *) NULL);  
  map<long, ItemCellPtr>::iterator it;

  for (int k = 0; k < vlItems.size(); k++) {
    it = m_mapItemCell.find(vlItems[k].m_idItemCell);
    if (it == m_mapItemCell.end()) {
      g_pErr->Report(pastestr::paste("sd", "", "Problem in SequenceItems(); missing ID ", vlItems[k].m_idItemCell));
    } else {}
    m_mapTrial[(long) k+nBegin] = TrialPtr(new Trial(k+nBegin, vlItems[k].m_idBlock, vlItems[k].m_idPhase ,it->second));
  }

  m_itTrial = m_mapTrial.find(nBegin);

  g_pErr->Debug(paste("sds", "", "There are ", m_mapTrial.size(), 
		      " trials in all"));

  g_pErr->DFO("Experiment::CreateTrialObjects", (const char *) NULL);  
}

void Experiment::RepeatExperiment() {
  // TODO repeat the experiment
  // TODO add items to the end of the experiment
  g_pErr->DFI("Experiment::RepeatExperiment", (const char *) NULL);
  vector<listordinfo> vlItems;

  vlItems = SequenceItems();
  int nBegin = m_mapTrial.size() + 1;
  m_bRefreshNeeded = true;

  // TODO
  CreateTrialObjects(vlItems, nBegin);
  FillTrialsTodo(vlItems);

  g_pErr->DFO("Experiment::RepeatExperiment", (const char *) NULL);
}

void Experiment::IncrementCounter(const char * pcCtr) {
  // TODO: increment the counter named by pcCtr
  //CounterMap::iterator it;
  //string s(pcCtr);

  g_pErr->Debug(pastestr::paste("ss", " ", "countername is", pcCtr));

  if (m_mapCounter.find(pcCtr)==m_mapCounter.end()) {
    m_mapCounter[pcCtr] = 1;
    //m_mapCounter.insert(std::pair<string, long>(s, 0));
    //it = m_mapCounter.find(pcCtr);
  } else {
    m_mapCounter[pcCtr] = m_mapCounter[pcCtr] + 1;
  }

  g_pErr->Debug(pastestr::paste("sssd", " ", "set counter", pcCtr, "to", m_mapCounter[pcCtr]));
}

void Experiment::ResetCounter(const char * pcCtr) {
   // TODO: reset the counter named by pcCtr
  m_mapCounter[pcCtr] = 0;
  g_pErr->Debug(pastestr::paste("sssd", " ", "*** reset counter", pcCtr, "to", m_mapCounter[pcCtr]));
}

long Experiment::GetCounter(const char * pcCtr) {
  // TODO: error checking!
  return m_mapCounter[pcCtr];
}

Uint32 Experiment::GetMSElapsed() {
  return (ClockFn()-m_msExpBegin);
}

Uint32 Experiment::GetTrialBegin() {
	Uint32 msBegin = 0;

	if (m_pCurTrial) {
		msBegin = m_pCurTrial->m_msBegin;
	} 

	return msBegin;
}

int Experiment::LoadConfiguration(unsigned long eid) {
	if (g_pConfig) {
		delete g_pConfig;
	} else {}

	g_pErr->Debug("LOADING EXPERIMENT CONFIGURATION...");
	g_pConfig = new EXPConfig(eid);

	return 0;
}
