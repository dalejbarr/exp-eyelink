#include "xy.hpp"

xy Loc2Grid(xy xyLoc, xy xy0, int g) {
  xy result;
  result.m_x = (xyLoc.m_x - xy0.m_x)/g + 1;
  result.m_y = (xyLoc.m_y - xy0.m_y)/g + 1;
  return result;
}
