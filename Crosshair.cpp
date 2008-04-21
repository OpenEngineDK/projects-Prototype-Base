
#include <Meta/OpenGL.h>
#include "Crosshair.h"


using namespace OpenEngine::Renderers;
//using namespace OpenEngine::Renderers::OpenGL;

namespace OpenEngine {
	namespace Utils {

		Crosshair::Crosshair() {
		}

		Crosshair::~Crosshair() {}

		void Crosshair::Apply(IRenderingView* view) {
			GLboolean t = glIsEnabled(GL_TEXTURE_2D);
			GLboolean l = glIsEnabled(GL_LIGHTING);
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);

			glPointSize(3.0);
			glLineWidth(3.0);

			glColor3f(1.0, 1.0, 0.0);
			int count = 10;
			float spacing = 33.0;
			/*glBegin(GL_POINTS);
			for ( int i = 0; i < count; i++ ) {
			glColor3f(1.0, (float)i/count, 0.0);
			glVertex3f((float)(i+1)*spacing, 0.0, 0.0);
			}
			glEnd();*/
			glBegin(GL_QUADS);
			for ( int i = 0; i < count; i++ ) {
			glColor3f(1.0, (float)i/count, 0.0);
			glVertex3f((float)(i+1.0)*spacing, 0.0, -0.25);
			glVertex3f((float)(i+1.0)*spacing, 0.0, 0.25);
			glVertex3f((float)(i+1.5)*spacing, 0.0, 0.25);
			glVertex3f((float)(i+1.5)*spacing, 0.0, -0.25);
			}
			glEnd();

			// reset state
			glColor3f(0.0,0.0,0.0);
			glLineWidth(1.0);
			glPointSize(1.0);

			if (t) glEnable(GL_TEXTURE_2D);
			if (l) glEnable(GL_LIGHTING);	
		}
	} // NS Utils
} // NS OpenEngine

