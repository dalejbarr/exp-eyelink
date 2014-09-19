#include "global.hpp"
#ifdef EXP_EYELINK_VERS
#include "eyelink.h"
#endif

Uint32 ClockFn() {
#ifdef EXP_EYELINK_VERS
  return current_msec();
#else
  return SDL_GetTicks();
#endif
}
