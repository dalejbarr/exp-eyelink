#include "EventGSC1DrawGrid.hpp"
#include "StimulusWav.hpp"
#include "GSC1RespObj.hpp"
#include <sstream>

bool EventGSC1DrawGrid::g_bEncoderLeft = true;

EventGSC1DrawGrid::EventGSC1DrawGrid(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate) :
  Event(id, msec, idCmd, mmArgs, pTemplate) {

  StimulusPtrMMap mmapAOI;

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  const SDL_VideoInfo * sdlVid = SDL_GetVideoInfo();

  pii = mmArgs.equal_range("ScreenWidth");
  if (pii.first == pii.second) {
    m_nWidth = sdlVid->current_w;
  } else {
    from_string<int>(m_nWidth, (pii.first)->second, std::dec);
  }

  pii = mmArgs.equal_range("ScreenHeight");
  if (pii.first == pii.second) {
    m_nHeight = sdlVid->current_h;
  } else {
    from_string<int>(m_nHeight, (pii.first)->second, std::dec);
  }

  m_nGridWidth = 375;
  m_nCellWidth = 124;
  m_nMargin = 6;

  m_nLeftTopX = ((m_nWidth / 2) - m_nGridWidth) / 2;
  m_nLeftTopY = (m_nHeight - m_nGridWidth) / 2;

  m_nRightTopX = (m_nWidth / 2) + ((m_nWidth / 2) - m_nGridWidth) / 2;
  m_nRightTopY = m_nLeftTopY;

  if (EventGSC1DrawGrid::g_bEncoderLeft) {
    m_nGridTopEncY = m_nLeftTopY;
    m_nGridTopEncX = m_nLeftTopX;
    m_nGridTopDecY = m_nRightTopY;
    m_nGridTopDecX = m_nRightTopX;
  } else { // encoder right
    m_nGridTopEncY = m_nRightTopY;
    m_nGridTopEncX = m_nRightTopX;
    m_nGridTopDecY = m_nLeftTopY;
    m_nGridTopDecX = m_nLeftTopX;
  }

  string sName;

  sName.assign("GRIDENC");
  m_pGridE = StimulusPtr(new StimulusImg(0, NULL, sName.c_str(), m_nGridTopEncX, m_nGridTopEncY, -1, -1, 1));
  mmapAOI.insert(StimulusPair(sName.c_str(), m_pGridE));

  sName.assign("GRIDDEC");
  m_pGridD = StimulusPtr(new StimulusImg(0, NULL, sName.c_str(), m_nGridTopDecX, m_nGridTopDecY, -1, -1, 1));
  mmapAOI.insert(StimulusPair(sName.c_str(), m_pGridD));

  int nRow, nCol;
  StimulusPtr pStim;
  std::ostringstream osstr;
  std::string s;

  // add in the blanks and flags
  for (int i = 0; i < 9; i++) {
    nRow = Cell2Row(i); //(int) i / 3;
    nCol = Cell2Col(i); //i % 3;

    // blank white (decoder)
    osstr.str(std::string());
    osstr << "B" << i+1;
    pStim = StimulusPtr(new StimulusImg(0, NULL, osstr.str().c_str(), 
					Col2X(false, nCol),
					Row2Y(false, nRow),
					-1, -1, 3));
    mmapAOI.insert(StimulusPair(osstr.str().c_str(), pStim));
    m_vBlankWhite.push_back(pStim);

    // blank black (encoder)
    osstr.str(std::string());
    osstr << "E" << i+1;
    pStim = StimulusPtr(new StimulusImg(0, NULL, osstr.str().c_str(), 
					Col2X(true, nCol),
					Row2Y(true, nRow),
					-1, -1, 3));
    mmapAOI.insert(StimulusPair(osstr.str().c_str(), pStim));
    m_vBlankBlack.push_back(pStim);

    // flag (decoder)
    for (int j = 0; j < 2; j++) {
      osstr.str(std::string());
      osstr << "F" << i+1;
      pStim = StimulusPtr(new StimulusImg(0, NULL, osstr.str().c_str(), 
					  Col2X(j, nCol),
					  Row2Y(j, nRow),
					  -1, -1, 2));
      ((StimulusImg *) pStim.get())->SetColorkey(255,128,255);
      mmapAOI.insert(StimulusPair(osstr.str().c_str(), pStim));
      m_vFlags.push_back(pStim);
    }
  }

  // add in the Cursor
  s.assign("cursor");
  m_pCursorD = StimulusPtr(new StimulusImg(0, NULL, s.c_str(),
					  Col2X(false, 1),
					  Row2Y(false, 1),
					  -1, -1, 6));
  ((StimulusImg *) m_pCursorD.get())->SetColorkey(255,128,255);
  mmapAOI.insert(StimulusPair(s.c_str(), m_pCursorD));
  m_pCursorE = StimulusPtr(new StimulusImg(0, NULL, s.c_str(),
					  Col2X(true, 1),
					  Row2Y(true, 1),
					  -1, -1, 6));
  ((StimulusImg *) m_pCursorE.get())->SetColorkey(255,128,255);
  mmapAOI.insert(StimulusPair(s.c_str(), m_pCursorE));

  // add in the Next Button
  s.assign("Next");
  m_pNext = StimulusPtr(new StimulusImg(0, NULL, s.c_str(),
				      m_nGridTopDecX + (m_nGridWidth-116)/2,
				      m_nGridTopDecY + (m_nGridWidth+15),
				      -1, -1, 1));
  mmapAOI.insert(StimulusPair(s.c_str(), m_pNext));

  m_pTemplate->InsertAOIs(mmapAOI);
}

EventGSC1DrawGrid::~EventGSC1DrawGrid() {
  m_vBlankWhite.clear(); 
  m_vBlankBlack.clear();
  m_vFlags.clear();
  m_vBombs.clear();
  m_vBombLocs.clear();
  m_pCursorD.reset();
  m_pCursorE.reset();
  m_pNext.reset();
  m_vGSC1Toggle.clear();
  m_vGSC1Cursor.clear();
}

int EventGSC1DrawGrid::Prepare() {
  g_pErr->DFI("EventGSC1DrawGrid::Prepare", 0L, 3);
  int j = 0;

  m_vGSC1Stars.clear();
  m_vGSC1Flags.clear();

  pair<StimulusPtrMMap::iterator,StimulusPtrMMap::iterator> pii;
  StimulusPtrMMap::iterator ii;
  //
  // TODO: change m_nGridTopEnc/Dec
  //

  std::ostringstream osstr;

  if (EventGSC1DrawGrid::g_bEncoderLeft) {
    g_pErr->Debug("encoderleft");
    m_nGridTopEncY = m_nLeftTopY;
    m_nGridTopEncX = m_nLeftTopX;
    m_nGridTopDecY = m_nRightTopY;
    m_nGridTopDecX = m_nRightTopX;
  } else { // encoder right
    g_pErr->Debug("encoderright");
    m_nGridTopEncY = m_nRightTopY;
    m_nGridTopEncX = m_nRightTopX;
    m_nGridTopDecY = m_nLeftTopY;
    m_nGridTopDecX = m_nLeftTopX;
  }

  m_vGSC1Toggle.clear();
  m_vGSC1Cursor.clear();

  StimulusImg * pStim = (StimulusImg *) m_pTemplate->m_mmapAOI.find("GRIDENC")->second.get();
  //  pStim = (StimulusImg *) m_pGridE.get();
  pStim->LoadBMP("resource/gridonly.bmp");
  pStim->m_x1 = m_nGridTopEncX;
  pStim->m_CurX.Set(m_nGridTopEncX);
  pStim->m_y1 = m_nGridTopEncY;
  pStim->m_CurY.Set(m_nGridTopEncY);

  pStim = (StimulusImg *) m_pTemplate->m_mmapAOI.find("GRIDDEC")->second.get();
  pStim->LoadBMP("resource/gridonly.bmp");
  pStim->m_x1 = m_nGridTopDecX;
  pStim->m_CurX.Set(m_nGridTopDecX);
  pStim->m_y1 = m_nGridTopDecY;
  pStim->m_CurY.Set(m_nGridTopDecY);

  // make sure all the AOIs have the correct starting layers/position
  for (int i = 0; i < m_vBlankWhite.size(); i++) {

    osstr.str(std::string());
    osstr << "B" << i+1;
    pStim = (StimulusImg *) m_pTemplate->m_mmapAOI.find(osstr.str().c_str())->second.get();
    //pStim = (StimulusImg *) m_vBlankWhite[i].get();
    pStim->LoadBMP("resource/blankwhite.bmp");
    pStim->m_nLayer = 3;
    pStim->m_x1 = Col2X(false, Cell2Col(i));
    pStim->m_CurX.Set(pStim->m_x1);
    pStim->m_y1 = Row2Y(false, Cell2Row(i));
    pStim->m_CurY.Set(pStim->m_y1);

    osstr.str(std::string());
    osstr << "E" << i+1;
    //pStim = (StimulusImg *) m_vBlankBlack[i].get();
    pStim = (StimulusImg *) m_pTemplate->m_mmapAOI.find(osstr.str().c_str())->second.get();
    pStim->LoadBMP("resource/blankblack.bmp");
    pStim->m_nLayer = 3;
    pStim->m_x1 = Col2X(true, Cell2Col(i));
    pStim->m_y1 = Row2Y(true, Cell2Row(i));
    pStim->m_CurX.Set(pStim->m_x1);
    pStim->m_CurY.Set(pStim->m_y1);

    osstr.str(std::string());
    osstr << "F" << i+1;
    pii = m_pTemplate->m_mmapAOI.equal_range(osstr.str().c_str());
    j = 0;
    for (ii = pii.first; ii != pii.second; ii++, j++) {
      pStim = (StimulusImg *) ii->second.get();
      //pStim = (StimulusImg *) m_pTemplate->m_mmapAOI.find(osstr.str().c_str())->second.get();
      pStim->LoadBMP("resource/flagcolor.bmp");
      pStim->m_nLayer = 2;
      pStim->m_x1 = Col2X(j, Cell2Col(i));
      pStim->m_CurX.Set(pStim->m_x1);
      pStim->m_y1 = Row2Y(j, Cell2Row(i));
      pStim->m_CurY.Set(pStim->m_y1);      
    }
    //pStim = (StimulusImg *) m_vFlags[i].get();
      /*
      */
  }

  //pStim = (StimulusImg *) m_pNext.get();
  pStim = (StimulusImg *) m_pTemplate->m_mmapAOI.find("Next")->second.get();
  pStim->LoadBMP("resource/next1.bmp");
  pStim->m_x1 = m_nGridTopDecX + (m_nGridWidth-116)/2;
  pStim->m_y1 = m_nGridTopDecY + (m_nGridWidth+15);
  pStim->m_CurX.Set(pStim->m_x1);
  pStim->m_CurY.Set(pStim->m_y1);
  g_pErr->Debug(pastestr::paste("sdd", " ", "nextxy", (long) pStim->m_x1, (long) pStim->m_y1));

  //pStim = (StimulusImg *) m_pCursor.get();

  pii = m_pTemplate->m_mmapAOI.equal_range("cursor");
  for (ii = pii.first; ii != pii.second; ii++) {
    pStim = (StimulusImg *) ii->second.get();
    pStim->m_nLayer = 6;
    pStim->LoadBMP("resource/cursor.bmp");
    if (pStim == m_pCursorD.get()) {
      pStim->m_x1 = Col2X(false, 1);
      pStim->m_y1 = Row2Y(false, 1);
    } else {
      pStim->m_x1 = Col2X(true, 1);
      pStim->m_y1 = Row2Y(true, 1);
    }      
    pStim->m_CurX.Set(pStim->m_x1);
    pStim->m_CurY.Set(pStim->m_y1);
  }

  // TODO setup the bombs
  if (rand() % 2) {
    g_pErr->Debug("three bombs");
    m_nBombs = 3;
  } else {
    g_pErr->Debug("two bombs");
    m_nBombs = 2;
  }
  //m_nBombs = 2;
  
  StimulusPtr pStim2;  
  StimulusPtrMMap mmapAOI;
  int nLoc = -1;
  int nCol = -1;
  int nRow = -1;
  //int * pnUsed;
  //pnUsed = new int[9];
  int pnUsed[9];

  for (int i = 0; i < 9; i++) {
    pnUsed[i] = 0;
  }

  for (int i = 0; i < m_nBombs; i++) {
    do {
      nLoc = rand() % 9;
      g_pErr->Debug(pastestr::paste("sdsd", " ", "rnum is", (long) nLoc, "used is", (long) pnUsed[nLoc]));
    } while(pnUsed[nLoc]==1) ;
    g_pErr->Debug(pastestr::paste("sd", " ", "chosen rnum is", (long) nLoc));
    pnUsed[nLoc] = 1;
    nCol = Cell2Col(nLoc);
    nRow = Cell2Row(nLoc);
    pStim2 = StimulusPtr(new StimulusImg(0, NULL, "Bomb", 
					Col2X(true, nCol),
					Row2Y(true, nRow),
					-1, -1, 4));
    ((StimulusImg *) pStim2.get())->LoadBMP("resource/bombblack.bmp");
    mmapAOI.insert(StimulusPair("Bomb", pStim2));
    m_vGSC1Stars.push_back(GSC1Locs(nRow+1, nCol+1));
    m_vBombs.push_back(pStim2);
    m_vBombLocs.push_back(xy(1, 1));
  }

  /*
  pStim2 = StimulusPtr(new StimulusImg(0, NULL, "Bomb", 
					Col2X(true, 1),
					Row2Y(true, 2),
					-1, -1, 3));
  ((StimulusImg *) pStim2.get())->LoadBMP("resource/bombblack.bmp");
  mmapAOI.insert(StimulusPair("Bomb", pStim2));
  m_vBombs.push_back(pStim2);
  m_vBombLocs.push_back(xy(2, 3));
  */

  m_pTemplate->AddAOIs(mmapAOI, true);

  m_pTemplate->Redraw(true); // redraw to memory
  m_pTemplate->Redraw(false); // redraw to screen

  g_pErr->DFO("EventGSC1DrawGrid::Prepare", 0L, 3);

  return 0;
}

int EventGSC1DrawGrid::Action() {
  g_pErr->DFO("EventGSC1DrawGrid::Action", 0L, 3);
  m_pTemplate->Redraw(false);
  g_pErr->DFO("EventGSC1DrawGrid::Action", 0L, 3);
  return 0;
}

int EventGSC1DrawGrid::Col2X(bool bEncoder, int c) {
  int x0 = 0;
  if (bEncoder) {
    x0 = m_nGridTopEncX;
  } else {
    x0 = m_nGridTopDecX;
  }
  return (x0 + c*m_nCellWidth + m_nMargin);
}

int EventGSC1DrawGrid::Row2Y(bool bEncoder, int r) {
  int y0 = 0;
  if (bEncoder) {
    y0 = m_nGridTopEncY;
  } else {
    y0 = m_nGridTopDecY;
  }
  return (y0 + r*m_nCellWidth + m_nMargin);
}

int EventGSC1DrawGrid::Cell2Row(int i) {
  return (i / 3);
}

int EventGSC1DrawGrid::Cell2Col(int i) {
  return (i % 3);
}

void EventGSC1DrawGrid::MoveUp() {
  g_pErr->DFI("EventGSC1DrawGrid::MoveUp", 0L, 3);

  StimulusImg * pStimD = (StimulusImg *) m_pCursorD.get();
  StimulusImg * pStimE = (StimulusImg *) m_pCursorE.get();
  
  xy xyThisD = Loc2Grid(xy(pStimD->m_x1, pStimD->m_y1),
		       xy(m_nGridTopDecX, m_nGridTopDecY), m_nCellWidth);
  xy xyThisE = Loc2Grid(xy(pStimE->m_x1, pStimE->m_y1),
		       xy(m_nGridTopEncX, m_nGridTopEncY), m_nCellWidth);

  if (xyThisD.m_y > 1) { // not in the top row
    m_vGSC1Cursor.push_back(GSC1Cursor(ClockFn(), xyThisD.m_y-1, xyThisD.m_x));

    xyThisD.m_y = xyThisD.m_y - 2;
    pStimD->m_y1 = Row2Y(false, xyThisD.m_y);
    pStimD->m_CurY.Set(pStimD->m_y1);

    xyThisE.m_y = xyThisE.m_y - 2;
    pStimE->m_y1 = Row2Y(true, xyThisE.m_y);
    pStimE->m_CurY.Set(pStimE->m_y1);

    m_pTemplate->Redraw(0);
  } else {} 

  g_pErr->DFO("EventGSC1DrawGrid::MoveUp", 0L, 3);
}

void EventGSC1DrawGrid::MoveDown() {
  g_pErr->DFI("EventGSC1DrawGrid::MoveDown", 0L, 3);

  StimulusImg * pStimD = (StimulusImg *) m_pCursorD.get();
  StimulusImg * pStimE = (StimulusImg *) m_pCursorE.get();
  
  xy xyThisD = Loc2Grid(xy(pStimD->m_x1, pStimD->m_y1),
		       xy(m_nGridTopDecX, m_nGridTopDecY), m_nCellWidth);
  xy xyThisE = Loc2Grid(xy(pStimE->m_x1, pStimE->m_y1),
		       xy(m_nGridTopEncX, m_nGridTopEncY), m_nCellWidth);

  if (xyThisD.m_y < 3) { // not in the bottom row
    m_vGSC1Cursor.push_back(GSC1Cursor(ClockFn(), xyThisD.m_y+1, xyThisD.m_x));

    pStimD->m_y1 = Row2Y(false, xyThisD.m_y);
    pStimD->m_CurY.Set(pStimD->m_y1);
    m_pTemplate->Redraw(0);

    pStimE->m_y1 = Row2Y(true, xyThisE.m_y);
    pStimE->m_CurY.Set(pStimE->m_y1);
    m_pTemplate->Redraw(0);

  } else {} 

  g_pErr->DFO("EventGSC1DrawGrid::MoveDown", 0L, 3);
}

void EventGSC1DrawGrid::MoveLeft() {
  g_pErr->DFI("EventGSC1DrawGrid::MoveLeft", 0L, 3);

  StimulusImg * pStimD = (StimulusImg *) m_pCursorD.get();
  StimulusImg * pStimE = (StimulusImg *) m_pCursorE.get();
  
  xy xyThisD = Loc2Grid(xy(pStimD->m_x1, pStimD->m_y1),
		       xy(m_nGridTopDecX, m_nGridTopDecY), m_nCellWidth);
  xy xyThisE = Loc2Grid(xy(pStimE->m_x1, pStimE->m_y1),
		       xy(m_nGridTopEncX, m_nGridTopEncY), m_nCellWidth);

  if (xyThisD.m_x > 1) { // not in the leftmost col
    m_vGSC1Cursor.push_back(GSC1Cursor(ClockFn(), xyThisD.m_y, xyThisD.m_x-1));

    xyThisD.m_x = xyThisD.m_x - 2;
    pStimD->m_x1 = Col2X(false, xyThisD.m_x);
    pStimD->m_CurX.Set(pStimD->m_x1);

    xyThisE.m_x = xyThisE.m_x - 2;
    pStimE->m_x1 = Col2X(true, xyThisE.m_x);
    pStimE->m_CurX.Set(pStimE->m_x1);

    m_pTemplate->Redraw(0);
  } else {} 

  g_pErr->DFO("EventGSC1DrawGrid::MoveLeft", 0L, 3);
}

void EventGSC1DrawGrid::MoveRight() {
  g_pErr->DFI("EventGSC1DrawGrid::MoveRight", 0L, 3);

  StimulusImg * pStimD = (StimulusImg *) m_pCursorD.get();
  StimulusImg * pStimE = (StimulusImg *) m_pCursorE.get();
  
  xy xyThisD = Loc2Grid(xy(pStimD->m_x1, pStimD->m_y1),
		       xy(m_nGridTopDecX, m_nGridTopDecY), m_nCellWidth);
  xy xyThisE = Loc2Grid(xy(pStimE->m_x1, pStimE->m_y1),
		       xy(m_nGridTopEncX, m_nGridTopEncY), m_nCellWidth);

  if (xyThisD.m_x < 3) { // not in the rightmost col
    m_vGSC1Cursor.push_back(GSC1Cursor(ClockFn(), xyThisD.m_y, xyThisD.m_x+1));

    pStimD->m_x1 = Col2X(false, xyThisD.m_x);
    pStimD->m_CurX.Set(pStimD->m_x1);
    m_pTemplate->Redraw(0);

    pStimE->m_x1 = Col2X(true, xyThisE.m_x);
    pStimE->m_CurX.Set(pStimE->m_x1);
    m_pTemplate->Redraw(0);
  } else {} 

  g_pErr->DFO("EventGSC1DrawGrid::MoveRight", 0L, 3);
}

void EventGSC1DrawGrid::Toggle() {
  StimulusImg * pFlag = NULL;
  //StimulusImg * pBlank = NULL;
  pair<StimulusPtrMMap::iterator, StimulusPtrMMap::iterator> piiFlag; //, piiBlank;
  //StimulusPtrMMap::iterator ii;
  StimulusPtrMMap::iterator iiFlag, iiBlank;
  int nLayer = 0;

  g_pErr->Debug("toggling");

  // figure out cursor position
  StimulusImg * pStim = (StimulusImg *) m_pCursorD.get();
  xy xyThis = Loc2Grid(xy(pStim->m_x1, pStim->m_y1),
		       xy(m_nGridTopDecX, m_nGridTopDecY), m_nCellWidth);
  int nCell = xyThis.toScalar();

  // find flag
  std::ostringstream osstr;
  osstr << "F" << nCell;
  piiFlag = m_pTemplate->m_mmapAllAOI.equal_range(osstr.str().c_str());
  osstr.str(std::string());
  //osstr << "B" << nCell;
  //piiBlank = m_pTemplate->m_mmapAllAOI.equal_range(osstr.str().c_str());
  //iiFlag = m_pTemplate->m_mmapAllAOI.find(osstr.str().c_str());
  //pFlag = (StimulusImg *) iiFlag->second.get();
  // find blank
  //iiBlank = m_pTemplate->m_mmapAllAOI.find(osstr.str().c_str());
  //pBlank = (StimulusImg *) iiBlank->second.get();

  //iiBlank = piiBlank.first;
  int nFlag = 0;
  bool bFirst = 0;
  //  for (iiFlag = piiFlag.first; iiFlag != piiFlag.second; iiFlag++, iiBlank++) {
  for (iiFlag = piiFlag.first; iiFlag != piiFlag.second; iiFlag++) {
    pFlag = (StimulusImg *) iiFlag->second.get();
    //pBlank = (StimulusImg *) iiBlank->second.get();

    if (pFlag) {
      // swap the aois and redraw.
      nLayer = pFlag->m_nLayer;
      if (pFlag->m_nLayer == 5) {
	nFlag = 0; // turning off
	pFlag->m_nLayer = 2;
      } else {
	nFlag = 1;
	pFlag->m_nLayer = 5;
      }
      //pFlag->m_nLayer = pBlank->m_nLayer;
      //pBlank->m_nLayer = nLayer;

      m_pTemplate->Reinsert(pFlag);
      //m_pTemplate->Reinsert(pBlank);
      m_pTemplate->Redraw(false);

      if (!bFirst) {
	m_vGSC1Toggle.push_back(GSC1Toggle(ClockFn(), xyThis.m_y, xyThis.m_x, nFlag));
	bFirst = true;
      } else {}
      
    } else {
      g_pErr->Debug("can't find aois!!");
    }
  }
}

void EventGSC1DrawGrid::GiveFeedback() {

  vector<StimulusImg *> vpAOIBomb;
  pair<StimulusPtrMMapIter, StimulusPtrMMapIter> pii;
  StimulusPtrMMap::iterator ii, ii0, ii1, ii2;
  StimulusImg * pAOI1 = NULL;
  StimulusImg * pAOI2 = NULL;
  typedef map<xy, StimulusImg *> mapXYAOI;
  mapXYAOI mapBomb;
  xy bloc;
  //StimulusWav wavLose(0, NULL, "explosion-03.wav");
  StimulusWav wavSuccess(0, NULL, "success.wav");
  StimulusWav wavFail(0, NULL, "failure.wav");
  int nLose = 0;
  vector<StimulusPtr> vFlagsForEncoder;
  vector<StimulusPtr> vStarsForDecoder;
  vector<StimulusPtr> vRedX;
  vector<StimulusPtr> vGreenCheck;
  std::ostringstream osstr;

  g_pErr->DFI("EventGSC1DrawGrid::GiveFeedback", 0L, 3);

  int nGridSide = m_nCellWidth;
  xy xyTopLeftD;
  
  xyTopLeftD.m_x = m_nGridTopDecX;
  xyTopLeftD.m_y = m_nGridTopDecY;

  xy xyTopLeftE;
  xyTopLeftE.m_x = m_nGridTopEncX; // = pAOI1->m_x1;
  xyTopLeftE.m_y = m_nGridTopEncY; // pAOI1->m_y1;

  g_pErr->Debug(pastestr::paste("sdsddsdd", " ", "gridside:", (long) nGridSide, 
				"topleftE(X,Y):", (long) xyTopLeftE.m_x, (long) xyTopLeftE.m_y,
				"topleftD(X,Y):", (long) xyTopLeftD.m_x, (long) xyTopLeftD.m_y));

  ///////////////////////////////////////
  // create map of bombs
  mapXYAOI::iterator xyii;
  pii = m_pTemplate->m_mmapAllAOI.equal_range("Bomb");
  StimulusPtr p1;
  for (ii = pii.first; ii != pii.second; ii++) {
    pAOI1 = (StimulusImg *) (*ii).second.get();
    bloc = Loc2Grid(xy(pAOI1->m_x1, pAOI1->m_y1), xyTopLeftE, nGridSide);
    mapBomb[bloc] = pAOI1;
    p1 = StimulusPtr(new StimulusImg(0, NULL, "StarFbk", 
				     Col2X(false, bloc.m_x - 1), 
				     Row2Y(false, bloc.m_y - 1), -1, -1, 4));
    vStarsForDecoder.push_back(p1);
    // put bombs down to layer 1
    //pAOI1->m_nLayer = 1;
    //m_pTemplate->Reinsert(pAOI1);
    g_pErr->Debug(pastestr::paste("sdd", " ", "star", (long) bloc.m_y, (long) bloc.m_x));
  }

  ((StimulusImg *) m_pCursorE.get())->m_nLayer = 1;
  ((StimulusImg *) m_pCursorD.get())->m_nLayer = 1;
  m_pTemplate->Reinsert((StimulusImg *) m_pCursorE.get());
  m_pTemplate->Reinsert((StimulusImg *) m_pCursorD.get());

  // put all decoder blanks down to layer 3 (so they don't obscure stars on layer 4)
  // later: move all flags up to layer 5
  /*
  ii0 = m_pTemplate->m_mmapAllAOI.find("B1");
  ii1 = m_pTemplate->m_mmapAllAOI.find("B9");
  ii1++;
  for (ii = ii0; ii != ii1; ii++) {
    pAOI1 = (StimulusImg *) (*ii).second.get();
    pAOI1->m_nLayer = 3;
    m_pTemplate->Reinsert(pAOI1);
    pAOI1 = (StimulusImg *) (*ii1).second.get();
  }
  */

  for (int i = 0; i < vStarsForDecoder.size(); i++) {
    m_pTemplate->m_mmapAllAOI2.insert(StimulusPair2(4, vStarsForDecoder[i]));
    ((StimulusImg *) vStarsForDecoder[i].get())->LoadBMP("resource/bombblack.bmp");
    //Reinsert((StimulusImg *) vStarsForDecoder[i].get());
  }


  //////////////////////////////////////
  // now process flags
  //
  pair<StimulusPtrMMap::iterator, StimulusPtrMMap::iterator> piix;
  StimulusPtrMMap::iterator iix;
  for (int i = 0; i < 9; i++) {
    osstr.str(std::string());
    osstr << "F" << (i+1);
    piix = m_pTemplate->m_mmapAllAOI.equal_range(osstr.str().c_str());
    iix = piix.first;

    pAOI1 = (StimulusImg *) iix->second.get();
    bloc = Loc2Grid(xy(pAOI1->m_x1, pAOI1->m_y1), xyTopLeftD, nGridSide);
    if (pAOI1->m_nLayer == 5) {
      g_pErr->Debug(pastestr::paste("sdd", " ", "flag at", (long) bloc.m_y, (long) bloc.m_x));
      if (mapBomb.find(bloc) != mapBomb.end()) {
	// +flag, +bomb = successfully defused!
	p1 = StimulusPtr(new StimulusImg(0, NULL, "Check", 
					 Col2X(true, bloc.m_x - 1), 
					 Row2Y(true, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/check.bmp");
	vGreenCheck.push_back(p1);

	p1 = StimulusPtr(new StimulusImg(0, NULL, "Check", 
					 Col2X(false, bloc.m_x - 1), 
					 Row2Y(false, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/check.bmp");
	vGreenCheck.push_back(p1);

	m_vGSC1Flags.push_back(GSC1Locs(bloc.m_y, bloc.m_x));
      } else {
	// +flag, -bomb = error!
	p1 = StimulusPtr(new StimulusImg(0, NULL, "RedX", 
					 Col2X(true, bloc.m_x - 1), 
					 Row2Y(true, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/redx.bmp");
	vRedX.push_back(p1);

	p1 = StimulusPtr(new StimulusImg(0, NULL, "RedX", 
					 Col2X(false, bloc.m_x - 1), 
					 Row2Y(false, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/redx.bmp");
	vRedX.push_back(p1);

	m_vGSC1Flags.push_back(GSC1Locs(bloc.m_y, bloc.m_x));
	nLose = 1;
      }
    } else {
      // no flag here.
      pAOI1->m_nLayer = 2;
      if (mapBomb.find(bloc) != mapBomb.end()) {
	// -flag, +bomb = exploded!
	p1 = StimulusPtr(new StimulusImg(0, NULL, "RedX", 
					 Col2X(true, bloc.m_x - 1), 
					 Row2Y(true, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/redx.bmp");
	vRedX.push_back(p1);

	p1 = StimulusPtr(new StimulusImg(0, NULL, "RedX", 
					 Col2X(false, bloc.m_x - 1), 
					 Row2Y(false, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/redx.bmp");
	vRedX.push_back(p1);
	nLose = 1;
      } else {
	// -flag, -bomb = OK
      }
    }
  }

  //ii0 = m_pTemplate->m_mmapAllAOI.find("F1");
  //ii1 = m_pTemplate->m_mmapAllAOI.find("F9");
  //ii1++;

  /*
  for (ii = ii0; ii != ii1; ii++) {
    pAOI1 = (StimulusImg *) (*ii).second.get();
    if (pAOI1->m_nLayer == 5) {
      bloc = Loc2Grid(xy(pAOI1->m_x1, pAOI1->m_y1), xyTopLeftD, nGridSide);
      p1 = StimulusPtr(new StimulusImg(0, NULL, "FlagFbk", 
				       Col2X(true, bloc.m_x - 1), 
				       Row2Y(true, bloc.m_y - 1), -1, -1, 3));
      ((StimulusImg *) p1.get())->SetColorkey(255,128,255);
      ((StimulusImg *) p1.get())->LoadBMP("resource/flagcolor.bmp");
      m_pTemplate->m_mmapAllAOI2.insert(StimulusPair2(3, p1));
      vFlagsForEncoder.push_back(p1);
    } else {}
  }
  */

  //ii2 = m_pTemplate->m_mmapAllAOI.find("E1");

  /*
  for (ii = ii0; ii != ii1; ii++, ii2++) {
    pAOI1 = (StimulusImg *) (*ii).second.get();
    //pAOI2 = (StimulusImg *) (*ii2).second.get();
    bloc = Loc2Grid(xy(pAOI1->m_x1, pAOI1->m_y1), xyTopLeftD, nGridSide);
    if (pAOI1->m_nLayer == 5) {
      //pAOI1->m_nLayer == 5;
      //m_pTemplate->Reinsert(pAOI1);
      g_pErr->Debug(pastestr::paste("sdd", " ", "flag at", (long) bloc.m_y, (long) bloc.m_x));
      if (mapBomb.find(bloc) != mapBomb.end()) {
	// +flag, +bomb = successfully defused!
	//pAOI1->LoadBMP("resource/bombblackdefusedcheck.bmp");
	p1 = StimulusPtr(new StimulusImg(0, NULL, "Check", 
					 Col2X(true, bloc.m_x - 1), 
					 Row2Y(true, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/check.bmp");
	vGreenCheck.push_back(p1);

	p1 = StimulusPtr(new StimulusImg(0, NULL, "Check", 
					 Col2X(false, bloc.m_x - 1), 
					 Row2Y(false, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/check.bmp");
	vGreenCheck.push_back(p1);

	m_vGSC1Flags.push_back(GSC1Locs(bloc.m_y, bloc.m_x));

	//pAOI2->LoadBMP("resource/bombblackdefusedcheck.bmp");
      } else {
	// +flag, -bomb = error!
	p1 = StimulusPtr(new StimulusImg(0, NULL, "RedX", 
					 Col2X(true, bloc.m_x - 1), 
					 Row2Y(true, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/redx.bmp");
	vRedX.push_back(p1);

	p1 = StimulusPtr(new StimulusImg(0, NULL, "RedX", 
					 Col2X(false, bloc.m_x - 1), 
					 Row2Y(false, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/redx.bmp");
	vRedX.push_back(p1);

	m_vGSC1Flags.push_back(GSC1Locs(bloc.m_y, bloc.m_x));

	//pAOI1->LoadBMP("resource/blackredX.bmp");
	//pAOI2->LoadBMP("resource/blackredX.bmp");
	nLose = 1;
      }
    } else {
      // no flag here.
      pAOI1->m_nLayer = 2;
      //m_pTemplate->Reinsert(pAOI1);
      if (mapBomb.find(bloc) != mapBomb.end()) {
	// -flag, +bomb = exploded!
	p1 = StimulusPtr(new StimulusImg(0, NULL, "RedX", 
					 Col2X(true, bloc.m_x - 1), 
					 Row2Y(true, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/redx.bmp");
	vRedX.push_back(p1);

	p1 = StimulusPtr(new StimulusImg(0, NULL, "RedX", 
					 Col2X(false, bloc.m_x - 1), 
					 Row2Y(false, bloc.m_y - 1), -1, -1, 3));
	((StimulusImg *) p1.get())->SetColorkey(255,128,255);
	((StimulusImg *) p1.get())->LoadBMP("resource/redx.bmp");
	vRedX.push_back(p1);
	//pAOI1->LoadBMP("resource/explosion.bmp");
	//pAOI2->LoadBMP("resource/explosion.bmp");
	nLose = 1;
      } else {
	// -flag, -bomb = OK
	//pAOI1->LoadBMP("resource/blankblack.bmp");
	//pAOI2->LoadBMP("resource/blankblack.bmp");
      }
    }
  }
  */

  m_pTemplate->Redraw(0);
  if (nLose) {
    //wavLose.Prepare();
    wavFail.Prepare();
    //wavLose.Action();

    for (int i = 0; i < vRedX.size(); i++) {
      m_pTemplate->m_mmapAllAOI2.insert(StimulusPair2(6, vRedX[i]));
    }

    wavFail.Action();
    SDL_Delay(1000);

    m_pTemplate->Redraw(0);
    SDL_Delay(3000);

    //wavLose.Finish();
    wavFail.Finish();
  } else {
    wavSuccess.Prepare();
    for (int i = 0; i < vGreenCheck.size(); i++) {
      m_pTemplate->m_mmapAllAOI2.insert(StimulusPair2(6, vGreenCheck[i]));
    }

    wavSuccess.Action();
    SDL_Delay(1000);
    m_pTemplate->Redraw(false);
    SDL_Delay(1500);
    wavSuccess.Finish();
  }

  EventGSC1DrawGrid::g_bEncoderLeft = !EventGSC1DrawGrid::g_bEncoderLeft;

  vStarsForDecoder.clear();
  vFlagsForEncoder.clear();
  vRedX.clear();
  vGreenCheck.clear();

  g_pErr->DFO("EventGSC1DrawGrid::GiveFeedback", 0L, 3);

}

int EventGSC1DrawGrid::PostTrial() {
  m_pTemplate->m_vGSC1Toggle.insert(m_pTemplate->m_vGSC1Toggle.end(),
				    m_vGSC1Toggle.begin(), m_vGSC1Toggle.end());
  m_pTemplate->m_vGSC1Cursor.insert(m_pTemplate->m_vGSC1Cursor.end(),
				    m_vGSC1Cursor.begin(), m_vGSC1Cursor.end());
  m_pTemplate->m_vGSC1Stars.insert(m_pTemplate->m_vGSC1Stars.end(),
				    m_vGSC1Stars.begin(), m_vGSC1Stars.end());
  m_pTemplate->m_vGSC1Flags.insert(m_pTemplate->m_vGSC1Flags.end(),
				    m_vGSC1Flags.begin(), m_vGSC1Flags.end());
  return 0;
}
