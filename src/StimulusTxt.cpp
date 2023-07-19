#include "StimulusTxt.hpp"
#include "StimulusImg.hpp"
#include "Template.hpp"
#include "SDL.h"

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

  m_strFontFile = "seguibk.ttf";
  pii = mmArgs.equal_range("fontfile"); 
  if (pii.first != pii.second) {
    m_strFontFile = (*pii.first).second;
  }

  m_ptsize = 24;
  pii = mmArgs.equal_range("pointsize"); 
  if (pii.first != pii.second) {
    ii = pii.first;
    m_ptsize = atoi((*ii).second.c_str());
  }
  
  // TODO: make sure font file exists and can be opened
  TTF_Font * font = NULL;
  font = TTF_OpenFont(m_strFontFile.c_str(), m_ptsize);
  if (font == NULL) {
    g_pErr->Report(pastestr::paste("ss", " ",
				   "couldn't open font file",
				   m_strFontFile.c_str()));
  } else {}
  TTF_CloseFont(font);
  
  g_pErr->DFO("StimulusTxt::StimulusTxt()", m_sResource.c_str(), 4);  
}

StimulusTxt::~StimulusTxt() {
}

int StimulusTxt::Action() {
  g_pErr->DFI("StimulusTxt::Action", 0L, 2);

  string s1 = Stimulus::GetResourceString(m_sResource.c_str());

  //g_pErr->Debug(pastestr::paste("sssdd", " ", "*", s1.c_str(), "*", m_x1, m_y1));
  if (s1.length()>0) {
    Display_SDL::MessageXY(m_x1, m_y1, s1.c_str(), m_strFontFile.c_str(),
			   m_ptsize);
  } else {}

  g_pErr->DFO("StimulusTxt::Action", 0L, 2);
  return Stimulus::Action();
}

int StimulusTxt::Prepare() {
  g_pErr->DFI("StimulusTxt::Prepare", 0L, 2);
  g_pErr->DFO("StimulusTxt::Prepare", 0L, 2);

  return 0;
}
