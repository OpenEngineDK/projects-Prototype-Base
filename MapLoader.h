// Map loader
// -------------------------------------------------------------------
// Copyright (C) 2007 OpenEngine.dk (See AUTHORS) 
// 
// This program is free software; It is covered by the GNU General 
// Public License version 2 or any later version. 
// See the GNU General Public License for more details (see LICENSE). 
//--------------------------------------------------------------------

#ifndef _MAP_LOADER_H_
#define _MAP_LOADER_H_

#include <Scene/ISceneNode.h>
#include <Utils/PropertyList.h>

using namespace OpenEngine::Scene;
using namespace OpenEngine::Utils;

#include <vector>

//namespace OpenEngine {
//namespace Something {

  /**
   * 
   */
  class MapLoader {
  private:
  	std::vector< Vector<3,float> > spawnPoints;
  	std::vector< PropertyList* > particleResources;
  	
  	Vector<3,float> gravity;
  
    ISceneNode* dynamicObjects;
    ISceneNode* staticObjects;
    ISceneNode* physicsObjects;
    
    ISceneNode* LoadModelFromFile(string filepath);
    void LoadModelsToScene(PropertyList* plist, ISceneNode* scene, string type);
	
  public:
	MapLoader();
	virtual ~MapLoader();
	virtual bool LoadMap(PropertyList* plist);
	virtual ISceneNode* GetStaticScene();
	virtual ISceneNode* GetDynamicScene();
	virtual ISceneNode* GetPhysicsScene();
	virtual std::vector< Vector<3,float> > GetSpawnPoints();
	virtual Vector<3,float> GetGravity();
	virtual std::vector< PropertyList* > GetParticleResources();
  };

//}
//}

#endif // _MAP_LOADER_H_
