#include "Operation.hpp"
//#include <iostream>
//using std::cout;
//using std::endl;
#include <cstdlib>
using std::abs;

OpPtrMap OpChain::g_mapUnassignedPtrs;

int OpChain::Get() {
  OpPtrVec::iterator ii;
  int nResult = 0;

  for (ii = m_oplist.begin(); ii != m_oplist.end(); ii++) {
    nResult += (*ii)->Get();
  }

  return m_nSign * nResult;
}

OpChain::~OpChain() {
  Cleanup();
}

void OpChain::Set(string s, int nSign) {
  Cleanup();

  m_nSign = nSign;
  string delimiters("+-");
  string::size_type pos0 = 0;
  string::size_type pos1 = s.find_first_of(delimiters, 1);

  OpChain::g_mapUnassignedPtrs.clear();

  while (pos1 != string::npos) {

    if (pos0 == 0) {
      NewOp(s.substr(pos0, (pos1-pos0)));
    } else {
      NewOp(s.substr(pos0-1, (pos1-pos0)+1));
    }
    pos0 = pos1; pos0++;
    pos1 = s.find_first_of(delimiters, pos0);
  }

  NewOp(s.substr(pos0 > 0 ? pos0-1 : 0, pos1-pos0));
}

void OpChain::NewOp(string s) {

  string sOrig = s;
  int nSign = 1;
  Operation * po = NULL;

  if (s == "") {
    return;
  } else {
    //cout << s << endl;
  }

  if (s[0] == '-') {
    nSign = -1;
  } else {
    nSign = 1;
  }

  if ((s[0] == '+') || (s[0] == '-')) {
    s = s.substr(1, s.size()-1);
  } else {}

  string nums("0123456789");
  if (s.find_first_not_of(nums) != string::npos) {
    po = new Operation(nSign);
    //cout << "sign is now " << po->GetSign() << endl;
    OpChain::g_mapUnassignedPtrs.insert(pair<string, Operation *>(s, po));
  } else {
    po = (Operation *) new OpInt(atoi(s.c_str()), nSign);
  }

  m_oplist.push_back(po);
}

void OpChain::Cleanup() {
  OpPtrVec::iterator ii;

  for (ii = m_oplist.begin(); ii != m_oplist.end(); ii++) {
    delete (*ii);
  }

  m_oplist.clear();  
}

void OpChain::Set(int n, int nSign) {
  Cleanup();

  Operation * po = (Operation *) new OpInt(n, 1);
  m_nSign = nSign;

  m_oplist.push_back(po);
}

void OpChain::Set(Operation * po, int nSign) {
  Cleanup();

  //cout << "here" << endl;
  Operation * pon = new Operation(po, 1);
  m_nSign = nSign;

  m_oplist.push_back(pon);
}

