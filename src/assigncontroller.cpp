#include "Experiment.hpp"
#include "global.hpp"

#include "Display_SDL.hpp"
#include "Audio_SDL.hpp"
#include "Template.hpp"
//#include "SDL_ttf.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>
using std::cout;
using std::endl;

Experiment * g_pExperiment = NULL;
string expdb("");
char dbDriver[32] = "";
char pcMode[32] = "auto";
bool g_bFullScreen = 0;
bool g_bShowSession = 0;
int g_nMainResult = EXP_IN_PROGRESS;
//Template g_template(0, 0, "test");

int parseArgs(int argc, char **argv);
void usage(char * progname);

int poll_event() {
  static SDL_Event event;
  static int nResult = SBX_NO_EVENT;
  char charinfo[1024];
  SDL_QuitEvent eq;
  std::string msg;

  if (SDL_PollEvent(&event)==1) {
    nResult = SBX_EVENT;
    switch (event.type) {
    case SDL_QUIT :
      nResult = SBX_ABORT_EXPERIMENT;
      break;
    case SDL_KEYDOWN :
      {
	if ((event.key.keysym.mod & KMOD_CTRL) && (event.key.keysym.sym==99)) {
	  g_pErr->Debug("break pressed");
	  nResult = SBX_ABORT_EXPERIMENT;
	} else {
	  sprintf(charinfo, "key %s (%d) pressed", SDL_GetKeyName(event.key.keysym.sym), event.key.keysym.sym);
	  g_pErr->Debug(charinfo);
	}
      }
      break;
    case SDL_JOYAXISMOTION :      
      msg.assign(pastestr::paste("sddd", " ", "motion on", (long) event.jaxis.which,
				 (long) event.jaxis.axis,
				 (long) event.jaxis.value));
      if (event.jaxis.value != 0) {
	g_pExperiment->Message(msg.c_str());
      } else {}
      break;
    case SDL_JOYBUTTONDOWN :
      msg.assign(pastestr::paste("sdd", " ", "button on", (long) event.jbutton.which,
				 (long) event.jbutton.button));
      g_pExperiment->Message(msg.c_str());
      break;
    default :
      break;
    }
  } else {}

  return(nResult);
}

int exp_init(int argc, char * argv[]) {

  if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
    g_pErr->Report("Could not initialize Simple DirectMedia Layer.");
  } else {}

  /*
  if (parseArgs(argc, argv)==1) {
    exit(1);
  } else {}
  */

  //g_pExperiment = new Experiment(expdb.c_str(), dbDriver, pcMode);
  g_pExperiment = new Experiment();
  //g_pExperiment->InitializeDB(expdb.c_str(), dbDriver);
  //g_pExperiment->InitializeExp(pcMode);
  g_pExperiment->Prepare(NULL, g_bFullScreen);

  g_pErr->Debug("Finished exp_init");

  return 0;
}

int exp_cleanup() {
  g_pErr->Debug("cleanup");
  /*
  g_pExperiment->Message("storing data... please wait.");
  g_pExperiment->StoreTrialData();
  if (g_nMainResult == EXP_COMPLETED) {
    g_pExperiment->UpdateSessionStatus("COMPLETED");
  } else {
    g_pExperiment->UpdateSessionStatus("ABORTED");
  }
  //g_pExperiment->Message("Press any key to exit.");
  //g_pExperiment->WaitKey();
  */

  delete g_pExperiment;
  SDL_Quit();
}

int main(int argc, char *argv[]){
  exp_init(argc, argv);
  SDL_Joystick * stick1 = NULL;
  SDL_Joystick * stick2 = NULL;
  int nEvent;

  if (SDL_NumJoysticks() < 2) {
    g_pErr->Debug(pastestr::paste("sds", " ", "only", (long) SDL_NumJoysticks(), "found ;need 2 to play"));
  } else {
    g_pErr->Debug(pastestr::paste("ds", " ", (long) SDL_NumJoysticks(), "joysticks found"));
    stick1 = SDL_JoystickOpen(0);
    stick2 = SDL_JoystickOpen(1);

    while(1) {
      
      nEvent = poll_event();
      if (nEvent == SBX_ABORT_EXPERIMENT) {
	break;
      } else {}
    }

    SDL_JoystickClose(stick1);
    SDL_JoystickClose(stick2);
  }


  // loop for single trial
  /*
  int nTrials = 1;
  while ( (g_nMainResult = g_pExperiment->PrepareNextTrial()) == EXP_TRIAL_READY) {
    g_pExperiment->StartTrial();
    while( (nResult = g_pExperiment->Update()) == SBX_CONTINUE) ;
    if (nResult == SBX_END_EXPERIMENT) {
      g_nMainResult = SBX_END_EXPERIMENT;
      break;
    } else {
      g_pExperiment->FinishTrial();
    }
  }
  */

  if (g_nMainResult == EXP_COMPLETED) {
    g_pErr->Debug("finished experiment");
  } else {
    g_pErr->Debug("aborted experiment");
  }

  exp_cleanup();

  return 0;
}

void usage(char * progname) {
  printf("usage %s \n", progname);
  printf("\t options: \n");
  printf("\t[-expdb         name of database] eg. Experiment \n");
  printf("\t[-dbdriver       database driver] eg. sqlite OR MySQL \n");
  printf("\t[-session  how to choose session] eg. auto OR top OR [ID] \n");
}

int parseArgs(int argc, char **argv)
{
  int i =0;
  if (argc < 2) {
    usage(argv[0]);
  }
  for( i =1; i < argc; i++)
    {
      if (strcmp(argv[i],"-expdb") ==0 && argv[i+1])
	{
	  i++;
	  expdb.assign(argv[i]);
	    //sprintf(expdb, "%s", argv[i]);
	}
      else if (strcmp(argv[i],"-dbdriver") ==0 && argv[i+1]) 
	{
	  i++;
	  strcpy(dbDriver, argv[i]);
	}
      else if (strcmp(argv[i],"-session") ==0 && argv[i+1]) 
	{
	  i++;
	  strcpy(pcMode, argv[i]);
	}
      else if (strcmp(argv[i],"-fullscreen") ==0 )
	{
	  g_bFullScreen = 1;
	}
      else if (strcmp(argv[i],"-showsession") ==0 )
	{
	  g_bShowSession = 1;
	}
      else
	{
	  usage(argv[0]);
	  return 1;
	}

    }

  if ( strcmp(expdb.c_str(),"")==0 ) {
    printf("need to specify experiment database using -expdb\n");
    return 1;
  } else {
  }

  return 0;
}
