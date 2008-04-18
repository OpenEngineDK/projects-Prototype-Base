
#include "TestShotB.h"
#include <Meta/OpenGL.h>
#include <Utils/Timer.h>

using namespace OpenEngine::Renderers;
using namespace OpenEngine::Utils;
//using namespace OpenEngine::Renderers::OpenGL;

namespace OpenEngine {
	namespace Prototype {
		TestShotB::TestShotB(Vector<3,float> from) {
			this->from = from;
			timeCreated = Timer::GetTime();
			color = Vector<3,float>(1.0, 1.0, 0.0);
			decayTime = 5000.0f;
			shotSpeed = 3.0f;
			scale = 3.0;
		}

		TestShotB::~TestShotB() {

		}
                void TestShotB::Initialize() {
                }
                void TestShotB::Deinitialize() {
                }
                bool TestShotB::IsTypeOf(const std::type_info& inf) {
                    return false;
                }
                void TestShotB::Process(const float dt, const float percent) {
			scale += shotSpeed;
                }

		void TestShotB::Apply(IRenderingView* view) {
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
			glTranslatef(p1[0],p1[1],p1[2]);
			glScalef(scale,scale,scale);

			glColor4f(color[0],color[1],color[2],0.2);

			GLUquadricObj *q = gluNewQuadric();
			gluQuadricDrawStyle(q, (GLenum)GLU_FILL);
			gluQuadricNormals(q, (GLenum)GLU_SMOOTH);
			gluSphere(q, diameter, 6, 6);
			gluDeleteQuadric(q);

			glPopMatrix();

			// reset state
			glColor3f(0.0,0.0,0.0);
			glLineWidth(1.0);
			glPointSize(1.0);

			if (t) glEnable(GL_TEXTURE_2D);
			if (l) glEnable(GL_LIGHTING);		
		}
	} // NS Utils
} // NS OpenEngine

