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
            //FIXME: shit sometimes gets weird here
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
  selectMaterial.Lighting = false;

  scene::IBillboardSceneNode * bill = smgr->addBillboardSceneNode();
  bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR );
  bill->setMaterialTexture(0, driver->getTexture("media/particle.bmp"));
  bill->setMaterialFlag(video::EMF_LIGHTING, false);
  bill->setMaterialFlag(video::EMF_ZBUFFER, false);
  bill->setSize(core::dimension2d<f32>(20.0f, 20.0f));
  bill->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it

  // Set up terminal
  scene::IMeshSceneNode *terminal = smgr->addCubeSceneNode(15.0f, 0, -1, core::vector3df(10,-10,10), core::vector3df(0,0,0), core::vector3df(4, 4, 1));
  terminal->setMaterialFlag(video::EMF_LIGHTING, true);
  terminal->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
  terminal->setTriangleSelector( smgr->createTriangleSelector( terminal->getMesh(), terminal ));

  // Add lighting
  scene::ISceneNode* light = smgr->addLightSceneNode(0, core::vector3df(10,10,10),
      video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
  light->addAnimator(smgr->createFlyCircleAnimator(core::vector3df(0,150,0),250.0f));

  light = smgr->addBillboardSceneNode(light, core::dimension2d<f32>(50, 50));
  light->setMaterialFlag(video::EMF_LIGHTING, false);
  light->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
  light->setMaterialTexture(0, driver->getTexture("media/particle.bmp"));

  // Metal room
  scene::IMeshSceneNode *room = smgr->addCubeSceneNode(15.0f, 0, IDFlag_IsSolid, core::vector3df(10,160,30), core::vector3df(0,0,0), core::vector3df(30, 30, 30));
  smgr->getMeshManipulator()->flipSurfaces(room->getMesh());
  smgr->getMeshManipulator()->recalculateNormals(room->getMesh());
  //smgr->getMeshManipulator()->recalculateNormals(room->getMeshBuffer()); maybe this?
  room->setMaterialFlag(video::EMF_LIGHTING, true);
  room->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
  room->setMaterialFlag(video::EMF_BACK_FACE_CULLING, false);
  room->setMaterialTexture(0, driver->getTexture("media/texture.jpg"));
  room->setTriangleSelector(smgr->createTriangleSelector( room->getMesh(), room ));


  scene::IMeshSceneNode *room2 = smgr->addCubeSceneNode(15.0f, 0, IDFlag_IsSolid, core::vector3df(10,160,30), core::vector3df(0,0,0), core::vector3df(3, 3, 3));
  room2->setMaterialFlag(video::EMF_LIGHTING, true);
  room2->setMaterialTexture(0, driver->getTexture("media/texture.jpg"));
  room2->setTriangleSelector(smgr->createTriangleSelector( room2->getMesh(), room2 ));

  bool yesLevel = false;
  int collideablesNumber = 3;
  scene::ITriangleSelector* collideables[collideablesNumber];
  collideables[0] = terminal->getTriangleSelector();
  collideables[1] = room->getTriangleSelector();

  scene::IMetaTriangleSelector* cameraCollisionSelector = 
    smgr->createMetaTriangleSelector();
  if(yesLevel){
    // Create the level and the collision levelSelector
    device->getFileSystem()->addFileArchive("models/map-20kdm2.pk3");
    scene::IAnimatedMesh* levelMesh = smgr->getMesh("20kdm2.bsp");
    scene::IMeshSceneNode* levelNode = 0;
    scene::ITriangleSelector* levelSelector = 0;
    if (levelMesh) {
      levelNode = smgr->addOctreeSceneNode(levelMesh->getMesh(0), 0, IDFlag_IsSolid);
//        node = smgr->addMeshSceneNode(mesh->getMesh(0));
      if(levelNode){
        levelNode->setMaterialFlag(video::EMF_LIGHTING, true);
        levelNode->setPosition(core::vector3df(-1350,-130,-1400));
        levelSelector = smgr->createOctreeTriangleSelector(
                   levelNode->getMesh(), levelNode, 128);
        collideables[2] = levelSelector;
        levelNode->setTriangleSelector(levelSelector);
        levelSelector->drop(); // As soon as we're done with the levelSelector, drop it.
      }
    }
  } else {
    collideablesNumber = 2;
  }
  for(int i=0; i<collideablesNumber; i++){
    cameraCollisionSelector->addTriangleSelector(collideables[i]);
  }

  //Create camera and pin it to the floor
  scene::ICameraSceneNode *camera = smgr->addCameraSceneNodeFPS(0, 100.0f, .3f, ID_IsNotPickable, 0, 0, true, 3.f);
  camera->setPosition(core::vector3df(50,50,-60));

  //Add collision to the camera
  scene::ISceneNodeAnimator* levelAnim = smgr->createCollisionResponseAnimator(
    cameraCollisionSelector, camera, core::vector3df(30,50,30),
    core::vector3df(0,-10,0), core::vector3df(0,30,0));
  camera->addAnimator(levelAnim);
  levelAnim->drop();  // And likewise, drop the animator when we're done referring to it.


  device->getCursorControl()->setVisible(false);
  //camera->setTarget(terminal->getAbsolutePosition());
  int lastFPS = -1;
  video::ITexture * texture;
  scene::ISceneNode* lastSelectedSceneNode = NULL;
  while(device->run()) {
    device->getVideoDriver()->removeTexture(texture);
    texture = driver->getTexture("/tmp/vrhs/shot.png");
    terminal->setMaterialTexture(0, texture);
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

    if(lastSelectedSceneNode) lastSelectedSceneNode->setMaterialFlag(video::EMF_LIGHTING, true);
    if(selectedSceneNode)
    {
      bill->setPosition(intersection);
      driver->setTransform(video::ETS_WORLD, core::matrix4());
      driver->setMaterial(selectMaterial);
      driver->draw3DTriangle(hitTriangle, video::SColor(0, 255, 0, 0));
      selectedSceneNode->setMaterialFlag(video::EMF_LIGHTING, false);
      lastSelectedSceneNode = selectedSceneNode;
    } 

    smgr->drawAll();
    guienv->drawAll();
    driver->endScene();
  }
  device->drop();
  return 0;

}
