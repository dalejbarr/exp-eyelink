#include "Experiment.hpp"
#include "EventSetFlag.hpp"
#include <sstream>
#include <boost/lexical_cast.hpp>
using std::ostringstream;

EventSetFlag::EventSetFlag(long idEvent, long msec, long idCmd, 
			 ArgMMap mmArgs, Template * pTemplate) :
  Event(idEvent, msec, idCmd, mmArgs, pTemplate) {
  g_pErr->DFI("EventSetFlag::EventSetFlag", idEvent, 3);

  m_pTemplate = pTemplate;

  pair<ArgIter, ArgIter> pii;
  ArgMMap::iterator ii;

  string s;
  pii = mmArgs.equal_range("Bit");
  if (pii.first != pii.second) {
    istringstream iss((*pii.first).second);    
    iss >> m_nFlag;
  } else {
    g_pErr->Report("need to specify argument 'Bit' for EventSetFlag");
  }

  g_pErr->Debug(pastestr::paste("sd", " ",
				"will set bit", (long) m_nFlag));
  
  g_pErr->DFO("EventSetFlag::EventSetFlag", idEvent, 3);
}

int EventSetFlag::Action() {
  g_pErr->DFI("EventSetFlag::Action", ID(), 3);

  Experiment::g_bsFlag.set(m_nFlag, true);

  std::string bstr =
    Experiment::g_bsFlag.to_string<char,
				   std::string::traits_type,
				   std::string::allocator_type>();
  
  g_pErr->Debug(pastestr::paste("ss", " ",
				"bitstring is now", bstr.c_str()));

  g_pErr->DFO("EventSetFlag::Action", ID(), 3);
  return Event::Action();
}
