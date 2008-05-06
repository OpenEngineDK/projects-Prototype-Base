
#include "RapidLaserShot.h"
#include <Meta/OpenGL.h>
#include <Utils/Timer.h>
#include "../ShotManager.h"

using namespace OpenEngine::Renderers;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Prototype;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {
		RapidLaserShot::RapidLaserShot(Vector<3,float> from, Vector<3,float> to) {
			this->from = from;
			this->to = to;
			color = Vector<3,float>(1.0, 1.0, 0.0);
			lifeTime = 0.0f;
			decayTime = 2000.0f;
			shotSpeed = 0.07f;
		}

		RapidLaserShot::~RapidLaserShot() {

		}
		void RapidLaserShot::Process(const float timeSinceLast) {

			Vector<3,float> p1 = from;
			Vector<3,float> p2 = to;
			from += (p2 - p1)*shotSpeed*timeSinceLast;
			to += (p2 - p1)*shotSpeed*timeSinceLast;

			lifeTime += timeSinceLast;
			if ( lifeTime >= decayTime ) {
				Destroy();
			}
		}

		void RapidLaserShot::Destroy() {
			shotMgr->DeleteShot(this);
		}

		void RapidLaserShot::Apply(IRenderingView* view) {
			GLboolean t = glIsEnabled(GL_TEXTURE_2D);
			GLboolean l = glIsEnabled(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);

			glLineWidth(5.0);
			glPointSize(3.0);

			glColor3f(0.0, 0.0, 0.7);


			// Render the shots
			float c[3];
			float diameter = 0.25;
			color.ToArray(c);
			Vector<3,float> p1 = from;
			Vector<3,float> p2 = to;

			float height = p1.Distance(p2);
			Vector<3,float> v = p2-p1;
			float ax = 180.0/PI*acos(v[2]/height);
			if ( v[2] < 0.0) {
				ax = -ax;
			}
			if (v[2] == 0.0) v[2] = 1.0;
			float rx = -v[1]*v[2];
			float ry = v[0]*v[2];

			glPushMatrix();
			glTranslatef(p1[0],p1[1],p1[2]);
			glRotatef(ax,rx,ry,0.0);
			glScalef(1.0,1.0,height);

			glColor3fv(c);

			GLUquadricObj *q = gluNewQuadric();
			gluQuadricDrawStyle(q, (GLenum)GLU_FILL);
			gluQuadricNormals(q, (GLenum)GLU_SMOOTH);
			gluCylinder(q, diameter, diameter, 1.0, 6, 1);
			gluDeleteQuadric(q);

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

