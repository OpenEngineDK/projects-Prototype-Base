// Game Factory for the OERacer project.
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

// Class header
#include "GameFactory.h"

// OpenEngine library
#include <Display/FollowCamera.h>
#include <Display/Frustum.h>
#include <Display/Orthotope.h>
#include <Display/InterpolatedViewingVolume.h>
#include <Display/ViewingVolume.h>
#include <Display/SDLFrame.h>
#include <Devices/SDLInput.h>
#include <Logging/Logger.h>
#include <Renderers/IRenderNode.h>
#include <Renderers/RenderStateNode.h>
#include <Renderers/OpenGL/Renderer.h>
#include <Renderers/OpenGL/RenderingView.h>
#include <Resources/IModelResource.h>
#include <Resources/File.h>
#include <Resources/GLSLResource.h>
#include <Resources/TGAResource.h>
#include <Resources/OBJResource.h>
#include <Resources/DirectoryManager.h>
#include <Resources/ResourceManager.h>
#include <Scene/SceneNode.h>
#include <Scene/GeometryNode.h>
#include <Scene/TransformationNode.h>
#include <Utils/Statistics.h>
#include <Renderers/OpenGL/TextureLoader.h>

// from AccelerationStructures extension
#include <Scene/CollectedGeometryTransformer.h>
#include <Scene/QuadTransformer.h>
#include <Scene/BSPTransformer.h>
#include <Scene/ASDotVisitor.h>
#include <Renderers/AcceleratedRenderingView.h>

// Project files
#include "RenderStateHandler.h"
#include "QuitHandler.h"

#include "Crosshair.h"
#include "TankManager.h"
#include "TankController.h"
#include "ClassicMovementHandler.h"
#include "Vehicles/ITank.h"
#include "Vehicles/SnowTank.h"
#include "Vehicles/SeanTank.h"
#include "Gamemodes/IGamemode.h"
#include "Gamemodes/TestGamemode.h"
#include <Scene/PointLightNode.h>
#include <Scene/SpotLightNode.h>
#include "MapLoader.h"


#include <Geometry/Sphere.h>
#include <Geometry/AABB.h>
#include <Geometry/TriangleMesh.h>
#include <Geometry/CompoundShape.h>


// Physics extension
#include <Physics/RigidBody.h>
#include <Physics/DynamicBody.h>
#include "ForceHandler.h"

// Network extension
#include <Network/ErlNetwork.h>
#include "NetworkHandler.h"

// Particle extension
#include <Particles/ParticleGroups.h>
#include "ParticleGroupBuilder.h"
#include <Particles/Particles.h>
#include <Particles/Modifiers.h>
#include <Math/Interpolators.h>
#include <Particles/PointEmitter.h>
#include <Utils/PropertyList.h>


// From extensions
#include <Resources/ColladaResource.h>



#define MyParticleGroup EnergyParticleGroup<BillBoardParticle<EnergyParticle<DirectionParticle<IParticle> > > >
#define MyParticle EnergyParticle<BillBoardParticle<EnergyParticle<DirectionParticle<IParticle> > > >

// Additional namespaces (others are in the header).
using namespace OpenEngine::Devices;
using namespace OpenEngine::Renderers::OpenGL;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Physics;

using namespace OpenEngine::Network;

// Prototype namespace
using namespace OpenEngine::Prototype;
using namespace OpenEngine::Prototype::Gamemode;

// composite rendering view. Uses RenderingView for drawing and
// AcceleratedRenderingView for clipping. 
class MyRenderingView : 
    public RenderingView,
    public AcceleratedRenderingView {
public:
    MyRenderingView(Viewport& viewport) :
        IRenderingView(viewport),
        RenderingView(viewport),
        AcceleratedRenderingView(viewport) {

    }
};

class SpawnHandler : public OpenEngine::Core::IListener<OpenEngine::Devices::KeyboardEventArg> {
 public:
  SpawnHandler(OpenEngine::Physics::DynamicBody* body, std::vector< Vector<3,float> > spawnPoints) : body(body), spawnPoints(spawnPoints) {}
  void Handle(OpenEngine::Devices::KeyboardEventArg arg) {
  	if (arg.type == KeyboardEventArg::PRESS && arg.sym == KEY_SPACE) {
  		int randomIndex = (int)(((float)std::rand()/RAND_MAX) * spawnPoints.size());
  		body->SetPosition(spawnPoints[randomIndex]);
        body->SetRotation(Quaternion<float>(1,0,0,0));
        body->SetLinearVelocity(Vector<3,float>(0,0,0));
        body->SetAngularVelocity(Vector<3,float>(0,0,0));
  	}
  }

 private:
  OpenEngine::Physics::DynamicBody* body;
  std::vector< Vector<3,float> > spawnPoints;
};

GeometryNode* LoadGeometryFromFile(string filepath) {
	// Load the model
	IModelResourcePtr mod_res = ResourceManager<IModelResource>::Create(filepath);
	mod_res->Load();
	GeometryNode* mod_node = dynamic_cast<GeometryNode*>(mod_res->GetSceneNode());
	mod_res->Unload();
	return mod_node;
}

IRigidBody * CreateSphere() {
  DynamicBody * sphereBody = new DynamicBody( new RigidBody( new Sphere( Vector<3,float>(0,2,40),40.f) ) );

  sphereBody->SetPosition(Vector<3,float>(0,200,40));
  sphereBody->SetLinearVelocity(Vector<3,float>(0,0,0));
  sphereBody->SetMass(300.0f);
  return sphereBody;
}

void CreateCrate(PhysicsFacade * physics, FaceSet* faces, ISceneNode* scene, float width = 11.0, float height = 11.0) {
  int maxcount = 50;
  for (int i = 0; i < maxcount; i++) {
  	GeometryNode* newNode = new GeometryNode(faces);
    AABB * shape = new AABB(*newNode);
    DynamicBody * blockBody = new DynamicBody( new RigidBody(shape) );
    
    TransformationNode* transNode = blockBody->GetTransformationNode();
	transNode->AddNode(newNode);

	int cols = 5;
	int row = i / cols;
	int col = i % cols;

    Vector<3,float> position(200.0, 1.0 + row * height, col * width);
    blockBody->SetMass(0.2f);
    blockBody->SetName("Box " + Convert::int2string(i+1));
    blockBody->SetPosition(position);
    physics->AddRigidBody(blockBody);
    scene->AddNode(transNode);
  }
}

/**
* Factory constructor.
*
* Initializes the different components so they are accessible when
* the setup method is executed.
*/
GameFactory::GameFactory() {

	// Create a frame and viewport.
	this->frame = new SDLFrame(1024, 768, 32);
	//this->frame = new SDLFrame(640, 480, 32);

	// Main viewport
	viewport = new Viewport(*frame);

	// Create a renderer.
	this->renderer = new Renderer();
    renderer->SetFarPlane(50000.0);

	// Add a rendering view to the renderer
	this->renderer->AddRenderingView(new MyRenderingView(*viewport));
}

/**
* Setup handler.
*
* This is the main setup method of the game factory. Here you can add
* any non-standard modules and perform other setup tasks prior to the
* engine start up.
*
* @param engine The game engine instance.
*/
bool GameFactory::SetupEngine(IGameEngine& engine) {

	// Add mouse and keyboard module here
	SDLInput* input = new SDLInput();
	engine.AddModule(*input);

	// Add Statistics module
	engine.AddModule(*(new OpenEngine::Utils::Statistics(1000)));

	// Create a root scene node
	SceneNode* scene = new SceneNode();

	// Supply the scene to the renderer
	this->renderer->SetSceneRoot(scene);

	// Add RenderStateNode to change rendering features at runtime
	RenderStateNode* rNode = new RenderStateNode();
	rNode->AddOptions(RenderStateNode::RENDER_TEXTURES);
	rNode->AddOptions(RenderStateNode::RENDER_SHADERS);
	rNode->AddOptions(RenderStateNode::RENDER_BACKFACES);
	scene->AddNode(rNode);

	// Bind keys for changing rendering state
	RenderStateHandler* renderStateHandler = new RenderStateHandler(rNode);
	renderStateHandler->BindToEventSystem();

	// Bind the quit handler
	QuitHandler* quit_h = new QuitHandler();
	quit_h->BindToEventSystem();

	// Bind the camera to the viewport
	camera = new FollowCamera( *(new InterpolatedViewingVolume( *(new ViewingVolume()) )));
	Frustum* frustum = new Frustum(*camera, 1, 1000.0);
	viewport->SetViewingVolume(frustum);

	// set the resources directory
	string resources = "projects/Prototype-Base/data/";
	DirectoryManager::AppendPath(resources);
	logger.info << "Resource directory: " << resources << logger.end;

	// load the resource plug-ins
	ResourceManager<IModelResource>::AddPlugin(new OBJPlugin());
	ResourceManager<ITextureResource>::AddPlugin(new TGAPlugin());
	ResourceManager<IShaderResource>::AddPlugin(new GLSLPlugin());

	// Add models from models.txt to the scene
	dynamicObjects = new SceneNode();
	staticObjects  = new SceneNode();
	physicsObjects  = new SceneNode();
	
	
	// Particles
	particleSystem = new ParticleSystem();    
	engine.AddModule(*particleSystem, IGameEngine::TICK_DEPENDENT);
	

	MapLoader* mapLoader = new MapLoader();
	bool result = mapLoader->LoadMap(new PropertyList("Maps/testmap.btm"));
	if (result) {
		staticObjects->AddNode(mapLoader->GetStaticScene());
		physicsObjects->AddNode(mapLoader->GetPhysicsScene());
		dynamicObjects->AddNode(mapLoader->GetDynamicScene());
	}

	// Register movement handler to be able to move the camera
	classicMovement = new ClassicMovementHandler(input);
	classicMovement->BindToEventSystem();
	engine.AddModule(*classicMovement,IGameEngine::TICK_DEPENDENT);

	//Setup the tanks
	tankMgr = new TankManager();
	TankController* tankCtrl = new TankController(tankMgr, classicMovement, camera);
	classicMovement->SetTankController(tankCtrl);
	engine.AddModule(*tankMgr);
	
	/*PointLightNode
	// Setup SnowTank
	GeometryNode* snowTankBody = LoadGeometryFromFile("ProtoTank/tank_body.obj");
	GeometryNode* snowTankTurret = LoadGeometryFromFile("ProtoTank/tank_turret.obj");
	GeometryNode* snowTankGun = LoadGeometryFromFile("ProtoTank/tank_cannon.obj");
	SnowTank::SetModel(snowTankBody,snowTankTurret,snowTankGun);
	*/
	
	// Setup SeanTank
	GeometryNode* seanTankBody = LoadGeometryFromFile("Tank2/tank2.obj");
	GeometryNode* seanTankTurret = LoadGeometryFromFile("Tank2/turret.obj");
	GeometryNode* seanTankGun = LoadGeometryFromFile("Tank2/gun.obj");
	SeanTank::SetModel(seanTankBody,seanTankTurret,seanTankGun);
	
	class Temp : public IModule {
    public:
        NetworkHandler* net;
        void Initialize() {}
        void Deinitialize() {}
        bool IsTypeOf(const std::type_info& i) { return false; }
        void Process(float dt, float p) {
            net->Notify();
        }
    };

	try {
    	ErlNetwork* netm = new ErlNetwork("camel24.daimi.au.dk", 2345);
    	NetworkHandler* neth = new NetworkHandler(this, netm);
    	engine.AddModule(*netm);
    	netm->Attach(*neth);
    	
    	Temp* t = new Temp();
    	t->net = neth;
    	engine.AddModule(*t, IGameEngine::TICK_DEPENDENT);
    } catch (Exception e) {
    	logger.error << e.what() << logger.end;
    }

	shotMgr = new ShotManager();
	rNode->AddNode(shotMgr);
	engine.AddModule(*shotMgr,IGameEngine::TICK_DEPENDENT);


	crosshairNode = new Crosshair();
	
	
	// Add particle systems to the scene
	std::vector< PropertyList* >::iterator plistIter;
	std::vector< PropertyList* > particleResources = mapLoader->GetParticleResources();
	for (plistIter = particleResources.begin(); plistIter != particleResources.end(); plistIter++) {
	  PropertyList* plist = (*plistIter);
	  
	  ParticleGroupBuilder* groupBuilder = new ParticleGroupBuilder(*plist, string("p1"));
	  MyParticleGroup* group = (MyParticleGroup*)(groupBuilder->GetParticleGroup());
	  particleSystem->AddGroup(group);
	  staticObjects->AddNode(groupBuilder->GetRenderNode());
	}
	
	
  	// create physics engine
	AABB worldAabb(Vector<3,float>(-5000,-5000,-5000),Vector<3,float>(5000,5000,5000));
	Vector<3,float> gravity = mapLoader->GetGravity();
	physics = new PhysicsFacade(worldAabb, gravity);
	engine.AddModule(*physics, IGameEngine::TICK_DEPENDENT);
	
	
	
	// load the collada resource plug-in
	ResourceManager<IModelResource>::AddPlugin(new ColladaPlugin());

    IModelResourcePtr resource = ResourceManager<IModelResource>::Create("Rock/rock.dae");
    resource->Load();
    ISceneNode* model = resource->GetSceneNode();
    resource->Unload();
    
    TransformationNode* transNode = new TransformationNode();
    transNode->AddNode(model);
    transNode->Move(0.0, -20.0, 0.0);
    
    
    DynamicBody* rockBody = new DynamicBody( new RigidBody( new AABB( *transNode ) ) );
    rockBody->SetName("Rock");
    TransformationNode* modTrans = rockBody->GetTransformationNode();
    rockBody->SetMass(100.0f);
    rockBody->SetAngularDamping(20.0f);
    rockBody->SetLinearDamping(5.0f);
    physics->AddRigidBody(rockBody);

    modTrans->AddNode(transNode);
    modTrans->Move(80.0, 20.0, 0.0);
    dynamicObjects->AddNode(modTrans);
    
    // Add point light to the tank    
    PointLightNode* pln = new PointLightNode();
            
    pln->constAtt = 0.5;
    pln->linearAtt = 0.01;
    pln->quadAtt = 0.001;
    
    pln->ambient = Vector<4,float>(0.0, 1.0, 0.2, 0.8);
    pln->diffuse = Vector<4,float>(0.0, 0.0, 1.0, 1.0);
    //pln->specular
    
    modTrans->AddNode(pln);

	IGamemode* gamemode = new TestGamemode();
	engine.AddModule(*gamemode);
	// Load tanks
	int tankCount = 1;
	for ( int i = 0; i < tankCount; i++ ) {
		ITank* tank = AddTank(i);
		tank->GetDynamicBody()->SetPosition( mapLoader->GetSpawnPoints()[i] );
		gamemode->OnPlayerConnect(i);
	}

	tankCtrl->SetPlayerTank(0);
	
    // steering
    ForceHandler * handler = new ForceHandler(tankMgr->GetTank(0)->GetDynamicBody(), physics);
    SDLInput::keyEvent.Attach(*handler);
    engine.AddModule(*handler,IGameEngine::TICK_DEPENDENT);
    rNode->AddNode(handler->GetRenderNode());
    
    
    
    SpawnHandler* spawnHandler = new SpawnHandler(tankMgr->GetTank(0)->GetDynamicBody(), mapLoader->GetSpawnPoints());
    SDLInput::keyEvent.Attach(*spawnHandler);
    
    

	/*
    logger.info << "Preprocessing of static tree: started" << logger.end;
    QuadTransformer quadT;
    quadT.SetMaxFaceCount(500);
    quadT.SetMaxQuadSize(100);
    quadT.Transform(*staticObjects);
    rNode->AddNode(staticObjects);
    logger.info << "Preprocessing of static tree: done" << logger.end;
	*/

	rNode->AddNode(staticObjects);
	rNode->AddNode(dynamicObjects);
	//rNode->AddNode(physicsObjects);

  // physics debug node
  //scene->AddNode(physics->getRenderNode(this->renderer));
  
  GeometryNode* geoBox = LoadGeometryFromFile("Box/Box.obj");
  CreateCrate(physics, geoBox->GetFaceSet(), scene);
  
  // load static geometry
   {
    TriangleMesh * triMesh = new TriangleMesh(*physicsObjects);
    RigidBody * meshBody = new RigidBody(triMesh);
    meshBody->SetName("Island");
    meshBody->SetPosition(Vector<3,float>(0,0,5.0));
    physics->AddRigidBody(meshBody);
   }

	// Return true to signal success.
	return true;
}


ITank* GameFactory::AddTank(int i) {
	DynamicBody* tankBody = NULL;
	ITank* tank;
	tankBody = new DynamicBody( new RigidBody( new AABB(*SeanTank::bodyModel) ) );
	tank = new SeanTank(tankBody);

	tank->SetShotManager(shotMgr);
	tankMgr->AddTank(tank, i);

    tankBody->SetName("Tank " + Convert::int2string(i+1));
    TransformationNode* mod_tran = tankBody->GetTransformationNode();
    tankBody->SetMass(1.0f);
    tankBody->SetAngularDamping(20.0f);
    tankBody->SetLinearDamping(5.0f);
    physics->AddRigidBody(tankBody);

    mod_tran->AddNode(tank->GetTankTransformationNode());
    dynamicObjects->AddNode(mod_tran);
    
    // Add point light to the tank    
    PointLightNode* pln = new PointLightNode();
            
    pln->constAtt = 0.8;
    pln->linearAtt = 0.005;
    pln->quadAtt = 0.0005;
    
    pln->ambient = Vector<4,float>(0.9, 0.4, 0.6, 1.0);
    pln->diffuse = Vector<4,float>(0.8, 0.8, 0.6, 0.8);
    //pln->specular
    
    TransformationNode* lightTrans = new TransformationNode();
    lightTrans->AddNode(pln);
    lightTrans->Move(20.0, 0.0, 0.0);
    
    mod_tran->AddNode(lightTrans);
    
    
    // Add particle system to the tank
    PropertyList* plist = new PropertyList("ParticleSystems/smoke2.txt");  
    ParticleGroupBuilder* groupBuilder = new ParticleGroupBuilder(*plist, string("p1"));
	MyParticleGroup* group = (MyParticleGroup*)(groupBuilder->GetParticleGroup());
    particleSystem->AddGroup(group);
    staticObjects->AddNode(groupBuilder->GetRenderNode());
    IEmitter *emitter = groupBuilder->GetEmitter();
    TransformationNode* emitNode = dynamic_cast<TransformationNode*>(emitter);
    if (emitNode)
        mod_tran->AddNode(emitNode);

    //mod_tran->AddNode(groupBuilder->GetRenderNode());

    
    return tank;
}

void GameFactory::RemoveTank(int i) {
	ITank* tank = tankMgr->GetTank(i);
	
	ISceneNode* node = tank->GetTankTransformationNode()->GetParent();
	physics->RemoveRigidBody(tank->GetDynamicBody());

    dynamicObjects->RemoveNode(node);
    
    tankMgr->RemoveTank(i);
}


// Other factory methods. The returned objects are all created in the
// factory constructor.
IFrame*      GameFactory::GetFrame()    { return this->frame;    }
IRenderer*   GameFactory::GetRenderer() { return this->renderer; }
