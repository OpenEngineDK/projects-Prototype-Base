
#include "TestShot.h"
#include <Meta/OpenGL.h>
#include <Utils/Timer.h>

using namespace OpenEngine::Renderers;
//using namespace OpenEngine::Renderers::OpenGL;

namespace OpenEngine {
	namespace Utils {
		TestShot::TestShot() {
			decayTime = 2000.0f;
			shotSpeed = 0.03f;
		}

		TestShot::~TestShot() {}
                void TestShot::Initialize() {
                }
                void TestShot::Deinitialize() {
                }
                bool TestShot::IsTypeOf(const std::type_info& inf) {
                    return false;
                }
                void TestShot::Process(const float dt, const float percent) {
                    double currentTime = Timer::GetTime();
                    for (shotIter =  shots.begin(); shotIter != shots.end(); shotIter++) {
                        ShotStruct* shot = (*shotIter);
                        Vector<3,float> p1 = shot->from;
			Vector<3,float> p2 = shot->to;
                        shot->from += (p2 - p1)*shotSpeed*dt;
                        shot->to += (p2 - p1)*shotSpeed*dt;

                        if ( shot->timeCreated + decayTime < currentTime ) {
                            toDelete.push_back(shot);
                        }
                    }
                    for ( unsigned int i = 0; i < toDelete.size(); i++ ) {
                        shots.remove(toDelete[i]);
                    }
                    toDelete.clear();
                }

		void TestShot::Apply(IRenderingView* view) {
			GLboolean t = glIsEnabled(GL_TEXTURE_2D);
			GLboolean l = glIsEnabled(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);

			glLineWidth(5.0);
			glPointSize(3.0);

			glColor3f(0.0, 0.0, 0.7);


			// Render the shots
			float c[3];
			float diameter = 0.50;
			for (shotIter =  shots.begin(); shotIter != shots.end(); shotIter++) {
				ShotStruct* shot = (*shotIter);
				shot->color.ToArray(c);
				Vector<3,float> p1 = shot->from;
				Vector<3,float> p2 = shot->to;

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

				
			}

			// reset state
			glColor3f(0.0,0.0,0.0);
			glLineWidth(1.0);
			glPointSize(1.0);

			if (t) glEnable(GL_TEXTURE_2D);
			if (l) glEnable(GL_LIGHTING);		
		}

		void TestShot::AddShot(Vector<3,float> from, Vector<3,float> to, Vector<3,float> color) {
			ShotStruct* shot = new ShotStruct();
			shot->from = from;
			shot->to = to;
			shot->color = color;
			shot->timeCreated = Timer::GetTime();
			shots.push_back(shot);
		}
	} // NS Utils
} // NS OpenEngine

