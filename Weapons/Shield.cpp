
#include "Shield.h"
#include <Meta/OpenGL.h>
#include <Utils/Timer.h>
#include "../ShotManager.h"
#include "ShieldGenerator.h"

using namespace OpenEngine::Renderers;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Prototype;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {
		Shield::Shield(ShieldGenerator* generator) {
			this->generator = generator;
			color = Vector<3,float>(0.0, 0.0, 1.0);
			lifeTime = 0.0f;
			decayTime = 7500.0f;
			shotSpeed = 15.0f;
			rotation = 0.0;
		}

		Shield::~Shield() {

		}

		void Shield::Process(const float timeSinceLast) {
			pos = generator->UpdatePosition();
			rotation += shotSpeed;

			lifeTime += timeSinceLast;
			if ( lifeTime >= decayTime ) {
				Destroy();
			}
		}

		void Shield::Destroy() {
			shotMgr->DeleteShot(this);
		}

		void Shield::Apply(IRenderingView* view) {
			GLboolean t = glIsEnabled(GL_TEXTURE_2D);
			GLboolean l = glIsEnabled(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);

			glLineWidth(5.0);
			glPointSize(3.0);

			glColor3f(0.0, 0.0, 0.7);

			// Render the shots
			//@todo: unused variable: float diameter = 1.0;

			glPushMatrix();
			glTranslatef(pos[0],pos[1],pos[2]);
			glRotatef(rotation,0.0,1.0,0.0);

			glBegin(GL_QUADS);
			glVertex3f(17.0, 6.0, 10.0);
			glVertex3f(17.0, 12.0, 10.0);
			glVertex3f(20.0, 12.0, 0.0);
			glVertex3f(20.0, 6.0, 0.0);	

			glVertex3f(20.0, 6.0, 0.0);
			glVertex3f(20.0, 12.0, 0.0);
			glVertex3f(17.0, 12.0, -10.0);
			glVertex3f(17.0, 6.0, -10.0);

			glVertex3f(-17.0, 6.0, 10.0);
			glVertex3f(-17.0, 12.0, 10.0);
			glVertex3f(-20.0, 12.0, 0.0);
			glVertex3f(-20.0, 6.0, 0.0);	

			glVertex3f(-20.0, 6.0, 0.0);
			glVertex3f(-20.0, 12.0, 0.0);
			glVertex3f(-17.0, 12.0, -10.0);
			glVertex3f(-17.0, 6.0, -10.0);
			glEnd();

			glPopMatrix();

			// reset state
			glColor3f(0.0,0.0,0.0);
			glLineWidth(1.0);
			glPointSize(1.0);

			if (t) glEnable(GL_TEXTURE_2D);
			if (l) glEnable(GL_LIGHTING);		
		}
            }
	} // NS Utils
} // NS OpenEngine

