#ifndef IRR_INCLUDE
#include <irrlicht/irrlicht.h>
#define IRR_INCLUDE
#endif
#ifndef IDS_INCLUDE
#include "ids.h"
#define IDS_INCLUDE
#endif

using namespace irr;
class Terminal {
  public:
    Terminal(scene::ISceneManager* smgr);
    ~Terminal();
    scene::IMeshSceneNode* node;
};
