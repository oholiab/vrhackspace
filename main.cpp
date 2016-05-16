#include <irrlicht/irrlicht.h>
#include <irrlicht/driverChoice.h>
#include "controlterm.cpp"

const char* disp = ":1";

//#pragma comment(lib, "Irrlicht.lib")

using namespace irr;
//using namespace core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;
//
enum {
  ID_IsNotPickable = 0,

  IDFlag_IsSolid = 1 << 0,

  IDFlag_IsInteractable = 1 << 1
};

X11Display xdisp(disp);

class VREventReceiver : public IEventReceiver {
  public:
    virtual bool OnEvent(const SEvent& event){
      if(event.EventType == irr::EET_KEY_INPUT_EVENT){
        if(IsModKey(event.KeyInput.Key)){
          if(event.KeyInput.PressedDown){
            CurrentMod |= mapKeyCode(event.KeyInput.Key);
          } else {
            CurrentMod &= ~mapKeyCode(event.KeyInput.Key);
          }
        } else {
          KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
          xdisp.sendKeyEvent(mapKeyCode(event.KeyInput.Key), event.KeyInput.PressedDown, CurrentMod);
        }
      }
      return false;
    }
    virtual bool IsKeyDown(EKEY_CODE keyCode) const {
      return KeyIsDown[keyCode];
    }
  private:
    bool KeyIsDown[KEY_KEY_CODES_COUNT];
    // FIXME: can initialise to nonzero
    int CurrentMod;
};

int main() {
  // Uses driverChoiceConsole() from driverChoice.h

  VREventReceiver receiver;
  IrrlichtDevice *device = createDevice(driverChoiceConsole(), core::dimension2d<u32>(512, 384), 16, false, false, false, &receiver);
  device->setWindowCaption(L"Grimmware VRHackspace::Mk I");

  // set up video driver, scene manager and gui environment
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();
  gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

  video::SMaterial selectMaterial;
  selectMaterial.Wireframe = true;

  scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
  bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
  bill->setMaterialTexture(0, driver->getTexture("media/particle.jpg"));
  bill->setMaterialFlag(video::EMF_LIGHTING, false);
  bill->setMaterialFlag(video::EMF_ZBUFFER, false);
  bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
  bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it

  scene::IMeshSceneNode *cube = smgr->addCubeSceneNode(15.0f, 0, -1, core::vector3df(10,-10,10), core::vector3df(0,0,0), core::vector3df(4, 4, 1));
  cube->setMaterialFlag(video::EMF_LIGHTING, false);
  if(cube){
    scene::ITriangleSelector* selector = smgr->createTriangleSelectorFromBoundingBox( cube );
    cube->setTriangleSelector( selector );
    selector->drop();
  }
  scene::ITriangleSelector* cubeSelector = cube->getTriangleSelector();

  // Create the level and the collision levelSelector
  device->getFileSystem()->addFileArchive("models/map-20kdm2.pk3");
  scene::IAnimatedMesh* levelMesh = smgr->getMesh("20kdm2.bsp");
  scene::IMeshSceneNode* levelNode = 0;
  scene::ITriangleSelector* levelSelector = 0;
  scene::IMetaTriangleSelector* cameraCollisionSelector = 
    smgr->createMetaTriangleSelector();
  if (levelMesh) {
    levelNode = smgr->addOctreeSceneNode(levelMesh->getMesh(0), 0, IDFlag_IsSolid);
//      node = smgr->addMeshSceneNode(mesh->getMesh(0));
    if(levelNode){
      levelNode->setPosition(core::vector3df(-1350,-130,-1400));
      levelSelector = smgr->createOctreeTriangleSelector(
                 levelNode->getMesh(), levelNode, 128);
      cameraCollisionSelector->addTriangleSelector(levelSelector);
      cameraCollisionSelector->addTriangleSelector(cubeSelector);
      levelNode->setTriangleSelector(levelSelector);
    }
  }

  //Create camera and pin it to the floor
  scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
  camera->setPosition(core::vector3df(50,50,-60));

  //Add collision to the camera
  if (levelSelector) {
    scene::ISceneNodeAnimator* levelAnim = smgr->createCollisionResponseAnimator(
      cameraCollisionSelector, camera, core::vector3df(30,50,30),
      core::vector3df(0,-10,0), core::vector3df(0,30,0));
    levelSelector->drop(); // As soon as we're done with the levelSelector, drop it.
    camera->addAnimator(levelAnim);
    levelAnim->drop();  // And likewise, drop the animator when we're done referring to it.
  }


  device->getCursorControl()->setVisible(false);
  //camera->setTarget(cube->getAbsolutePosition());
  int lastFPS = -1;
  video::ITexture * texture;
  int mark = 0;
  while(device->run()) {
    device->getVideoDriver()->removeTexture(texture);
    texture = driver->getTexture("/tmp/vrhs/shot.png");
    cube->setMaterialTexture(0, texture);
    // Make the pointy pointer thingy
    core::line3d<f32> ray;
    ray.start = camera->getPosition();
    ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 1000.0f;

    scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

    core::vector3df intersection; 
    core::triangle3df hitTriangle;

    driver->beginScene(true, true, video::SColor(255,100,101,140));
    scene::ISceneNode * selectedSceneNode =
      collMan->getSceneNodeAndCollisionPointFromRay(
          ray,
          intersection,
          hitTriangle,
          IDFlag_IsInteractable,
          0);

    if(selectedSceneNode)
    {
      mark++;
      // I have nfi why I have to do this if... but the selection in collMan doesn't work!
      if(selectedSceneNode->getID() == IDFlag_IsInteractable); // do a thing
      bill->setPosition(intersection);
      driver->setTransform(video::ETS_WORLD, core::matrix4());
      driver->setMaterial(selectMaterial);
      driver->draw3DTriangle(hitTriangle, video::SColor(0, 255, 0, 0));
    }

    smgr->drawAll();
    guienv->drawAll();
    driver->endScene();
  }
  device->drop();
  return 0;

}
