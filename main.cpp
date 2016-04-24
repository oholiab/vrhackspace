#include <irrlicht/irrlicht.h>
#include <irrlicht/driverChoice.h>

//#pragma comment(lib, "Irrlicht.lib")

using namespace irr;
//using namespace core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;
//
enum {
  // I use this ISceneNode ID to indicate a scene node that is
  // not pickable by getSceneNodeAndCollisionPointFromRay()
  ID_IsNotPickable = 0,

  // I use this flag in ISceneNode IDs to indicate that the
  // scene node can be picked by ray selection.
  IDFlag_IsPickable = 1 << 0,

  // I use this flag in ISceneNode IDs to indicate that the
  // scene node can be highlighted.  In this example, the
  // homonids can be highlighted, but the level mesh can't.
  IDFlag_IsHighlightable = 1 << 1
};


int main() {
  // Uses driverChoiceConsole() from driverChoice.h
  IrrlichtDevice *device = createDevice(driverChoiceConsole(), core::dimension2d<u32>(512, 384), 16, false, false, false, 0);
  device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

  // set up video driver, scene manager and gui environment
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();
  gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

  // Create the level and the collision selector
  device->getFileSystem()->addFileArchive("models/map-20kdm2.pk3");
  scene::IAnimatedMesh* levelMesh = smgr->getMesh("20kdm2.bsp");
  scene::IMeshSceneNode* levelNode = 0;
  if (levelMesh) {
    scene::ITriangleSelector* selector = 0;
    levelNode = smgr->addOctreeSceneNode(levelMesh->getMesh(0), 0, IDFlag_IsPickable);
//      node = smgr->addMeshSceneNode(mesh->getMesh(0));
    if(levelNode){
      levelNode->setPosition(core::vector3df(-1350,-130,-1400));
      selector = smgr->createOctreeTriangleSelector(
                 levelNode->getMesh(), levelNode, 128);
      levelNode->setTriangleSelector(selector);
    }
  }

  scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS();
  device->getCursorControl()->setVisible(false);
  //camera->setTarget(cube->getAbsolutePosition());
  int lastFPS = -1;
  while(device->run()) {
    if(device->isWindowActive()){
      //int fps = driver->getFPS();
      //core::stringw status;
      //if(fps != lastFPS){
      //  status = L"grimmwa.re VR hackspace mkI [";
      //  status += driver->getName();
      //  status += "] FPS::";
      //  status += fps;
      //  lastFPS = fps;
      //}
      //font->draw(status, core::rect<s32>(10,10,200,22), video::SColor(255,255,255,255));

      driver->beginScene(true, true, video::SColor(255,100,101,140));
      smgr->drawAll();
      guienv->drawAll();
      driver->endScene();
    }
  }
  device->drop();
  return 0;

}
