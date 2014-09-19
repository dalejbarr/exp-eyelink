#include "global.hpp"
#include "StimulusEyeLinkMsg.hpp"
#include "Template.hpp"

#ifdef EXP_EYELINK_VERS
#include "eyelink.h"
#include "core_expt.h"
#endif

StimulusEyeLinkMsg::StimulusEyeLinkMsg(long id, Template * pTemplate, long idCmd, ArgMMap mmArgs) :
  Stimulus(id, pTemplate) {

  string s1;

  // parse out all common arguments
  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("MSG");
  ArgIter ii;

  ii = pii.first;
  s1 = (*ii).second;

  m_pcMsg = new char[s1.length()+1];
  strcpy(m_pcMsg, s1.c_str());
}

StimulusEyeLinkMsg::~StimulusEyeLinkMsg() {
  if (m_pcMsg) {
    delete[] m_pcMsg;
    m_pcMsg = NULL;
  } else {}
}

int StimulusEyeLinkMsg::Action() {
  g_pErr->DFI("StimulusEyeLinkMsg::Action", 0L, 2);
#ifdef EXP_EYELINK_VERS
  g_pErr->Debug(m_pcMsg);
  eyemsg_printf(m_pcMsg);
#endif
  g_pErr->DFO("StimulusEyeLinkMsg::Action", 0L, 2);
  return Stimulus::Action();
}
