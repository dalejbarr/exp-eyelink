#ifndef SBT_OPERATION_INCLUDED
#define SBT_OPERATION_INCLUDED

#include <vector>
using std::vector;

#include <string>
using std::string;

#include <map>
using std::map;
using std::pair;

class Operation {
private:
  int m_nSign;
  Operation * m_pOp;
public:
  Operation(int nSign = 1) : m_nSign(nSign), m_pOp(NULL) {}
  Operation(Operation * po, int nSign) : m_nSign(nSign), m_pOp(po) {}
  virtual ~Operation() {}
  virtual int Get() { if (m_pOp) { return (m_nSign * (m_pOp->Get())); } else {return 0;} }
  virtual void Set(Operation * po) { m_pOp = po; }
  virtual void Set(Operation * po, int nSign) { m_pOp = po; m_nSign = nSign; }
  virtual void Set(int n, int nSign = 1) {}; 
  int GetSign() {return m_nSign;}
};

typedef map<string, Operation *> OpPtrMap;
typedef vector<Operation *> OpPtrVec;

class OpInt : public Operation {
protected:
  int m_n;
public:
  OpInt(int n = 0, int nSign = 1) : m_n(n*nSign) {}
  virtual int Get() { return m_n; }
  virtual void Set(int n, int nSign = 1) { m_n = n * nSign; }
  void Set(Operation * po, int nSign) { m_n = po->Get() * nSign; }
};

class OpChain : public Operation {
protected:
  int m_nSign;
  OpPtrVec m_oplist;
  void NewOp(string s);
public:
  static OpPtrMap g_mapUnassignedPtrs;
  OpChain() {};
  virtual ~OpChain();
  OpChain(Operation * po) { m_oplist.push_back(po); }
  OpChain(int n, int nSign = 1) { Set(n, nSign); }
  OpChain(string s, int nSign = 1) { Set(s, nSign); }
  OpChain(const char *pc, int nSign = 1) { Set(pc, nSign); }
  OpChain(OpChain * po, int nSign = 1) { Set(po, nSign); }
  virtual void Set(Operation * po, int nSign = 1);
  virtual void Set(string s, int nSign = 1);
  virtual void Set(const char *pc, int nSign = 1) { Set(string(pc)); }
  virtual void Set(int n, int nSign = 1);
  virtual void Set() {}
  void SetSign(int nSign) { m_nSign = nSign; }
  void Cleanup();
  virtual int Get();
};

#endif
