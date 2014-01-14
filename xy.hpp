#ifndef SBX_INCLUDED_XY
#define SBX_INCLUDED_XY

#include <utility>

class xy {
private:
public:
  xy(int x=0,int y=0) : m_x(x), m_y(y) {}
  inline bool operator==(const xy &other) const { return ( (m_x==other.m_x) && (m_y==other.m_y) ); }
  inline bool operator<(const xy& rhs) const { return (toScalar() < rhs.toScalar() ); }
  inline int toScalar() const { return ( (m_y-1)*3 + m_x ); }

  int m_x;
  int m_y;
};

xy Loc2Grid(xy xyLoc, xy xy0, int g);

#endif
