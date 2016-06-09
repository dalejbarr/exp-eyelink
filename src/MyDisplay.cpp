#include "MyDisplay.hpp"

MyDisplay::MyDisplay(long id) : Device(id) {
}

MyDisplay::~MyDisplay() {
}

int MyDisplay::Draw() {
  for (int i = 0; i < m_vpStim.size(); i++) {
    m_vpStim[i]->Action();
  }
}
