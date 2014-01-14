#include "Display.hpp"

Display::Display(long id) : Device(id) {
}

Display::~Display() {
}

int Display::Draw() {
  for (int i = 0; i < m_vpStim.size(); i++) {
    m_vpStim[i]->Action();
  }
}
