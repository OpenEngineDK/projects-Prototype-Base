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
#include "ActionMovementHandler.h"
#include "QuitHandler.h"
#include "Tank.h"
#include "Crosshair.h"

// Additional namespaces (others are in the header).
using namespace OpenEngine::Devices;
using namespace OpenEngine::Renderers::OpenGL;
using namespace OpenEngine::Renderers;
using namespace OpenEngine::Resources;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Physics;

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
	IModelResourcePtr mod_res = ResourceManager::CreateModel(filepath);
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
        FollowCamera* camera = new FollowCamera( *(new InterpolatedViewingVolume( *(new ViewingVolume()) )));
	Frustum* frustum = new Frustum(*camera, 20, 50000);
	viewport->SetViewingVolume(frustum);

	// set the resources directory
	string resources = "projects/Prototype-Base/data/";
	ResourceManager::AppendPath(resources);
	logger.info << "Resource directory: " << resources << logger.end;

	// load the resource plug-ins
	ResourceManager::AddModelPlugin(new OBJPlugin());
	ResourceManager::AddTexturePlugin(new TGAPlugin());
	ResourceManager::AddShaderPlugin(new GLSLPlugin());

	// pointer to Box transformation node
	RigidBox* box = NULL;
	Vector<3,float> position(0, 0, 0);

	// Add models from models.txt to the scene
	ISceneNode* current = rNode;
	ISceneNode* dynamicObjects = new SceneNode();
	ISceneNode* staticObjects  = new SceneNode();
	ISceneNode* physicObjects  = new SceneNode();


	//Static
	GeometryNode* geoSkyBox = LoadGeometryFromFile("MarioBox/MarioBox.obj");
	GeometryNode* geoGround = LoadGeometryFromFile("2DGround/2DGround.obj");
	//GeometryNode* geoGround = LoadGeometryFromFile("2DGround/Field.obj");
	//GeometryNode* geoGround = LoadGeometryFromFile("Island/island.obj");
	staticObjects->AddNode(geoSkyBox);
	staticObjects->AddNode(geoGround);

	//Physic
        GeometryNode* geoGround2 = LoadGeometryFromFile("2DGround/2DGround.obj");
	physicObjects->AddNode(geoGround2);

	// Dynamic
	GeometryNode* geoTank = LoadGeometryFromFile("ProtoTank/tank_body.obj");//LoadGeometryFromFile("ToonyTank/tanklower.obj");
	GeometryNode* geoTurret = LoadGeometryFromFile("ProtoTank/tank_turret.obj");//LoadGeometryFromFile("ToonyTank/tankupper.obj");
	GeometryNode* geoGun = LoadGeometryFromFile("ProtoTank/tank_cannon.obj");//LoadGeometryFromFile("ToonyTank/tankgun.obj");

	// Load rigid-box
	box = new RigidBox( Box(*(geoTank->GetFaceSet())) );
	box->SetCenter( position );
	Tank* tank = new Tank(geoTank,geoTurret,geoGun,box);
	engine.AddModule(*tank);
	dynamicObjects->AddNode(tank->GetTankTransformationNode());
	box->SetTransformationNode(tank->GetTankTransformationNode());
	tank->GetTurretGunTransformationNode()->AddNode(new Crosshair());
	rNode->AddNode(tank->GetTestShot());
        engine.AddModule(*(tank->GetTestShot()),IGameEngine::TICK_DEPENDENT);

	camera->SetPosition(position + Vector<3,float>(-80,30,0));
	camera->LookAt(position + Vector<3,float>(0,25,0));
	//camera->Follow(tank->GetTurretTransformationNode());
        //camera->Follow(tank->GetTurretGunTransformationNode());
        camera->Follow(tank->GetCameraTransformationNode());

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

	// Add FixedTimeStepPhysics module
        FixedTimeStepPhysics* physic = new FixedTimeStepPhysics( physicObjects );
	physic->AddRigidBody(box);
	engine.AddModule(*physic, IGameEngine::TICK_DEPENDENT);

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

	// add the RigidBox to the scene, for debuging
	//if (box != NULL) rNode->AddNode( box->GetRigidBoxNode() );

	// Register movement handler to be able to move the camera
	ClassicMovementHandler* classicMovement = new ClassicMovementHandler(tank, input, physic);
	classicMovement->BindToEventSystem();
	engine.AddModule(*classicMovement,IGameEngine::TICK_DEPENDENT);

	/*ActionMovementHandler* actionMovement = new ActionMovementHandler(tank, input, physic);
	actionMovement->BindToEventSystem();
	engine.AddModule(*actionMovement,IGameEngine::TICK_DEPENDENT);*/

	// Keyboard bindings to the rigid box and camera
	/*KeyboardHandler* keyHandler = new KeyboardHandler(camera,box,physic);
	keyHandler->BindToEventSystem();
	engine.AddModule(*keyHandler);*/

	// Return true to signal success.
	return true;
}

// Other factory methods. The returned objects are all created in the
// factory constructor.
IFrame*      GameFactory::GetFrame()    { return this->frame;    }
IRenderer*   GameFactory::GetRenderer() { return this->renderer; }
