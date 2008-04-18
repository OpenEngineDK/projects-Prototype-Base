
#include <Meta/OpenGL.h>
#include "TankController.h"


using namespace OpenEngine::Renderers;
//using namespace OpenEngine::Renderers::OpenGL;

namespace OpenEngine {
	namespace Prototype {

		TankController::TankController(TankManager* tankMgr, ClassicMovementHandler* movementHandler, FollowCamera* camera) {
			this->tankMgr = tankMgr;
			this->classicMovement = movementHandler;
			this->camera = camera;
			crosshairNode = new Crosshair();
		}

		TankController::~TankController() {}

		void TankController::SetPlayerTank(int i) {
			Tank* tank = tankMgr->GetTank(i);
			if ( tank != NULL ) {	
				classicMovement->SetTank(tank);
				ISceneNode* parent = crosshairNode->GetParent();
				if ( parent != NULL ) {
					parent->RemoveNode(crosshairNode);
				}
				tank->GetTurretGunTransformationNode()->AddNode(crosshairNode);
	
				Vector<3,float> position(0, 0, 0);
				camera->SetPosition(position + Vector<3,float>(-80,30,0));
				camera->LookAt(position + Vector<3,float>(0,25,0));
				camera->Follow(tank->GetCameraTransformationNode());
			}
		}

	} // NS Utils
} // NS OpenEngine

