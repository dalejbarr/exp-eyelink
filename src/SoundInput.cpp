#include "SoundInput.hpp"

SoundInput::SoundInput(unsigned long ulDev) : InputDev(ulDev) {
	Initialize();
}

SoundInput::~SoundInput() {
	Destroy();
}

void SoundInput::Initialize() {
}

void SoundInput::Destroy() {
}
