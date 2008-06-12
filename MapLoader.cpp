// Map loader
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#include "MapLoader.h"

#include <Scene/SceneNode.h>

#include <Resources/ResourceManager.h>
#include <Resources/IModelResource.h>

using namespace OpenEngine::Resources;

#include <string.h>
using std::string;

//namespace OpenEngine {
//namespace Something {

  MapLoader::MapLoader() {
	dynamicObjects = new SceneNode();
	staticObjects  = new SceneNode();
	physicsObjects  = new SceneNode();
  }

  MapLoader::~MapLoader() {

  }
  
  bool MapLoader::LoadMap(PropertyList* plist) {
	dynamicObjects->DeleteAllNodes();
	staticObjects->DeleteAllNodes();
	physicsObjects->DeleteAllNodes();
	
	int staticCount = plist->ListSize("staticObjects.resource");
 	for (int i = 0; i < staticCount; i++) {
        string modelName = plist->GetString("staticObjects.resource", i);
        ISceneNode* model = LoadModelFromFile(modelName);
		staticObjects->AddNode(model);
    }

/*
    string skydomeDir = "Sky/Skydome";
    DirectoryManager::AppendPath(resources + skydomeDir + "/"); // Hack to enable us to find files in this directory
    PropertyList* skydomePList = new PropertyList(skydomeDir + "/skydome.skd");
	SkyDome* skydome = new SkyDome(skydomePList);
	
	string islandGeometry;
	//islandGeometry = "2DGround/2DGround.obj"; 
	islandGeometry = "Island/island.obj";
	
	GeometryNode* geoGround = LoadGeometryFromFile(islandGeometry);
	staticObjects->AddNode(skydome->GetSceneNode());
	staticObjects->AddNode(geoGround);

	//Physic
	GeometryNode* geoGround2 = LoadGeometryFromFile(islandGeometry);
	physicObjects->AddNode(geoGround2);

	rNode->AddNode(staticObjects);
	rNode->AddNode(dynamicObjects);
	rNode->AddNode(physicObjects);

  
  	// load static geometry
	{
		TriangleMesh * triMesh = new TriangleMesh(*physicObjects);
		RigidBody * meshBody = new RigidBody(triMesh);
		meshBody->SetName("Island");
		meshBody->SetPosition(Vector<3,float>(0,0,5.0));
		physics->AddRigidBody(meshBody);
	}
	*/
	
	return true;
  }
  
  ISceneNode* MapLoader::LoadModelFromFile(string filepath) {
	// Load the model
	IModelResourcePtr mod_res = ResourceManager<IModelResource>::Create(filepath);
	mod_res->Load();
	ISceneNode* mod_node = mod_res->GetSceneNode();
	mod_res->Unload();
	return mod_node;
  }

  ISceneNode* MapLoader::GetStaticScene() {
  	return staticObjects;
  }
  
  ISceneNode* MapLoader::GetDynamicScene() {
  	return dynamicObjects;
  }
  
  ISceneNode* MapLoader::GetPhysicsScene() {
  	return physicsObjects;
  }

//}
//}
