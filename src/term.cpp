#include "term.h"

// We want to either turn term into an extension of iscenenode with a couple of
// extra parameters (terminal id and the like) and extra methods (initialiser
// to spin up the container/vm) or we want to create a new object or struct with
// the pointer to the iscenenode as an attribute

scene::IMeshSceneNode* addTerminal(scene::ISceneManager* smgr){
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
}
