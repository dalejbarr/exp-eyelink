#include "StimulusTxt.hpp"
#include "StimulusImg.hpp"
#include "Template.hpp"

StimulusTxt::StimulusTxt(long id, Template * pTemplate, long idCmd, ArgMMap mmArgs) : Stimulus(id, pTemplate) {

  // parse out all common arguments
  pair<ArgIter, ArgIter> pii = mmArgs.equal_range("Text");
  ArgIter ii;

  ii = pii.first;
  m_sResource = (*ii).second;

  g_pErr->DFI("StimulusTxt::StimulusTxt()", m_sResource.c_str(), 4);

  pii = mmArgs.equal_range("x"); ii = pii.first;
  m_x1 = atoi((*ii).second.c_str());

  pii = mmArgs.equal_range("y"); ii = pii.first;
  m_y1 = atoi((*ii).second.c_str());

  /*
  pii = mmArgs.equal_range("ColorKey"); 
  if (pii.first != pii.second) {
    ii = pii.first;
    int r, g, b;
    istringstream iss((*pii.first).second);
    iss >> r >> g >> b;
    SetColorkey(r, g, b);      
    //g_pErr->Report(pastestr::paste("ddd", " ", (long) r, (long) g, (long) b));
    //m_nLayer = atoi((*ii).second.c_str());
  } else {
    //m_nLayer = 1;
  }
  */

  g_pErr->DFO("StimulusTxt::StimulusTxt()", m_sResource.c_str(), 4);  
}

StimulusTxt::~StimulusTxt() {
}

int StimulusTxt::Action() {
  g_pErr->DFI("StimulusTxt::Action", 0L, 2);
  string s1 = Stimulus::GetResourceString(m_sResource.c_str());

  //g_pErr->Debug(pastestr::paste("sssdd", " ", "*", s1.c_str(), "*", m_x1, m_y1));
  if (s1.length()>0) {
		Display_SDL::MessageXY(m_x1, m_y1, s1.c_str());
  } else {}
  g_pErr->DFO("StimulusTxt::Action", 0L, 2);
  return Stimulus::Action();
}

int StimulusTxt::Prepare() {
  g_pErr->DFI("StimulusTxt::Prepare", 0L, 2);
  g_pErr->DFO("StimulusTxt::Prepare", 0L, 2);
}
