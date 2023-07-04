#include "Experiment.hpp"
#include "StimulusWav.hpp"
#include "global.hpp"
#include "pastestr.hpp"

StimulusWav::StimulusWav(long id, Template * pTemplate, 
			 long idCmd, ArgMMap mmArgs) :
  Stimulus(id, pTemplate) {

  // parse out all common arguments
  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("Resource");
  pair<ArgIter, ArgIter> pii2 = mmArgs.equal_range("Resource");
  ArgIter ii;

  ii = pii.first;
  m_sResource = (*ii).second;

  m_nLoop = 0;
  pii = mmArgs.equal_range("Loops");
  if (pii.first != pii.second) {
    g_pErr->Debug("~- here -~");
    string s1( (*pii.first).second );
    istringstream iss(s1);
    iss >> m_nLoop;
  }

  m_bConditional = false;
  m_bConditional_TargetValue = true;
  m_nBit = 0;
  pii = mmArgs.equal_range("Bit"); // conditional playback
  if (pii.first != pii.second) {
    m_bConditional = true;
    string s1( (*pii.first).second );
    istringstream iss(s1);
    iss >> m_nBit;
    pii2 = mmArgs.equal_range("Value");
    if (pii.first != pii.second) {
      string s2( (*pii2.first).second );
      if (s2 == "true") {
      } else if (s2 == "false") {
	m_bConditional_TargetValue = false;
      } else {
	g_pErr->Report(
		       pastestr::paste("sss", "",
				       "unrecognized target value '",
				       s2, "' for PLAY_SOUND argument 'Value'"));
      }
    }
    
    g_pErr->Debug(pastestr::paste("sd", " ",
				  "conditional playback, bit", m_nBit));
  }

  g_pErr->DFI("StimulusWav::StimulusWav(mmArgs)", m_sResource.c_str(), 4);
  Initialize();
  g_pErr->DFO("StimulusWav::StimulusWav(mmArgs)", m_sResource.c_str(), 4);  
}

StimulusWav::StimulusWav(long id, Template * pTemplate,
			 const char * pName) : Stimulus(id, pTemplate) {
  g_pErr->DFI("StimulusWav::StimulusWav", pName, 3);
  m_sFileName.assign(pName);
  Initialize();
  g_pErr->DFO("StimulusWav::StimulusWav", pName, 3);
}

void StimulusWav::Initialize() {
  m_pSound = NULL;
}

StimulusWav::~StimulusWav() {
  g_pErr->DFI("StimulusWav::~StimulusWav", m_sFileName.c_str(), 3);

  g_pErr->DFO("StimulusWav::~StimulusWav", m_sFileName.c_str(), 3);
}

int StimulusWav::Prepare() {
  // load it in
  Stimulus::Prepare();

  g_pErr->DFI("StimulusWav::Prepare", m_sFileName.c_str(), 3);

  if (m_sFileName == "") {
    g_pErr->Report("WAV filename not defined!");
    return 1;
  } else {}

  string s1 = GetFilenameFromResource(m_sFileName);

  m_pSound = Mix_LoadWAV(s1.c_str());
  if (!m_pSound) {
    g_pErr->Report(pastestr::paste("ss", "", "Couldn't load WAV file ", s1.c_str()));
  } else {
  }

  g_pErr->DFO("StimulusWav::Prepare", m_sResource.c_str(), 3);
  return 0;
}

int StimulusWav::Finish() {
  g_pErr->DFI("StimulusWav::Finish", m_sResource.c_str(), 3);
  g_pErr->DFO("StimulusWav::Finish", m_sResource.c_str(), 3);
  
  return 0;
}

int StimulusWav::Cleanup() {
  g_pErr->DFI("StimulusWav::Cleanup", m_sResource.c_str(), 3);
  Mix_FreeChunk(m_pSound);
  m_pSound = NULL;
  g_pErr->DFO("StimulusWav::Cleanup", m_sResource.c_str(), 3);

  return 0;
}

int StimulusWav::Action() {
  bool bPlay = true;
  
  g_pErr->DFI("StimulusWav::Action", m_sResource.c_str(), 3);
  if (m_pSound == NULL) {
    g_pErr->Report("error: sound object not defined!");
  } else {}

  if (m_bConditional) {
    bPlay = Experiment::g_bsFlag.test(m_nBit) == m_bConditional_TargetValue;
  }

  if (bPlay) {
    if (Mix_PlayChannel(-1, m_pSound, m_nLoop) == -1) {
      g_pErr->Report("error playing back the audio");
    } else {}
  }

  g_pErr->DFI("StimulusWav::Action", m_sResource.c_str(), 3);
  return 0;
}
