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

//namespace OpenEngine {
//namespace Something {

  /**
   * 
   */
  class MapLoader {
  private:
    ISceneNode* dynamicObjects;
    ISceneNode* staticObjects;
    ISceneNode* physicsObjects;
    
    ISceneNode* LoadModelFromFile(string filepath);
	
  public:
	MapLoader();
	virtual ~MapLoader();
	virtual bool LoadMap(PropertyList* plist);
	virtual ISceneNode* GetStaticScene();
	virtual ISceneNode* GetDynamicScene();
	virtual ISceneNode* GetPhysicsScene();
  };

//}
//}

#endif // _MAP_LOADER_H_
