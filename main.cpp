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
  scene::IAnimatedMesh* levelMesh = smgr->getMesh("20kdm2.bsp");
  scene::ISceneNode* levelNode = 0;
  if (levelMesh) {
    levelNode = smgr->addOctreeSceneNode(levelMesh->getMesh(0), 0, -1, 1024);
//      node = smgr->addMeshSceneNode(mesh->getMesh(0));
  }

  scene::IAnimatedMesh* sydneyMesh = smgr->getMesh("models/sydney.md2");
  if(!sydneyMesh){
    device->drop();
    return(1);
  }
  scene::IMeshSceneNode *cube = smgr->addCubeSceneNode(15.0f, 0, -1, core::vector3df(150,10,10));
  scene::IAnimatedMeshSceneNode* sydneyNode = smgr->addAnimatedMeshSceneNode( sydneyMesh );
  if (sydneyNode) {
    sydneyNode->setMaterialFlag(video::EMF_LIGHTING, false);
    sydneyNode->setFrameLoop(0, 310); 
    sydneyNode->setMaterialTexture( 0, driver->getTexture("models/sydneyMesh.bmp") );
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
