#include "EventLC1Display.hpp"
#include "Template.hpp"
#include "xy.hpp"
#include "StimulusImg.hpp"

EventLC1Display::EventLC1Display(long id, long msec, long idCmd, ArgMMap mmArgs, Template * pTemplate) :
  Event(id, msec, idCmd, mmArgs, pTemplate) {
}

int EventLC1Display::Action() {
  g_pErr->DFI("EventLC1Display::Action", m_id, 4); 

  StimulusPtrMMap::iterator ii;
  pair<StimulusPtrMMap::iterator, StimulusPtrMMap::iterator> rii;
  StimulusImg * pStim = NULL;
  StimulusImg * ppMain[4];
  StimulusImg * ppBack[4];
  vector<string> vName;
  vector<int> vOrd;

  vName.push_back(string("T"));
  vName.push_back(string("C"));
  vName.push_back(string("TC"));
  vName.push_back(string("CC"));

  for (int i = 0; i < 4; i++) {
    vOrd.push_back(i);
  }
  std::random_shuffle(vOrd.begin(), vOrd.end());

  rii = m_pTemplate->m_mmapAllAOI.equal_range("bkgd");

  for (int i = 0; i < 4; i++) {
    ppMain[i] = (StimulusImg *) m_pTemplate->m_mmapAllAOI.find(vName[i].c_str())->second.get();
  }
  
  for (int i = 0; i < 4; i++) {
    for (ii = rii.first; ii != rii.second; ii++) {
      pStim = (StimulusImg *) ii->second.get();
      if ( (ppMain[i]->m_x1 == (pStim->m_x1 + 2)) &&
	   (ppMain[i]->m_y1 == (pStim->m_y1 + 2)) ) {
	ppBack[i] = pStim;
	break;
      } else {}
    }
  }

  if (!ppMain[0] || !ppMain[1] || !ppMain[2] || !ppMain[3] ||
      !ppBack[0] || !ppBack[1] || !ppBack[2] || !ppBack[3] ) {
    g_pErr->Report("something's wrong");
  } else {
  }

  int pnX[4] = {44, 289, 534, 779};

  for (int i = 0; i < 4; i++) {
    ppMain[vOrd[i]]->m_CurX.Set(pnX[i]);
    ppMain[vOrd[i]]->m_CurY.Set(284);
    ppBack[vOrd[i]]->m_CurX.Set(pnX[i]-2);
    ppBack[vOrd[i]]->m_CurY.Set(282);
    ppBack[vOrd[i]]->Draw(false);
    ppMain[vOrd[i]]->Draw(false);
  }
  Display_SDL::Flip1();

  // TO DO : change this to "pump_delay"
  SDL_Delay(3000);

  int nFrames = 40;
  Uint32 msDelay = 25;
  int nStepsLeft = 0;

  for (int i = 1; i <= nFrames; i++) {
    nStepsLeft = nFrames - i;
    for (int j = 0; j < 4; j++) {
      ppBack[vOrd[j]]->Erase(false);
    }
    for (int j = 0; j < 4; j++) {
      ppMain[vOrd[j]]->m_CurX.Set(pnX[j]+((float)i/nFrames)*(ppMain[vOrd[j]]->m_x1-pnX[j]));
      ppMain[vOrd[j]]->m_CurY.Set(284+((float)i/nFrames)*(ppMain[vOrd[j]]->m_y1-284));
      ppBack[vOrd[j]]->m_CurX.Set(pnX[j]-2+((float)i/nFrames)*(ppBack[vOrd[j]]->m_x1-pnX[j]+2));
      ppBack[vOrd[j]]->m_CurY.Set(282+((float)i/nFrames)*(ppBack[vOrd[j]]->m_y1-282));
      ppBack[vOrd[j]]->Draw(false);
      ppMain[vOrd[j]]->Draw(false);
    }
    Display_SDL::Flip1();
    // TO DO : change this to "pump_delay"
    SDL_Delay(msDelay);
  }

  vName.clear();

  g_pErr->DFO("EventLC1Display::Action", m_id, 4); 


  return Event::Action();
}
