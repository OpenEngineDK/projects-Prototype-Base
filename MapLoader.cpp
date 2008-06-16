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

// Sky extension
#include <Sky/SkyBox.h>
#include <Sky/SkyDome.h>
using namespace OpenEngine::Sky;

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
    
	if (plist->HaveKey("sky.type")) {
		string skyType     = plist->GetString("sky.type");
		string skyResource = plist->GetString("sky.resource");
		string skyPath     = skyResource.substr(0, skyResource.rfind('/'));
		string skyFile     = skyResource.substr(skyPath.size() + 1, skyResource.size());
		
		DirectoryManager::AppendPath("projects/Prototype-Base/data/" + skyPath + "/");
		
		logger.info << "Skyfile '" << skyFile << "'. Loading... ";
		
		PropertyList* skyPList = new PropertyList(skyFile);
		
		if (skyType == "box") {
			SkyBox* skybox = new SkyBox(skyPList);
			staticObjects->AddNode(skybox->GetSceneNode());
			logger.info << "Done." << logger.end;
		} else if (skyType == "dome") {
			SkyDome* skydome = new SkyDome(skyPList);
			staticObjects->AddNode(skydome->GetSceneNode());
			logger.info << "Done." << logger.end;
		} else {
			logger.warning << "Unknown sky type '" << skyType << "'" << logger.end;
		}
	}
	
	LoadModelsToScene(plist, staticObjects,  "staticObjects");
	LoadModelsToScene(plist, physicsObjects, "physicsObjects");
	LoadModelsToScene(plist, dynamicObjects, "dynamicObjects");
	
	int spawnPointCount = plist->ListSize("spawnpoints");
 	for (int i = 0; i < spawnPointCount; i++) {
        Vector<3,float> spawnPoint = plist->GetVector<3,float>("spawnpoints", i);
		spawnPoints.push_back(spawnPoint);
    }
    
    gravity = plist->GetVector<3,float>("gravity");
    
    //DirectoryManager::AppendPath("projects/Prototype-Base/data/ParticleSystems/");
    
    int particleResourceCount = plist->ListSize("particlesystems");
 	for (int i = 0; i < particleResourceCount; i++) {
        PropertyList* particlePList = new PropertyList(plist->GetString("particlesystems", i));
		particleResources.push_back(particlePList);
		
		logger.info << "Particle system: '" << particlePList->GetFileName() << "'" << logger.end;
    }
	
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
  
  void MapLoader::LoadModelsToScene(PropertyList* plist, ISceneNode* scene, string type) {
  	int resourceCount = plist->ListSize(type + ".resource");
 	for (int i = 0; i < resourceCount; i++) {
        string modelName = plist->GetString(type + ".resource", i);
		scene->AddNode( LoadModelFromFile(modelName) );
    }
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
  
  std::vector< Vector<3,float> > MapLoader::GetSpawnPoints() {
  	return spawnPoints;
  }
  
  std::vector< PropertyList* > MapLoader::GetParticleResources() {
  	return particleResources;
  }
  
  Vector<3,float> MapLoader::GetGravity() {
  	return gravity;
  }

//}
//}
