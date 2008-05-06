
#include "Landmine.h"
#include <Meta/OpenGL.h>
#include <Utils/Timer.h>
#include "../ShotManager.h"

using namespace OpenEngine::Renderers;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Prototype;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {
		Landmine::Landmine(Vector<3,float> from) {
			this->from = from;
			color = Vector<3,float>(0.3, 0.3, 0.3);
			lifeTime = 0.0f;
			decayTime = 2000.0f;
			shotSpeed = 0.1f;
			scale = 1.0;
			exploding = false;
		}

		Landmine::~Landmine() {

		}

		void Landmine::Process(const float timeSinceLast) {
			if ( exploding ) {
				scale += shotSpeed * timeSinceLast;
			}

			lifeTime += timeSinceLast;
			if ( lifeTime >= decayTime ) {
				Destroy();
			}
		}

		void Landmine::Destroy() {
			if ( !exploding ) {
				exploding = true;
				decayTime = 750.0f;
				lifeTime = 0.0f;
			} else {
				shotMgr->DeleteShot(this);
			}
		}

		void Landmine::Apply(IRenderingView* view) {
			GLboolean t = glIsEnabled(GL_TEXTURE_2D);
			GLboolean l = glIsEnabled(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);

			glLineWidth(5.0);
			glPointSize(3.0);

			glColor3f(0.0, 0.0, 0.7);

			// Render the shots
			float diameter = 1.0;
			Vector<3,float> p1 = from;

			glPushMatrix();			
			glTranslatef(p1[0],p1[1]+2.0,p1[2]);
			if ( !exploding ) {
				glRotatef(-90,1.0,0.0,0.0);
				glColor3f(color[0],color[1],color[2]);
				GLUquadricObj *q = gluNewQuadric();
				gluQuadricDrawStyle(q, (GLenum)GLU_FILL);
				gluQuadricNormals(q, (GLenum)GLU_SMOOTH);
				//base
				gluCylinder(q, diameter*2, diameter*2, 2.0, 6, 1);
				//lamp
				glPushMatrix();	
				glTranslatef(0.0,0.0,2.0);
				glColor3f(1.0,0.0,0.0);
				gluSphere(q, diameter, 6, 6);
				glPopMatrix();
				gluDeleteQuadric(q);
			} else {
				glColor3f(1.0,1.0,0.0);
				glScalef(scale,scale,scale);
				GLUquadricObj *q = gluNewQuadric();
				gluQuadricDrawStyle(q, (GLenum)GLU_FILL);
				gluQuadricNormals(q, (GLenum)GLU_SMOOTH);
				gluSphere(q, diameter, 6, 6);
				gluDeleteQuadric(q);
			}

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

