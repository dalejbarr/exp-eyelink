#ifndef SBX_EVENTDRAWGRID_INCLUDED
#define SBX_EVENTDRAWGRID_INCLUDED

#include "Event.hpp"
#include "Template.hpp"
#include "xy.hpp"
#include "Display_SDL.hpp"

class EventGSC1DrawGrid : public Event {
protected:
  StimulusPtr m_pGridE;
  StimulusPtr m_pGridD;
  StimulusPtr m_pNext;
  StimulusPtr m_pCursorE;
  StimulusPtr m_pCursorD;
  vector<StimulusPtr> m_vBlankWhite;
  vector<StimulusPtr> m_vBlankBlack;
  vector<StimulusPtr> m_vFlags;
  vector<StimulusPtr> m_vBombs;
  vector<GSC1Toggle> m_vGSC1Toggle;
  vector<GSC1Cursor> m_vGSC1Cursor;
  vector<GSC1Locs> m_vGSC1Stars;
  vector<GSC1Locs> m_vGSC1Flags;
  //int m_nConfig; // 0 = encoder left, decoder right; 1 = decoder left, encoder right;
public:
  static bool g_bEncoderLeft; // which player is the encoder? (left or right?)
  vector<xy> m_vBombLocs;
  int m_nWidth;
  int m_nHeight;
  int m_nGridWidth;
  int m_nGridTopEncY;
  int m_nGridTopEncX;
  int m_nGridTopDecY;
  int m_nGridTopDecX;
  int m_nCellWidth;
  int m_nLeftTopX;
  int m_nLeftTopY;
  int m_nRightTopX;
  int m_nRightTopY;
  int m_nBombs;
  int m_nMargin;
  EventGSC1DrawGrid(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate);
  virtual ~EventGSC1DrawGrid();
  virtual int Prepare();
  virtual int Action();
  virtual int PostTrial();
  int Col2X(bool bEncoder, int c);
  int Row2Y(bool bEncoder, int r);
  int Cell2Row(int i);
  int Cell2Col(int i);
  void MoveUp();
  void MoveDown();
  void MoveLeft();
  void MoveRight();
  void Toggle();
  void GiveFeedback();
};

#endif
