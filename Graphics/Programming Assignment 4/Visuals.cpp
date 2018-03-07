#include "Visuals.h"
#include <GL/glut.h>

///////////////drawPlane and drawPlaneLocalAxes///////////////
void Visuals::drawPlane()
{
	glBegin(GL_TRIANGLES);
		///////////////////////////////front section - red
		glColor3f(1.0, 0.0, 0.0);
				//side view - furthest
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.4, -0.12, -0.1);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, -0.12, -0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, 0.1, -0.1);

		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, -0.12, -0.1);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.2, -0.07, -0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.26, -0.07, -0.1);

				//side view - closest
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.4, -0.12, 0.1);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, -0.12, 0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, 0.1, 0.1);

		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, -0.12, 0.1);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.2, -0.07, 0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, -0.07, 0.1);
	glEnd();

	glBegin(GL_QUADS);	
				//front
		glNormal3f(-1.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.4, -0.12, -0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.4, -0.12, 0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.26, 0.1, 0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.26, 0.1, -0.1);

				//bottom - flat
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.4, -0.12, -0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.4, -0.12, 0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.26, -0.12, 0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.26, -0.12, -0.1);

				//bottom - angled
		glNormal3f(1.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, -0.12, -0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, -0.12, 0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.2, -0.07, 0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.2, -0.07, -0.1);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		/////////////////////////////bridge - green
		glColor3f(0.0, 1.0, 0.0);
				//side view - furthest
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.37, 0.07, -0.05);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, 0.07, -0.05);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.37, -0.1, -0.05);

				//side view - closest
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.37, 0.07, 0.05);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, 0.07, 0.05);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.37, -0.1, 0.05);
	glEnd();

	glBegin(GL_QUADS);
				//top
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.37, 0.07, -0.05);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, 0.07, -0.05);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.26, 0.07, 0.05);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.37, 0.07, 0.05);

				//front
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.37, -0.1, -0.05);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.37, 0.07, -0.05);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.37, 0.07, 0.05);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.37, -0.1, 0.05);

		///////////////////////////////main section - blue
		glColor3f(0.0, 0.0, 1.0);
				//side view - furthest
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, -0.07, -0.1);
		glTexCoord2f(0.0, 1.0);  glVertex3f(-0.26, 0.1, -0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.4, 0.1, -0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.4, -0.07, -0.1);

				//side view - closest
		glNormal3f(0.5, 0.5, 1.0); glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, -0.07, 0.1);
		glNormal3f(1.0, 0.0, 1.0); glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, 0.1, 0.1);
		glNormal3f(0.0, 1.0, 1.0); glTexCoord2f(1.0, 1.0); glVertex3f(0.4, 0.1, 0.1);
		glNormal3f(0.5, 0.5, 1.0); glTexCoord2f(1.0, 0.0);  glVertex3f(0.4, -0.07, 0.1);

				//bottom
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, -0.07, -0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, -0.07, 0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.4, -0.07, 0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.4, -0.07, -0.1);

				//top
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.26, 0.1, -0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.26, 0.1, 0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.4, 0.1, 0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.4, 0.1, -0.1);

				//back
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.4, -0.07, -0.1);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.4, -0.07, 0.1);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.4, 0.1, 0.1);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.4, 0.1, -0.1);

		/////////////////////////////top section - white
		glColor3f(1.0, 1.0, 1.0);
				//side view - furthest
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.15, -0.2);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.07, -0.2);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, 0.07, -0.2);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.35, 0.15, -0.2);

				//side view - closest
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.15, 0.2);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.07, 0.2);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, 0.07, 0.2);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.35, 0.15, 0.2);

				//front
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.07, -0.2);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.0, 0.15, -0.2);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.0, 0.15, 0.2);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.07, 0.2);

				//back
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.35, 0.07, -0.2);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.35, 0.15, -0.2);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, 0.15, 0.2);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.35, 0.07, 0.2);
	
				//top
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.15, -0.2);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.35, 0.15, -0.2);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, 0.15, 0.2);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.15, 0.2);

				//bottom
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.0, 0.07, -0.2);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.35, 0.07, -0.2);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, 0.07, 0.2);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.0, 0.07, 0.2);

		//////////////////////////////landing bays - black
		glColor3f(0.0, 0.0, 0.0);
				//side view - furthest
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.03, -0.16, -0.3);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.03, -0.01, -0.3);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, -0.01, -0.3);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.35, -0.16, -0.3);

				//side view - closest
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.03, -0.16, 0.3);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.03, -0.01, 0.3);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, -0.01, 0.3);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.35, -0.16, 0.3);

				//top
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.03, -0.01, -0.3);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.03, -0.01, 0.3);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, -0.01, 0.3);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.35, -0.01, -0.3);

				//bottom
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.03, -0.16, -0.3);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.03, -0.16, 0.3);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, -0.16, 0.3);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.35, -0.16, -0.3);

				//front
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-0.03, -0.16, -0.3);
		glTexCoord2f(0.0, 1.0); glVertex3f(-0.03, -0.01, -0.3);
		glTexCoord2f(1.0, 1.0); glVertex3f(-0.03, -0.01, 0.3);
		glTexCoord2f(1.0, 0.0); glVertex3f(-0.03, -0.16, 0.3);

				//back
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.35, -0.16, -0.3);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.35, -0.01, -0.3);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.35, -0.01, 0.3);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.35, -0.16, 0.3);

		//////////////////////////////satellite mount - black
			//side view - furthest
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.175, 0.15, -0.004);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.183, 0.15, -0.004);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.183, 0.22, -0.004);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.175, 0.22, -0.004);
			//side view - closest
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.175, 0.15, 0.004);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.183, 0.15, 0.004);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.183, 0.22, 0.004);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.175, 0.22, 0.004);
			//front
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.175, 0.15, -0.004);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.175, 0.15, 0.004);
		glTexCoord2f(1.0, 1.0);glVertex3f(0.175, 0.22, 0.004);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.175, 0.22, -0.004);
			//back
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.183, 0.15, -0.004);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.183, 0.15, 0.004);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.183, 0.22, 0.004);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.183, 0.22, -0.004);
			//top
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.175, 0.22, -0.004);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.175, 0.22, 0.004);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.183, 0.22, 0.004);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.183, 0.22, -0.004);
	glEnd();
}//end drawObjectModelView


//////////////////Draw Guns and Barrels////////////////////////
void Visuals::drawGun()
{
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
			//side view - furthest
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(-0.05, -0.025, -0.05);
		glVertex3f(-0.05, 0.025, -0.05);
		glVertex3f(0.05, 0.025, -0.05);
		glVertex3f(0.05, -0.025, -0.05);
			//side view - closest
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-0.05, -0.025, 0.05);
		glVertex3f(-0.05, 0.025, 0.05);
		glVertex3f(0.05, 0.025, 0.05);
		glVertex3f(0.05, -0.025, 0.05);
			//top
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-0.05, 0.025, -0.05);
		glVertex3f(-0.05, 0.025, 0.05);
		glVertex3f(0.05, 0.025, 0.05);
		glVertex3f(0.05, 0.025, -0.05);
			//front
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-0.05, -0.025, -0.05);
		glVertex3f(-0.05, 0.025, -0.05);
		glVertex3f(-0.05, 0.025, 0.05);
		glVertex3f(-0.05, -0.025, 0.05);
			//back
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(0.05, -0.025, -0.05);
		glVertex3f(0.05, 0.025, -0.05);
		glVertex3f(0.05, 0.025, 0.05);
		glVertex3f(0.05, -0.025, 0.05);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_TRIANGLES);
			//side view - furthest
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-0.05, 0.025, -0.04001);
		glVertex3f(0.05, 0.025, -0.04001);
		glVertex3f(0.0, 0.055, -0.04001);
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(-0.05, 0.025, -0.048);
		glVertex3f(0.05, 0.025, -0.048);
		glVertex3f(0.0, 0.055, -0.048);
			//side view - mid furthest
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(-0.05, 0.025, -0.02001);
		glVertex3f(0.05, 0.025, -0.02001);
		glVertex3f(0.0, 0.055, -0.02001);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-0.05, 0.025, -0.012);
		glVertex3f(0.05, 0.025, -0.012);
		glVertex3f(0.0, 0.055, -0.012);
			//side view - closest
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(-0.05, 0.025, 0.04001);
		glVertex3f(0.05, 0.025, 0.04001);
		glVertex3f(0.0, 0.055, 0.04001);
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-0.05, 0.025, 0.048);
		glVertex3f(0.05, 0.025, 0.048);
		glVertex3f(0.0, 0.055, 0.048);

			//side view - mid closest
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-0.05, 0.025, 0.02001);
		glVertex3f(0.05, 0.025, 0.02001);
		glVertex3f(0.0, 0.055, 0.02001);
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(-0.05, 0.025, 0.012);
		glVertex3f(0.05, 0.025, 0.012);
		glVertex3f(0.0, 0.055, 0.012);
	glEnd();

	glBegin(GL_QUADS);
			//furthest
	    glNormal3f(-1.0, 1.0, 0.0);
		glVertex3f(-0.05, 0.025, -0.048);
		glVertex3f(-0.05, 0.025, -0.04001);
		glVertex3f(0.0, 0.055, -0.04001);
		glVertex3f(0.0, 0.055, -0.048);
		glNormal3f(1.0, 1.0, 0.0);
		glVertex3f(0.05, 0.025, -0.048);
		glVertex3f(0.05, 0.025, -0.04001);
		glVertex3f(0.0, 0.055, -0.04001);
		glVertex3f(0.0, 0.055, -0.048);
			//mid furthest
		glNormal3f(-1.0, 1.0, 0.0);
		glVertex3f(-0.05, 0.025, -0.02001);
		glVertex3f(-0.05, 0.025, -0.012);
		glVertex3f(0.0, 0.055, -0.012);
		glVertex3f(0.0, 0.055, -0.02001);
		glNormal3f(1.0, 1.0, 0.0);
		glVertex3f(0.05, 0.025, -0.02001);
		glVertex3f(0.05, 0.025, -0.012);
		glVertex3f(0.0, 0.055, -0.012);
		glVertex3f(0.0, 0.055, -0.02001);
			//mid closest
		glNormal3f(-1.0, 1.0, 0.0);
		glVertex3f(-0.05, 0.025, 0.012);
		glVertex3f(-0.05, 0.025, 0.02001);
		glVertex3f(0.0, 0.055, 0.02001);
		glVertex3f(0.0, 0.055, 0.012);
		glNormal3f(1.0, 1.0, 0.0);
		glVertex3f(0.05, 0.025, 0.012);
		glVertex3f(0.05, 0.025, 0.02001);
		glVertex3f(0.0, 0.055, 0.02001);
		glVertex3f(0.0, 0.055, 0.012);
			//closest
		glNormal3f(-1.0, 1.0, 0.0);
		glVertex3f(-0.05, 0.025, 0.04001);
		glVertex3f(-0.05, 0.025, 0.048);
		glVertex3f(0.0, 0.055, 0.048);
		glVertex3f(0.0, 0.055, 0.04001);
		glNormal3f(1.0, 1.0, 0.0);
		glVertex3f(0.05, 0.025, 0.04001);
		glVertex3f(0.05, 0.025, 0.048);
		glVertex3f(0.0, 0.055, 0.048);
		glVertex3f(0.0, 0.055, 0.04001);
	glEnd();
}//drawGun

void Visuals::drawBarrel()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	//furthest barrel
			//side view - furthest
	    glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(-0.01, 0.0, -0.04);
		glVertex3f(-0.01, 0.1, -0.04);
		glVertex3f(0.01, 0.1, -0.04);
		glVertex3f(0.01, 0.0, -0.04);
			//side view - closest
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-0.01, 0.0, -0.02);
		glVertex3f(-0.01, 0.1, -0.02);
		glVertex3f(0.01, 0.1, -0.02);
		glVertex3f(0.01, 0.0, -0.02);
			//bottom
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(-0.01, 0.0, -0.04);
		glVertex3f(-0.01, 0.0, -0.02);
		glVertex3f(0.01, 0.0, -0.02);
		glVertex3f(0.01, 0.0, -0.04);
			//top
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-0.01, 0.1, -0.04);
		glVertex3f(-0.01, 0.1, -0.02);
		glVertex3f(0.01, 0.1, -0.02);
		glVertex3f(0.01, 0.1, -0.04);
			//front
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-0.01, 0.0, -0.04);
		glVertex3f(-0.01, 0.1, -0.04);
		glVertex3f(-0.01, 0.1, -0.02);
		glVertex3f(-0.01, 0.0, -0.02);
			//back
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(0.01, 0.0, -0.04);
		glVertex3f(0.01, 0.1, -0.04);
		glVertex3f(0.01, 0.1, -0.02);
		glVertex3f(0.01, 0.0, -0.02);

	//closest barrel
			//side view - closest
		glNormal3f(0.0, 0.0, 1.0);
		glVertex3f(-0.01, 0.0, 0.04);
		glVertex3f(-0.01, 0.1, 0.04);
		glVertex3f(0.01, 0.1, 0.04);
		glVertex3f(0.01, 0.0, 0.04);
			//side view - furthest
		glNormal3f(0.0, 0.0, -1.0);
		glVertex3f(-0.01, 0.0, 0.02);
		glVertex3f(-0.01, 0.1, 0.02);
		glVertex3f(0.01, 0.1, 0.02);
		glVertex3f(0.01, 0.0, 0.02);
			//bottom
		glNormal3f(0.0, -1.0, 0.0);
		glVertex3f(-0.01, 0.0, 0.04);
		glVertex3f(-0.01, 0.0, 0.02);
		glVertex3f(0.01, 0.0, 0.02);
		glVertex3f(0.01, 0.0, 0.04);
			//top
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3f(-0.01, 0.1, 0.04);
		glVertex3f(-0.01, 0.1, 0.02);
		glVertex3f(0.01, 0.1, 0.02);
		glVertex3f(0.01, 0.1, 0.04);
			//front
		glNormal3f(-1.0, 0.0, 0.0);
		glVertex3f(-0.01, 0.0, 0.04);
		glVertex3f(-0.01, 0.1, 0.04);
		glVertex3f(-0.01, 0.1, 0.02);
		glVertex3f(-0.01, 0.0, 0.02);
			//back
		glNormal3f(1.0, 0.0, 0.0);
		glVertex3f(0.01, 0.0, 0.04);
		glVertex3f(0.01, 0.1, 0.04);
		glVertex3f(0.01, 0.1, 0.02);
		glVertex3f(0.01, 0.0, 0.02);
	glEnd();
}//drawBarrel

void Visuals::drawSatellite()
{
	glBegin(GL_TRIANGLES);
			//side view - furthest
		glColor3f(1.0,1.0,0.0);
		glNormal3f(1.0, 0.0, -1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.05, -0.03, -0.03);
		glVertex3f(-0.05, 0.03, -0.03);
			//side view - closest
		glColor3f(1.0,0.0,1.0);
		glNormal3f(1.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.05, -0.03, 0.03);
		glVertex3f(-0.05, 0.03, 0.03);
			//top
		glColor3f(0.0,1.0,1.0);
		glNormal3f(1.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.05, 0.03, -0.03);
		glVertex3f(-0.05, 0.03, 0.03);
			//bottom
		glColor3f(0.0,0.0,0.0);
		glNormal3f(1.0, -1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.05, -0.03, -0.03);
		glVertex3f(-0.05, -0.03, 0.03);
	glEnd();
}//drawSatellite

void Visuals::drawCamera()
{
	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_QUADS);
		//side view - closest
		glVertex3f(0.0, 0.1, -0.1);
		glVertex3f(0.0, -0.1, -0.1);
		glVertex3f(0.2, -0.1, -0.1);
		glVertex3f(0.2, 0.1, -0.1);

		//side view - furthest
		glVertex3f(0.0, 0.1, 0.1);
		glVertex3f(0.0, -0.1, 0.1);
		glVertex3f(0.2, -0.1, 0.1);
		glVertex3f(0.2, 0.1, 0.1);

		//top
		glVertex3f(0.0, 0.1, -0.1);
		glVertex3f(0.0, 0.1, 0.1);
		glVertex3f(0.2, 0.1, 0.1);
		glVertex3f(0.2, 0.1, -0.1);

		//bottom
		glVertex3f(0.0, -0.1, -0.1);
		glVertex3f(0.0, -0.1, 0.1);
		glVertex3f(0.2, -0.1, 0.1);
		glVertex3f(0.2, -0.1, -0.1);

		//left 
		glVertex3f(0.0, 0.1, -0.1);
		glVertex3f(0.0, -0.1, -0.1);
		glVertex3f(0.0, -0.1, 0.1);
		glVertex3f(0.0, 0.1, 0.1);

		//right
		glVertex3f(0.2, 0.1, -0.1);
		glVertex3f(0.2, -0.1, -0.1);
		glVertex3f(0.2, -0.1, 0.1);
		glVertex3f(0.2, 0.1, 0.1);
	glEnd();

	glBegin(GL_TRIANGLES);
		//side view - closest
		glColor3f(1.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.2, 0.1, -0.1);
		glVertex3f(-0.2, -0.1, -0.1);
		
		//side view - furthest
		glColor3f(1.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.2, 0.1, 0.1);
		glVertex3f(-0.2, -0.1, 0.1);

		//top
		glColor3f(0.0, 1.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.2, 0.1, -0.1);
		glVertex3f(-0.2, 0.1, 0.1);

		//bottom
		glColor3f(1.0, 1.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.2, -0.1, -0.1);
		glVertex3f(-0.2, -0.1, 0.1);
	glEnd();
}//end drawCamera

void Visuals::drawLight()
{
	glutSolidSphere(0.1, 1000, 1000);
}//end drawLight

void Visuals::drawCube()
{
	glBegin(GL_QUADS);
		//near face
		glColor3f(1.0, 1.0, 0.0);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -2.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 1.0, -2.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.5, 1.0, -2.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 0.0, -2.0);

		//far face
		glColor3f(0.0, 1.0, 1.0);
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -3.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 1.0, -3.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.5, 1.0, -3.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 0.0, -3.0);

		//front face
		glColor3f(1.0, 0.0, 1.0);
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -2.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 1.0, -2.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(0.5, 1.0, -3.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.0, -3.0);
		
		//back face
		glColor3f(0.0, 0.0, 1.0);
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(1.5, 0.0, -2.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(1.5, 1.0, -2.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.5, 1.0, -3.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 0.0, -3.0);
		
		//top face
		glColor3f(0.0, 1.0, 0.0);
		glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 1.0, -2.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 1.0, -3.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.5, 1.0, -3.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 1.0, -2.0);

		//bottom face
		glColor3f(1.0, 0.0, 0.0);
		glNormal3f(0.0, -1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(0.5, 0.0, -2.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(0.5, 0.0, -3.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(1.5, 0.0, -3.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(1.5, 0.0, -2.0);
}//end drawCube

void Visuals::drawNearFace()
{
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, -1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, 5.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, 5.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, 5.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, 5.0);
	glEnd();
}//end drawNearFace

void Visuals::drawFarFace()
{
	glBegin(GL_QUADS);
		glNormal3f(0.0, 0.0, 1.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, -5.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, -5.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, -5.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, -5.0);
	glEnd();
}//end drawFarFace

void Visuals::drawLeftFace()
{
	glBegin(GL_QUADS);
		glNormal3f(1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, -5.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, -5.0, 5.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(-5.0, 5.0, 5.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(-5.0, 5.0, -5.0);
	glEnd();
}//end drawLeftFace

void Visuals::drawRightFace()
{
	glBegin(GL_QUADS);
		glNormal3f(-1.0, 0.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(5.0, 5.0, -5.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(5.0, 5.0, 5.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(5.0, -5.0, 5.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, -5.0);
	glEnd();
}//end drawRightFace

void Visuals::drawTopFace()
{
	glBegin(GL_QUADS);
	glNormal3f(0.0,-1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, 5.0, 5.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 5.0, -5.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 5.0, -5.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(5.0, 5.0, 5.0);
	glEnd();
}//end drawTopFace

void Visuals::drawBottomFace()
{
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
		glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, -5.0, 5.0);
		glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, -5.0, -5.0);
		glTexCoord2f(1.0, 1.0); glVertex3f(5.0, -5.0, -5.0);
		glTexCoord2f(1.0, 0.0); glVertex3f(5.0, -5.0, 5.0);
	glEnd();
}//end drawBottomFace

void Visuals::drawCameraLocalAxes()
{
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(-0.5, 0.0, 0.0);

		//y-axis - green
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.5, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, -0.5);
	glEnd();
}//end drawCameraLocalAxes

void Visuals::drawPlaneLocalAxes()
{
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-0.75, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);

		//y-axis - green
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.75, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, -0.75);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}//drawPlaneLocalAxes


void Visuals::drawLeftWorldAxes()
{
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(X_MAX, 0.0, 0.0);

		//y-axis - green
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, Y_MAX, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, Z_MAX);
	glEnd();
}//end drawLeftWorldAxes

void Visuals::drawRightWorldAxes()
{
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(X_MIN, 0.0, 0.0);
		glVertex3f(X_MAX, 0.0, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, Y_MIN);
		glVertex3f(0.0, 0.0, Y_MAX);

		//viewport divider - black
		glColor3f(0.0, 0.0, 0.0);
		glVertex3f(X_MIN, Y_MIN, Z_MAX);
		glVertex3f(X_MIN, Y_MAX, Z_MAX);
	glEnd();

	glBegin(GL_QUADS);
		//y-axis - green, appears as a diamond
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.1, Z_MAX, 0.0);
		glVertex3f(0.0, Z_MAX, 0.1);
		glVertex3f(-0.1, Z_MAX, 0.0);
		glVertex3f(0.0, Z_MAX, -0.1);
	glEnd();
}//drawRightWorldAxes

void Visuals::drawSmallAxes()
{	
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(-0.1, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);

		//y-axis - green
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.1, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, -0.1);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}//drawSmallAxes
