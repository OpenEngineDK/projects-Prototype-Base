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
#include <Renderers/AcceleratedRenderingView.h>

// from FixedTimeStepPhysics
#include <Physics/FixedTimeStepPhysics.h>
#include <Physics/RigidBox.h>

// Project files
#include "RenderStateHandler.h"
#include "ClassicMovementHandler.h"
#include "QuitHandler.h"
#include "Tank.h"
#include "Crosshair.h"
#include "TankManager.h"
#include "TankController.h"

// Additional namespaces (others are in the header).
using namespace OpenEngine::Devices;
using namespace OpenEngine::Renderers::OpenGL;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Physics;

// Prototype namespace
using namespace OpenEngine::Prototype;

// composite rendering view. Uses RenderingView for drawing and
// AcceleratedRenderingView for clipping. 
class MyRenderingView : 
	public RenderingView/*,
	public AcceleratedRenderingView*/ {
public:
	MyRenderingView(Viewport& viewport) :
	  IRenderingView(viewport),
		  RenderingView(viewport)/*,
		  AcceleratedRenderingView(viewport)*/ {
	  }
};

GeometryNode* LoadGeometryFromFile(string filepath) {
	// Load the model
	IModelResourcePtr mod_res = ResourceManager<IModelResource>::Create(filepath);
	mod_res->Load();
	GeometryNode* mod_node = new GeometryNode();
	mod_node->SetFaceSet(mod_res->GetFaceSet());
	mod_res->Unload();
	return mod_node;
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

	// Main viewport
	viewport = new Viewport(*frame);

	// Create a renderer.
	this->renderer = new Renderer();

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
        /*FollowCamera* */ camera = new FollowCamera( *(new InterpolatedViewingVolume( *(new ViewingVolume()) )));
	Frustum* frustum = new Frustum(*camera, 20, 50000);
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
	ISceneNode* current = rNode;
	dynamicObjects = new SceneNode();
	staticObjects  = new SceneNode();
	physicObjects  = new SceneNode();

	//Static
	GeometryNode* geoSkyBox = LoadGeometryFromFile("MarioBox/MarioBox.obj");
	GeometryNode* geoGround = LoadGeometryFromFile("2DGround/2DGround.obj");
	staticObjects->AddNode(geoSkyBox);
	staticObjects->AddNode(geoGround);

	//Physic
        GeometryNode* geoGround2 = LoadGeometryFromFile("2DGround/2DGround.obj");
	physicObjects->AddNode(geoGround2);

	// Dynamic
	//GeometryNode* geoTank = LoadGeometryFromFile("ProtoTank/tank_body.obj");
	//GeometryNode* geoTurret = LoadGeometryFromFile("ProtoTank/tank_turret.obj");
	//GeometryNode* geoGun = LoadGeometryFromFile("ProtoTank/tank_cannon.obj");
	GeometryNode* geoTank = LoadGeometryFromFile("Tank2/tank2.obj");
	GeometryNode* geoTurret = LoadGeometryFromFile("Tank2/turret.obj");
	GeometryNode* geoGun = LoadGeometryFromFile("Tank2/gun.obj");

	// Add FixedTimeStepPhysics module
        physic = new FixedTimeStepPhysics( physicObjects );
	engine.AddModule(*physic, IGameEngine::TICK_DEPENDENT);

	// Register movement handler to be able to move the camera
	classicMovement = new ClassicMovementHandler(input, physic);
	classicMovement->BindToEventSystem();
	engine.AddModule(*classicMovement,IGameEngine::TICK_DEPENDENT);

	//Setup the tanks
	tankMgr = new TankManager();
	TankController* tankCtrl = new TankController(tankMgr, classicMovement, camera);
	classicMovement->SetTankController(tankCtrl);
	Tank::SetModel(geoTank,geoTurret,geoGun);
        engine.AddModule(*tankMgr);

	shotMgr = new ShotManager();
	rNode->AddNode(shotMgr);
        engine.AddModule(*shotMgr,IGameEngine::TICK_DEPENDENT);

	crosshairNode = new Crosshair();

	// Load tanks
	int tankCount = 10;
	for ( int i = 0; i < tankCount; i++ ) {
		AddTank();
	}

	tankCtrl->SetPlayerTank(0);

	rNode->AddNode(dynamicObjects);

	// Process static tree
	logger.info << "Preprocessing of physics tree: started" << logger.end;
        CollectedGeometryTransformer collT;
        QuadTransformer quadT;
        BSPTransformer bspT;
        collT.Transform(*physicObjects);
        quadT.Transform(*physicObjects);
        bspT.Transform(*physicObjects);
	logger.info << "Preprocessing of physics tree: done" << logger.end;

	logger.info << "Preprocessing of static tree: started" << logger.end;
        QuadTransformer quadT2;
        quadT2.SetMaxFaceCount(500);
        quadT2.SetMaxQuadSize(100);
        quadT2.Transform(*staticObjects);
        rNode->AddNode(staticObjects);
	logger.info << "Preprocessing of static tree: done" << logger.end;

	// Visualization of the frustum
	//frustum->VisualizeClipping(true);
	//rNode->AddNode(frustum->GetFrustumNode());	

	// Return true to signal success.
	return true;
}

void GameFactory::AddTank() {
	RigidBox* box = NULL;
	Vector<3,float> position(2, 1, 2);
	box = new RigidBox( Box(*(LoadGeometryFromFile("ProtoTank/tank_body.obj")->GetFaceSet())) );
	box->SetCenter( position );
	Tank* tank = new Tank(box);
	tank->SetShotManager(shotMgr);
	tankMgr->AddTank(tank);

	dynamicObjects->AddNode(tank->GetTankTransformationNode());
	box->SetTransformationNode(tank->GetTankTransformationNode());
	physic->AddRigidBody(box);

	//if (box != NULL) tank->GetTankTransformationNode()->GetParent()->AddNode( box->GetRigidBoxNode() );
}

// Other factory methods. The returned objects are all created in the
// factory constructor.
IFrame*      GameFactory::GetFrame()    { return this->frame;    }
IRenderer*   GameFactory::GetRenderer() { return this->renderer; }
