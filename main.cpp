#include <irrlicht/irrlicht.h>
#include <irrlicht/driverChoice.h>

//#pragma comment(lib, "Irrlicht.lib")

using namespace irr;
//using namespace core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;

int main() {
  // Uses driverChoiceConsole() from driverChoice.h
  IrrlichtDevice *device = createDevice(driverChoiceConsole(), core::dimension2d<u32>(512, 384), 16, false, false, false, 0);
  device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();
  gui::IGUIEnvironment* guienv = device->getGUIEnvironment();
  guienv->addStaticText(L"Hello World! This is the Irrlicht Software engine!",
         core::rect<int>(10,10,200,22), true);
  scene::IAnimatedMesh* mesh = smgr->getMesh("models/sydney.md2");
  if(!mesh){
    device->drop();
    return(1);
  }
  scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );
  if (node) {
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setFrameLoop(0, 310); 
    node->setMaterialTexture( 0, driver->getTexture("models/sydney.bmp") );
  }
  smgr->addCameraSceneNode(0, core::vector3df(0,30,-40), core::vector3df(0,5,0));
  while(device->run()) {
    driver->beginScene(true, true, video::SColor(255,100,101,140));
    smgr->drawAll();
    guienv->drawAll();
    driver->endScene();
  }
  device->drop();
  return 0;

}
