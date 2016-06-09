#ifndef EXP_STIMULUSIMG_INCLUDED
#define EXP_STIMULUSIMG_INCLUDED

#include <string>
using std::string;

#include "Stimulus.hpp"
#include "SDL.h"
#include "Display_SDL.hpp"
#include "global.hpp"

#include <map>
using std::pair;
using std::multimap;

#include <string>
using std::string;

#include "Operation.hpp"

class StimulusImg : public Stimulus {
protected:
	SDL_Surface * m_pSurface;
	// static Display_SDL * s_pDisplay;
  bool m_bColorKey;
  bool m_bHighlight;
  unsigned int m_nHighlightWidth;
  int m_nHR; // highlight red
  int m_nHG; // highlight green
  int m_nHB; // highlight blue
  //bool m_bHighlight;
  //int m_nHighlightWidth;
  int m_r;
  int m_g;
  int m_b;
  //int * m_px;  // pointer to a x variable (might be m_xCur or mouse x)
  //int * m_py;  // pointer to a y variable (might be m_yCur or mouse y)
  void Initialize(); // initialize common variables
public:
  //Oplist m_x;
  //Oplist m_y;
  OpInt m_CurX;  // current x position
  OpInt m_CurY;  // current y position
  int m_x1, m_y1, m_x2, m_y2, m_nLayer;
  //virtual Oplist * GetAttr(string s);
  SDL_Rect m_rect;

  void swap(StimulusImg &other) {
    std::swap(m_pSurface, other.m_pSurface);
    std::swap(m_rect, other.m_rect);
    std::swap(m_bColorKey, other.m_bColorKey);
    std::swap(m_r, other.m_r);
    std::swap(m_g, other.m_g);
    std::swap(m_b, other.m_b);
    //std::swap(m_x, other.m_x);
    //std::swap(m_y, other.m_y);
    std::swap(m_CurX, other.m_CurX);
    std::swap(m_CurY, other.m_CurY);
    std::swap(m_sResource, other.m_sResource);
    std::swap(m_sFileName, other.m_sFileName);
    std::swap(m_x1, other.m_x1);
    std::swap(m_y1, other.m_y1);
    std::swap(m_x2, other.m_x2);
    std::swap(m_y2, other.m_y2);
    std::swap(m_nLayer, other.m_nLayer);
  }

  StimulusImg & operator =(StimulusImg other) {
    swap(other);
    return *this;
  }

  StimulusImg(long id, Template * pTemplate, const char * pName, 
							int x1, int y1, int x2 = -1, int y2 = -1, int nLayer = 0);
  StimulusImg(long id, Template * pTemplate, long idCmd, ArgMMap mmArgs);
  virtual ~StimulusImg();
  virtual int Prepare();
  virtual int Action();
  virtual int Finish();
  virtual int Erase(bool bMem = false);
  int Draw(bool bMem = 0);
  int SetColorkey(int r, int g, int b);
	// int CheckFileType(const char *);
  int InAOI(Uint16 x, Uint16 y);
  //inline void SetpXY(int *px, int *py) { m_x = px; m_y = py; }
  int SetLayer(int nLayer);
  void ResetLoc();
  void Highlight(const char * pHinfo);
  inline SDL_Surface * GetSurface() { return m_pSurface; }
  virtual int Load(string s1);
	virtual SDL_Surface * LoadImage(string s1); // specific to image type
  int DrawRect(int x1, int y1, int x2, int y2, int r, int g, int b);
};

#endif
