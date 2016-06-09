#ifndef SBX_GLOBAL_INCLUDED
#define SBX_GLOBAL_INCLUDED

#include "Recordset.hpp"
#include "Device.hpp"
#include "EXPConfig.hpp"
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

#define SBX_ABORT_EXPERIMENT 2
#define SBX_NO_EVENT 3
#define SBX_EVENT 4
#define SBX_END_EXPERIMENT 5
#define SBX_CONTINUE 6
#define SBX_CRITICAL_ERROR 7

#define SBX_INITIALIZED 20
#define SBX_PREPARED 21
#define SBX_IN_PROGRESS 22
#define SBX_FINISHED 23
#define SBX_DEALLOCATED 24

// declared in file Experiment.cpp
extern Recordset * g_prsStim;
extern Recordset * g_prsResp;
extern ReportError * g_pErr;
extern EXPConfig * g_pConfig;

Device * FindOrCreateDevice(long lgType, long id);
string CheckSlashes(const char *pc);
string CheckSlashes(string s);

Uint32 ClockFn() ;

#endif
