#ifndef SBX_GSC1RESPOBJ_INCLUDED
#define SBX_GSC1RESPOBJ_INCLUDED

#include "RespObj.hpp"
#include "xy.hpp"

class GSC1Locs : public RespObj {
public:
  int m_nRow;
  int m_nCol;
  GSC1Locs(int r, int c) : m_nRow(r), m_nCol(c) {}
};

class GSC1SoundButton : public RespObj {
public:
  Uint32 m_ms;
  int m_id;
  GSC1SoundButton(int id, Uint32 ms) : m_id(id), m_ms(ms) {}
  virtual int StoreResp(long id, Recordset * pRS);
};

class GSC1Toggle : public RespObj {
public:
  Uint32 m_ms;
  xy m_xyCell;
  int m_nFlag;
  GSC1Toggle(Uint32 ms, int r, int c, int f) : m_ms(ms), m_xyCell(c, r), m_nFlag(f) { }
};

class GSC1Cursor : public RespObj {
public:
  Uint32 m_ms;
  xy m_xyCell;
  GSC1Cursor(Uint32 ms, int r, int c) : m_ms(ms), m_xyCell(c, r) { }
};

#endif
