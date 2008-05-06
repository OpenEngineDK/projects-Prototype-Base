
#include "LightningShot.h"
#include <Meta/OpenGL.h>
#include <Utils/Timer.h>
#include "../ShotManager.h"

using namespace OpenEngine::Renderers;
using namespace OpenEngine::Utils;
using namespace OpenEngine::Prototype;
//using namespace OpenEngine::Renderers::OpenGL;

namespace OpenEngine {
	namespace Prototype {
            namespace Weapons {
		LightningShot::LightningShot(Vector<3,float> from, Vector<3,float> to) {
			segments.push_back(new Vector<3,float>(from));
			segments.push_back(new Vector<3,float>(to));

			color = Vector<3,float>(1.0, 1.0, 1.0);
			lifeTime = 0.0f;
			decayTime = 100.0f;
			shotSpeed = 0.03f;

			randomness = 5.0;

			for ( int i = 1; i < 20; i++ ) {
				Vector<3,float> p1 = *segments[i-1];
				Vector<3,float> p2 = *segments[i];
				segments.push_back(new Vector<3,float>(p2 - p1 + p2));
			}
		}

		LightningShot::~LightningShot() {

		}

		Vector<3,float>* LightningShot::OffsetVector() {
			return new Vector<3,float>( ((float)std::rand()/RAND_MAX - 0.5) * randomness, ((float)std::rand()/RAND_MAX - 0.5) * randomness, ((float)std::rand()/RAND_MAX - 0.5) * randomness );
		}

		void LightningShot::Process(const float timeSinceLast) {

			lifeTime += timeSinceLast;
			if ( lifeTime >= decayTime) {
				Destroy();
			}
		}

		void LightningShot::Destroy() {
			segments.clear();
			shotMgr->DeleteShot(this);
		}

		void LightningShot::Apply(IRenderingView* view) {
			GLboolean t = glIsEnabled(GL_TEXTURE_2D);
			GLboolean l = glIsEnabled(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);

			glLineWidth(5.0);
			glPointSize(3.0);

			glColor3f(0.0, 0.0, 0.7);


			// Render the shots
			
			float diameter = 0.20;
			Vector<3,float> p1;
			Vector<3,float> p2;
			Vector<3,float>* lastRand = NULL;
			GLUquadricObj *q = gluNewQuadric();
			for ( unsigned int i = 1; i < segments.size(); i++ ) {
				p1 = Vector<3,float>(*segments[i-1]);
				if ( lastRand != NULL ) {
					p1 += *lastRand;
				}
				p2 = Vector<3,float>(*segments[i]);
				lastRand = OffsetVector();
				p2 += *lastRand;


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

				float random = (float)std::rand()/RAND_MAX / 2.0;
				glColor3f(color[0]-random,color[1]-random,color[2]);

				
				gluQuadricDrawStyle(q, (GLenum)GLU_FILL);
				gluQuadricNormals(q, (GLenum)GLU_SMOOTH);
				gluCylinder(q, diameter, diameter, 1.0, 3, 1);
				
				glPopMatrix();
			}
			gluDeleteQuadric(q);

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

