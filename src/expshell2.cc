#include "Experiment.hpp"
#include "global.hpp"

#include "Display_SDL.hpp"
#include "Audio_SDL.hpp"
//#include "SDL_ttf.h"

#include <stdlib.h>
#include <string.h>

#include <iostream>
using std::cout;
using std::endl;

int parseArgs(int argc, char **);
void usage(char *);

Experiment * g_pExperiment = NULL;
string expdb("");
char dbDriver[32] = "";
char pcMode[32] = "auto";
bool g_bFullScreen = 0;
bool g_bShowSession = 0;
int g_nMainResult = EXP_IN_PROGRESS;
bool g_bResume = false;

int exp_init(int argc, char * argv[]) {
	std::cout << "initializing SDL" << std::endl;
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_JOYSTICK) == -1) {
    g_pErr->Report("Could not initialize Simple DirectMedia Layer.");
  } else {}
	
	int flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if (IMG_Init(flags) != flags) {
		g_pErr->Report("image support not built in :(");
	} else {}

  if (parseArgs(argc, argv)==1) {
    exit(1);
  } else {}

  //g_pExperiment = new Experiment(expdb.c_str(), dbDriver, pcMode);
  g_pExperiment = new Experiment();
  g_pExperiment->InitializeDB(expdb.c_str(), dbDriver);
  g_pExperiment->InitializeExp(pcMode, g_bResume);  
  g_pExperiment->Prepare(NULL, g_bFullScreen);
  //g_pExperiment->Initialize(argv[1], argv[2]);

  if (g_bShowSession) {
    //string sMsg();
    g_pExperiment->Message(pastestr::paste("sd", "", "Your session ID is ", g_pExperiment->GetSessionID()).c_str());
    g_pExperiment->WaitKey();
		Display_SDL::ClearScreen();
  } else {
  }

  return 0;
}

int exp_cleanup() {
  //delete g_pDisplay;
  //delete g_pAudio;
  g_pErr->Debug("cleanup");
  g_pExperiment->Message("storing data... please wait.");
  g_pExperiment->StoreTrialData();
  if (g_nMainResult == EXP_COMPLETED) {
    g_pExperiment->UpdateSessionStatus("COMPLETED");
  } else {
    g_pExperiment->UpdateSessionStatus("ABORTED");
  }
  //g_pExperiment->Message("Press any key to exit.");
  //g_pExperiment->WaitKey();

  delete g_pExperiment;
  if (!(g_nMainResult == SBX_ABORT_EXPERIMENT)) {
    SDL_Quit();
  } else {}
	IMG_Quit();
}

int main(int argc, char *argv[]){
  //int nMainResult = 0;
  int nResult = 0;
  char fname[1024];

  exp_init(argc, argv);

  // loop for single trial
  int nTrials = 1;
  while ( (g_nMainResult = g_pExperiment->PrepareNextTrial()) == EXP_TRIAL_READY) {
    g_pExperiment->StartTrial();
    while( (nResult = g_pExperiment->Update()) == SBX_CONTINUE) ;
    if (nResult == SBX_END_EXPERIMENT) {
      g_nMainResult = SBX_END_EXPERIMENT;
      break;
    } else {
      g_pExperiment->FinishTrial();
			Display_SDL::ClearScreen();
    }

    /*
    if (nResult == SBX_END_EXPERIMENT) {
      g_pExperiment->FinishTrial();
      break;
    } else {
    } 
    */
   // screen shot
    //sprintf(fname, "screenshots/%02d.bmp", nTrials++);
    //SDL_SaveBMP(g_pDisplay->GetSDLScreen(), fname);
    //g_pExperiment->FinishTrial();
    //g_pExperiment->NextTrial();
  }

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
      else if (strcmp(argv[i],"-resume") ==0 )
	{
	  g_bResume = true;
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
