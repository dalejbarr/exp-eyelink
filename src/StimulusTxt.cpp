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

  m_strPoint = "24";
  pii = mmArgs.equal_range("pointsize"); 
  if (pii.first != pii.second) {
    m_strPoint = (*pii.first).second;
  }
  
  // TODO: make sure font file exists and can be opened
  /*
    ## commented this out because it won't work if m_strFontFile can be
    ## a resource string, defined at runtime
  TTF_Font * font = NULL;
  font = TTF_OpenFont(m_strFontFile.c_str(), m_ptsize);
  if (font == NULL) {
    g_pErr->Report(pastestr::paste("ss", " ",
				   "couldn't open font file",
				   m_strFontFile.c_str()));
  } else {}
  TTF_CloseFont(font);
  */
  
  g_pErr->DFO("StimulusTxt::StimulusTxt()", m_sResource.c_str(), 4);  
}

StimulusTxt::~StimulusTxt() {
}

int StimulusTxt::Action() {
  g_pErr->DFI("StimulusTxt::Action", 0L, 2);

  string s1 = Stimulus::GetResourceString(m_sResource.c_str());
  string s2 = Stimulus::GetResourceString(m_strFontFile.c_str());
  string s3 = Stimulus::GetResourceString(m_strPoint.c_str());
  std::wstring text = L"你好世界";
  
  int ptsize = atoi(s3.c_str());

  g_pErr->Debug(pastestr::paste("sssssdsdsd", "",
				"string: *",
				s1.c_str(),
				"* font: ",
				s2.c_str(),
				" (", ptsize, ") at ",
				m_x1, " ",
				m_y1));
  
  if (s1.length()>0) {
    Display_SDL::MessageXY(m_x1, m_y1, s1.c_str(), s2.c_str(),
			   ptsize);
  } else {}

  g_pErr->DFO("StimulusTxt::Action", 0L, 2);
  return Stimulus::Action();
}

int StimulusTxt::Prepare() {
  g_pErr->DFI("StimulusTxt::Prepare", 0L, 2);
  g_pErr->DFO("StimulusTxt::Prepare", 0L, 2);

  return 0;
}
