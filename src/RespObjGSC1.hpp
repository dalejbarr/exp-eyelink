#ifndef SBX_RESPOBJGSC1_INCLUDED
#define SBX_RESPOBJGSC1_INCLUDED

#include "RespObj.hpp"
#include "xy.hpp"

class RespObjGSC1 : public RespObj {
public:
  vector<xy> m_vBombLocs;
  vector<xy> m_vFlagLocs;
  RespObjGSC1();
  virtual int StoreResp(Recordset * pRS);
};

#endif
