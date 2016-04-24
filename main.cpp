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
  gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
  //gui::IGUIFont* font2 = device->getGUIEnvironment()->getFont("../../media/fonthaettenschweiler.bmp");
  if(!font){
    device->drop();
    return(1);
  }
  font->draw(L"I'm a little teapot", core::rect<s32>(10,10,200,22), video::SColor(255,255,255,255));
  guienv->addStaticText(L"Hello World! This is the Irrlicht Software engine!",
         core::rect<int>(10,10,200,22), true);
  device->getFileSystem()->addFileArchive("models/map-20kdm2.pk3");
  scene::IAnimatedMesh* sydney = smgr->getMesh("models/sydney.md2");
  if(!sydney){
    device->drop();
    return(1);
  }
  scene::IMeshSceneNode *cube = smgr->addCubeSceneNode(15.0f, 0, -1, core::vector3df(150,10,10));
  scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( sydney );
  if (node) {
    node->setMaterialFlag(video::EMF_LIGHTING, false);
    node->setFrameLoop(0, 310); 
    node->setMaterialTexture( 0, driver->getTexture("models/sydney.bmp") );
  }
  //smgr->addLightSceneNode();
  //scene::ICameraSceneNode *camera = smgr->addCameraSceneNode(0, core::vector3df(0,30,-40), core::vector3df(0,5,0));
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
