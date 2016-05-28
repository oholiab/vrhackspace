#ifndef IRR_INCLUDE
#include <irrlicht/irrlicht.h>
#define IRR_INCLUDE
#endif
#ifndef IDS_INCLUDE
#include "ids.h"
#define IDS_INCLUDE
#endif
#ifndef CONTROLTERM_INCLUDE
#include "controlterm.h"
#define CONTROLTERM_INCLUDE
#endif

using namespace irr;
class Terminal {
  char* launchCommand;
  char* stopCommand;
  char* rmCommand;
  public:
    Terminal(scene::ISceneManager* smgr);
    ~Terminal();
    scene::IMeshSceneNode* node;
    //FIXME: should be independent of platform
    int dispNum = -1;
    char* dispName = NULL;
    X11Display* display = NULL;
  private:
};
