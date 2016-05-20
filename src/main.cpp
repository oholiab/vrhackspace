#include <irrlicht/irrlicht.h>
#include <irrlicht/driverChoice.h>
#include "controlterm.h"

#define VISIBLE 1
#define INVISIBLE 0

const char* disp = ":1";

using namespace irr;

enum {
  ID_IsNotPickable = 0,
  IDFlag_IsSolid = 1 << 0,
  IDFlag_IsInteractable = 1 << 1,
  IDFlag_IsOutline = 1 << 2
};

X11Display xdisp(disp);
scene::ISceneNode* selectedSceneNode = NULL;

class VREventReceiver : public IEventReceiver {
  public:
    virtual bool OnEvent(const SEvent& event){
      if(selectedSceneNode && event.EventType == irr::EET_KEY_INPUT_EVENT){
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

void setOutlineVisible(scene::ISceneNode *node, int visible){
  // Will set child scene node's that are outlines as invisible if they exist
  irr::core::list<irr::scene::ISceneNode*> children = node->getChildren();
  irr::core::list<irr::scene::ISceneNode*>::ConstIterator start = children.begin();
  const irr::core::list<irr::scene::ISceneNode*>::ConstIterator& end = children.end();

  for (; start != end; ++start)
  {
     irr::scene::ISceneNode* const node = (*start);

     if(node->getID() & IDFlag_IsOutline) node->setVisible(visible);
  }
}

int main(int argc, char** argv) {
  // Uses driverChoiceConsole() from driverChoice.h
  bool yesQuake = false;
  if(argc > 1){
    if(strcmp(argv[1], "--quake") == 0) yesQuake = true;
  }

  VREventReceiver receiver;
  IrrlichtDevice *device = createDevice(driverChoiceConsole(), core::dimension2d<u32>(512, 384), 16, false, false, false, &receiver);
  device->setWindowCaption(L"Grimmware VRHackspace::Mk I");

  // set up video driver, scene manager and gui environment
  video::IVideoDriver* driver = device->getVideoDriver();
  scene::ISceneManager* smgr = device->getSceneManager();
  gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

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

  // Add lighting
  scene::ISceneNode* light = smgr->addLightSceneNode(0, core::vector3df(10,10,10),
      video::SColorf(1.0f, 0.6f, 0.7f, 1.0f), 800.0f);
  light->addAnimator(smgr->createFlyCircleAnimator(core::vector3df(0,150,0),250.0f));

  light = smgr->addBillboardSceneNode(light, core::dimension2d<f32>(50, 50));
  light->setMaterialFlag(video::EMF_LIGHTING, false);
  light->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
  light->setMaterialTexture(0, driver->getTexture("media/particle.bmp"));
  light->setID(ID_IsNotPickable); // This ensures that we don't accidentally ray-pick it

  core::list<scene::ITriangleSelector*> collideables;
  collideables.push_back(terminal->getTriangleSelector());

  scene::IMetaTriangleSelector* cameraCollisionSelector = 
    smgr->createMetaTriangleSelector();
  if(yesQuake){
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
        collideables.push_back(levelSelector);
        levelNode->setTriangleSelector(levelSelector);
        levelSelector->drop(); // As soon as we're done with the levelSelector, drop it.
      }
    }
  } else {
    // Metal room
    scene::IMeshSceneNode *room = smgr->addCubeSceneNode(15.0f, 0, IDFlag_IsSolid, core::vector3df(10,160,30), core::vector3df(0,0,0), core::vector3df(30, 30, 30));
    smgr->getMeshManipulator()->flipSurfaces(room->getMesh());
    smgr->getMeshManipulator()->recalculateNormals(room->getMesh());
    room->setMaterialFlag(video::EMF_LIGHTING, true);
    room->setMaterialFlag(video::EMF_NORMALIZE_NORMALS, true);
    room->setMaterialTexture(0, driver->getTexture("media/texture.jpg"));
    room->setTriangleSelector(smgr->createTriangleSelector( room->getMesh(), room ));

    collideables.push_back(room->getTriangleSelector());
  }
  for(
      core::list<scene::ITriangleSelector*>::ConstIterator coll = collideables.begin();
      coll != collideables.end();
      ++coll
     ) cameraCollisionSelector->addTriangleSelector((*coll));

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
    texture = driver->getTexture("/vrhs/shot.png");
    terminal->setMaterialTexture(0, texture);
    // Make the pointy pointer thingy
    core::line3d<f32> ray;
    ray.start = camera->getPosition();
    ray.end = ray.start + (camera->getTarget() - ray.start).normalize() * 100.0f;

    scene::ISceneCollisionManager* collMan = smgr->getSceneCollisionManager();

    core::vector3df intersection; 
    core::triangle3df hitTriangle;

    driver->beginScene(true, true, video::SColor(255,100,101,140));
    selectedSceneNode =
      collMan->getSceneNodeAndCollisionPointFromRay(
          ray,
          intersection,
          hitTriangle,
          IDFlag_IsInteractable,
          0);

    if(lastSelectedSceneNode) setOutlineVisible(lastSelectedSceneNode, INVISIBLE);
    if(selectedSceneNode)
    {
      setOutlineVisible(selectedSceneNode, VISIBLE);
      lastSelectedSceneNode = selectedSceneNode;
    } 

    smgr->drawAll();
    guienv->drawAll();
    driver->endScene();
  }
  device->drop();
  return 0;
}
