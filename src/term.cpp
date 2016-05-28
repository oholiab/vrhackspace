#include "term.h"
#include <string.h>
#include <unistd.h>

// We want to either turn term into an extension of iscenenode with a couple of
// extra parameters (terminal id and the like) and extra methods (initialiser
// to spin up the container/vm) or we want to create a new object or struct with
// the pointer to the iscenenode as an attribute

Terminal::Terminal(scene::ISceneManager* smgr):
  dispNum(getNextAvailableXDisplayNumber())
{
  // Set up terminal
  scene::IMeshSceneNode *terminal = smgr->addCubeSceneNode(15.0f, 0, IDFlag_IsInteractable, core::vector3df(10,-10,10), core::vector3df(0,0,0), core::vector3df(4, 4, 1));
  terminal->setMaterialFlag(video::EMF_LIGHTING, false); // because monitors are light sources
  terminal->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
  terminal->setTriangleSelector( smgr->createTriangleSelector( terminal->getMesh(), terminal ));
  scene::SMesh* outline = smgr->getMeshManipulator()->createMeshCopy(terminal->getMesh());
  smgr->getMeshManipulator()->flipSurfaces(outline);
  outline->setMaterialFlag(video::EMF_LIGHTING, false);
  smgr->getMeshManipulator()->recalculateNormals(outline);
  scene::IMeshSceneNode *outlineNode = smgr->addMeshSceneNode(outline, terminal, ID_IsNotPickable | IDFlag_IsOutline, core::vector3df(0,0,0), core::vector3df(0,0,0), core::vector3df(1.05,1.05,1.05));
  node = terminal;

  int dispNumDigits = 0;
  if(dispNum == 0){
    dispNumDigits = 1;
  }else{
    dispNumDigits = floor(log10(dispNum)) + 1;
  }

  dispName  = (char*)malloc((dispNumDigits + 2) * sizeof(char));
  sprintf(dispName, ":%d", dispNum);

  char buf[1024];
  sprintf(buf, "docker run -d -v /tmp/vrhs:/vrhs -v /tmp/.X11-unix:/tmp/.X11-unix --name vrhs_term_%d vrhs /screenme.sh /vrhs/shot.png %s\n",
      dispNum,
      dispName);
  launchCommand = (char*)malloc(strlen(buf) + 1);
  strcpy(launchCommand, buf);
  sprintf(buf, "docker kill vrhs_term_%d\n", dispNum);
  stopCommand = (char*)malloc(strlen(buf) + 1);
  strcpy(stopCommand, buf);
  printf("%s\n",stopCommand);
  sprintf(buf, "docker rm vrhs_term_%d\n", dispNum);
  rmCommand = (char*)malloc(strlen(buf) + 1);
  strcpy(rmCommand, buf);

  int exit = system(launchCommand);
  if(0 != exit){
    printf("command failed: %s\n", launchCommand);
  }

  //FIXME: definitely not ideal
  sleep(5);

  X11Display aDisp(dispName);
  display = &aDisp;
}

Terminal::~Terminal(){
  int exit = system(stopCommand);
  if(0 != exit){
    printf("command failed: %s\n", stopCommand);
  }
  exit = system(rmCommand);
  if(0 != exit){
    printf("command failed: %s\n", rmCommand);
  }
}

X11Display* Terminal::getDisp()
{
  X11Display aDisp(dispName);
  if(aDisp.display != NULL){
    display = &aDisp;
  }
  return display;
}
