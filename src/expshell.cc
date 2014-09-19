#include "Experiment.hpp"
#include "global.hpp"

#include "Display_SDL.hpp"
#include "Audio_SDL.hpp"
//#include "SDL_ttf.h"

#include <iostream>
using std::cout;
using std::endl;

Experiment * g_pExperiment = NULL;

int exp_init(int argc, char * argv[]) {
  g_pExperiment = new Experiment(argc, argv);
  //g_pExperiment->SequenceItems();

  //if (SDL_Init(SDL_INIT_TIMER|SDL_INIT_AUDIO|SDL_INIT_VIDEO|SDL_INIT_JOYSTICK) == -1) {
  if (SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER |
							 SDL_INIT_EVENTS) == -1) {
    g_pErr->Report("Could not initialize Simple DirectMedia Layer.");
  } else {}

  //g_pDisplay = new Display_SDL();
  //g_pDisplay->CreateScreen(896, 88, 2048, 768, SDL_DOUBLEBUF);
  //g_pDisplay->CreateScreen(0, 0, 1024, 768, SDL_DOUBLEBUF);

  //g_pAudio = new Audio_SDL();

  // uncomment if you want to use true type fonts
  /*
  if( TTF_Init() == -1 ) { 
    g_pErr->Report("could not initialize SDL true type font library");
    return -1; 
  }
  */

  //Initialize SDL_mixer 

  return 0;
}

int exp_cleanup() {
  //delete g_pDisplay;
  //delete g_pAudio;
  delete g_pExperiment;
}

int main(int argc, char *argv[]){

  exp_init(argc, argv);
  g_pExperiment->Run();
  exp_cleanup();

  return 0;
}
