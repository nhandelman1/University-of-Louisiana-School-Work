#include "Visuals.h"
#include <GL/glut.h>

///////////////drawPlane and drawPlaneLocalAxes///////////////
void Visuals::drawPlane()
{
	glBegin(GL_TRIANGLES);
		///////////////////////////////front section - red
		glColor3f(1.0, 0.0, 0.0);
				//side view - closest
		glVertex3f(-0.4, -0.12, -0.1);
		glVertex3f(-0.26, -0.12, -0.1);
		glVertex3f(-0.26, 0.1, -0.1);

		glVertex3f(-0.26, -0.12, -0.1);
		glVertex3f(-0.2, -0.07, -0.1);
		glVertex3f(-0.26, -0.07, -0.1);

				//side view - furthest
		glVertex3f(-0.4, -0.12, 0.1);
		glVertex3f(-0.26, -0.12, 0.1);
		glVertex3f(-0.26, 0.1, 0.1);

		glVertex3f(-0.26, -0.12, 0.1);
		glVertex3f(-0.2, -0.07, 0.1);
		glVertex3f(-0.26, -0.07, 0.1);
	glEnd();

	glBegin(GL_QUADS);	
				//front
		glVertex3f(-0.4, -0.12, -0.1);
		glVertex3f(-0.4, -0.12, 0.1);
		glVertex3f(-0.26, 0.1, 0.1);
		glVertex3f(-0.26, 0.1, -0.1);

				//bottom - flat
		glVertex3f(-0.4, -0.12, -0.1);
		glVertex3f(-0.4, -0.12, 0.1);
		glVertex3f(-0.26, -0.12, 0.1);
		glVertex3f(-0.26, -0.12, -0.1);

				//bottom - angled
		glVertex3f(-0.26, -0.12, -0.1);
		glVertex3f(-0.26, -0.12, 0.1);
		glVertex3f(-0.2, -0.07, 0.1);
		glVertex3f(-0.2, -0.07, -0.1);
	glEnd();
	
	glBegin(GL_TRIANGLES);
		/////////////////////////////bridge - green
		glColor3f(0.0, 1.0, 0.0);
				//side view - closest
		glVertex3f(-0.37, 0.07, -0.05);
		glVertex3f(-0.26, 0.07, -0.05);
		glVertex3f(-0.37, -0.1, -0.05);

				//side view - furthest
		glVertex3f(-0.37, 0.07, 0.05);
		glVertex3f(-0.26, 0.07, 0.05);
		glVertex3f(-0.37, -0.1, 0.05);
	glEnd();

	glBegin(GL_QUADS);
				//top
		glVertex3f(-0.37, 0.07, -0.05);
		glVertex3f(-0.26, 0.07, -0.05);
		glVertex3f(-0.26, 0.07, 0.05);
		glVertex3f(-0.37, 0.07, 0.05);

				//front
		glVertex3f(-0.37, -0.1, -0.05);
		glVertex3f(-0.37, 0.07, -0.05);
		glVertex3f(-0.37, 0.07, 0.05);
		glVertex3f(-0.37, -0.1, 0.05);

		///////////////////////////////main section - blue
		glColor3f(0.0, 0.0, 1.0);
				//side view - closest
		glVertex3f(-0.26, -0.07, -0.1);
		glVertex3f(-0.26, 0.1, -0.1);
		glVertex3f(0.4, 0.1, -0.1);
		glVertex3f(0.4, -0.07, -0.1);

				//side view - furthest
		glVertex3f(-0.26, -0.07, 0.1);
		glVertex3f(-0.26, 0.1, 0.1);
		glVertex3f(0.4, 0.1, 0.1);
		glVertex3f(0.4, -0.07, 0.1);

				//bottom
		glVertex3f(-0.26, -0.07, -0.1);
		glVertex3f(-0.26, -0.07, 0.1);
		glVertex3f(0.4, -0.07, 0.1);
		glVertex3f(0.4, -0.07, -0.1);

				//top
		glVertex3f(-0.26, 0.1, -0.1);
		glVertex3f(-0.26, 0.1, 0.1);
		glVertex3f(0.4, 0.1, 0.1);
		glVertex3f(0.4, 0.1, -0.1);

				//back
		glVertex3f(0.4, -0.07, -0.1);
		glVertex3f(0.4, -0.07, 0.1);
		glVertex3f(0.4, 0.1, 0.1);
		glVertex3f(0.4, 0.1, -0.1);

		/////////////////////////////top section - white
		glColor3f(1.0, 1.0, 1.0);
				//side view - closest
		glVertex3f(0.0, 0.15, -0.2);
		glVertex3f(0.0, 0.07, -0.2);
		glVertex3f(0.35, 0.07, -0.2);
		glVertex3f(0.35, 0.15, -0.2);

				//side view - furthest
		glVertex3f(0.0, 0.15, 0.2);
		glVertex3f(0.0, 0.07, 0.2);
		glVertex3f(0.35, 0.07, 0.2);
		glVertex3f(0.35, 0.15, 0.2);

				//front
		glVertex3f(0.0, 0.07, -0.2);
		glVertex3f(0.0, 0.15, -0.2);
		glVertex3f(0.0, 0.15, 0.2);
		glVertex3f(0.0, 0.07, 0.2);

				//back
		glVertex3f(0.35, 0.07, -0.2);
		glVertex3f(0.35, 0.15, -0.2);
		glVertex3f(0.35, 0.15, 0.2);
		glVertex3f(0.35, 0.07, 0.2);
	
				//top
		glVertex3f(0.0, 0.15, -0.2);
		glVertex3f(0.35, 0.15, -0.2);
		glVertex3f(0.35, 0.15, 0.2);
		glVertex3f(0.0, 0.15, 0.2);

				//bottom
		glVertex3f(0.0, 0.07, -0.2);
		glVertex3f(0.35, 0.07, -0.2);
		glVertex3f(0.35, 0.07, 0.2);
		glVertex3f(0.0, 0.07, 0.2);

		//////////////////////////////landing bays - black
		glColor3f(0.0, 0.0, 0.0);
				//side view - closest
		glVertex3f(-0.03, -0.16, -0.3);
		glVertex3f(-0.03, -0.01, -0.3);
		glVertex3f(0.35, -0.01, -0.3);
		glVertex3f(0.35, -0.16, -0.3);

				//side view - furthest
		glVertex3f(-0.03, -0.16, 0.3);
		glVertex3f(-0.03, -0.01, 0.3);
		glVertex3f(0.35, -0.01, 0.3);
		glVertex3f(0.35, -0.16, 0.3);

				//top
		glVertex3f(-0.03, -0.01, -0.3);
		glVertex3f(-0.03, -0.01, 0.3);
		glVertex3f(0.35, -0.01, 0.3);
		glVertex3f(0.35, -0.01, -0.3);

				//bottom
		glVertex3f(-0.03, -0.16, -0.3);
		glVertex3f(-0.03, -0.16, 0.3);
		glVertex3f(0.35, -0.16, 0.3);
		glVertex3f(0.35, -0.16, -0.3);

				//front
		glVertex3f(-0.03, -0.16, -0.3);
		glVertex3f(-0.03, -0.01, -0.3);
		glVertex3f(-0.03, -0.01, 0.3);
		glVertex3f(-0.03, -0.16, 0.3);

				//back
		glVertex3f(0.35, -0.16, -0.3);
		glVertex3f(0.35, -0.01, -0.3);
		glVertex3f(0.35, -0.01, 0.3);
		glVertex3f(0.35, -0.16, 0.3);
	glEnd();
}//end drawObjectModelView

void Visuals::drawPlaneLocalAxes()
{
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0.0, 0.0);
		glVertex3f(X_MAX/2, 0.0, 0.0);

		//y-axis - green
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0, 0.0);
		glVertex3f(0.0, Y_MAX/2, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(0.0, 0.0, Z_MAX/2);
	glEnd();
}//drawLocalAxes

//////////////////Draw Guns and Barrels////////////////////////
void Visuals::drawGunTop()
{
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
			//side view - closest
		glVertex3f(-0.2, 0.1, -0.05);
		glVertex3f(-0.2, 0.15, -0.05);
		glVertex3f(-0.1, 0.15, -0.05);
		glVertex3f(-0.1, 0.1, -0.05);
			//side view - furthest
		glVertex3f(-0.2, 0.1, 0.05);
		glVertex3f(-0.2, 0.15, 0.05);
		glVertex3f(-0.1, 0.15, 0.05);
		glVertex3f(-0.1, 0.1, 0.05);
			//top
		glVertex3f(-0.2, 0.15, -0.05);
		glVertex3f(-0.2, 0.15, 0.05);
		glVertex3f(-0.1, 0.15, 0.05);
		glVertex3f(-0.1, 0.15, -0.05);
			//front
		glVertex3f(-0.2, 0.1, -0.05);
		glVertex3f(-0.2, 0.15, -0.05);
		glVertex3f(-0.2, 0.15, 0.05);
		glVertex3f(-0.2, 0.1, 0.05);
			//back
		glVertex3f(-0.1, 0.1, -0.05);
		glVertex3f(-0.1, 0.15, -0.05);
		glVertex3f(-0.1, 0.15, 0.05);
		glVertex3f(-0.1, 0.1, 0.05);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
			//side view - closest
		glVertex3f(-0.2, 0.15, -0.04001);
		glVertex3f(-0.1, 0.15, -0.04001);
		glVertex3f(-0.15, 0.18, -0.04001);
		glVertex3f(-0.2, 0.15, -0.048);
		glVertex3f(-0.1, 0.15, -0.048);
		glVertex3f(-0.15, 0.18, -0.048);
			//side view - mid closest
		glVertex3f(-0.2, 0.15, -0.02001);
		glVertex3f(-0.1, 0.15, -0.02001);
		glVertex3f(-0.15, 0.18, -0.02001);
		glVertex3f(-0.2, 0.15, -0.012);
		glVertex3f(-0.1, 0.15, -0.012);
		glVertex3f(-0.15, 0.18, -0.012);
			//side view - furthest
		glVertex3f(-0.2, 0.15, 0.04001);
		glVertex3f(-0.1, 0.15, 0.04001);
		glVertex3f(-0.15, 0.18, 0.04001);
		glVertex3f(-0.2, 0.15, 0.048);
		glVertex3f(-0.1, 0.15, 0.048);
		glVertex3f(-0.15, 0.18, 0.048);
			//side view - mid furthest
		glVertex3f(-0.2, 0.15, 0.02001);
		glVertex3f(-0.1, 0.15, 0.02001);
		glVertex3f(-0.15, 0.18, 0.02001);
		glVertex3f(-0.2, 0.15, 0.012);
		glVertex3f(-0.1, 0.15, 0.012);
		glVertex3f(-0.15, 0.18, 0.012);
	glEnd();

	glBegin(GL_QUADS);
			//closest
		glVertex3f(-0.2, 0.15, -0.048);
		glVertex3f(-0.2, 0.15, -0.04001);
		glVertex3f(-0.15, 0.18, -0.04001);
		glVertex3f(-0.15, 0.18, -0.048);
		glVertex3f(-0.1, 0.15, -0.048);
		glVertex3f(-0.1, 0.15, -0.04001);
		glVertex3f(-0.15, 0.18, -0.04001);
		glVertex3f(-0.15, 0.18, -0.048);
			//mid closest
		glVertex3f(-0.2, 0.15, -0.02001);
		glVertex3f(-0.2, 0.15, -0.012);
		glVertex3f(-0.15, 0.18, -0.012);
		glVertex3f(-0.15, 0.18, -0.02001);
		glVertex3f(-0.1, 0.15, -0.02001);
		glVertex3f(-0.1, 0.15, -0.012);
		glVertex3f(-0.15, 0.18, -0.012);
		glVertex3f(-0.15, 0.18, -0.02001);
			//furthest
		glVertex3f(-0.2, 0.15, 0.012);
		glVertex3f(-0.2, 0.15, 0.02001);
		glVertex3f(-0.15, 0.18, 0.02001);
		glVertex3f(-0.15, 0.18, 0.012);
		glVertex3f(-0.1, 0.15, 0.012);
		glVertex3f(-0.1, 0.15, 0.02001);
		glVertex3f(-0.15, 0.18, 0.02001);
		glVertex3f(-0.15, 0.18, 0.012);
			//mid furthest
		glVertex3f(-0.2, 0.15, 0.04001);
		glVertex3f(-0.2, 0.15, 0.048);
		glVertex3f(-0.15, 0.18, 0.048);
		glVertex3f(-0.15, 0.18, 0.04001);
		glVertex3f(-0.1, 0.15, 0.04001);
		glVertex3f(-0.1, 0.15, 0.048);
		glVertex3f(-0.15, 0.18, 0.048);
		glVertex3f(-0.15, 0.18, 0.04001);
	glEnd();
}//drawGun

void Visuals::drawBarrelTop()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	//furthest barrel
			//side view - closest
		glVertex3f(-0.16, 0.16, -0.04);
		glVertex3f(-0.16, 0.26, -0.04);
		glVertex3f(-0.14, 0.26, -0.04);
		glVertex3f(-0.14, 0.16, -0.04);
			//side view - furthest
		glVertex3f(-0.16, 0.16, -0.02);
		glVertex3f(-0.16, 0.26, -0.02);
		glVertex3f(-0.14, 0.26, -0.02);
		glVertex3f(-0.14, 0.16, -0.02);
			//bottom
		glVertex3f(-0.16, 0.16, -0.04);
		glVertex3f(-0.16, 0.16, -0.02);
		glVertex3f(-0.14, 0.16, -0.02);
		glVertex3f(-0.14, 0.16, -0.04);
			//top
		glVertex3f(-0.16, 0.26, -0.04);
		glVertex3f(-0.16, 0.26, -0.02);
		glVertex3f(-0.14, 0.26, -0.02);
		glVertex3f(-0.14, 0.26, -0.04);
			//front
		glVertex3f(-0.16, 0.16, -0.04);
		glVertex3f(-0.16, 0.26, -0.04);
		glVertex3f(-0.16, 0.26, -0.02);
		glVertex3f(-0.16, 0.16, -0.02);
			//back
		glVertex3f(-0.14, 0.16, -0.04);
		glVertex3f(-0.14, 0.26, -0.04);
		glVertex3f(-0.14, 0.26, -0.02);
		glVertex3f(-0.14, 0.16, -0.02);

	//closest barrel
			//side view - closest
		glVertex3f(-0.16, 0.16, 0.04);
		glVertex3f(-0.16, 0.26, 0.04);
		glVertex3f(-0.14, 0.26, 0.04);
		glVertex3f(-0.14, 0.16, 0.04);
			//side view - furthest
		glVertex3f(-0.16, 0.16, 0.02);
		glVertex3f(-0.16, 0.26, 0.02);
		glVertex3f(-0.14, 0.26, 0.02);
		glVertex3f(-0.14, 0.16, 0.02);
			//bottom
		glVertex3f(-0.16, 0.16, 0.04);
		glVertex3f(-0.16, 0.16, 0.02);
		glVertex3f(-0.14, 0.16, 0.02);
		glVertex3f(-0.14, 0.16, 0.04);
			//top
		glVertex3f(-0.16, 0.26, 0.04);
		glVertex3f(-0.16, 0.26, 0.02);
		glVertex3f(-0.14, 0.26, 0.02);
		glVertex3f(-0.14, 0.26, 0.04);
			//front
		glVertex3f(-0.16, 0.16, 0.04);
		glVertex3f(-0.16, 0.26, 0.04);
		glVertex3f(-0.16, 0.26, 0.02);
		glVertex3f(-0.16, 0.16, 0.02);
			//back
		glVertex3f(-0.14, 0.16, 0.04);
		glVertex3f(-0.14, 0.26, 0.04);
		glVertex3f(-0.14, 0.26, 0.02);
		glVertex3f(-0.14, 0.16, 0.02);
	glEnd();
}//drawBarrelTop

void Visuals::drawGunBottom()
{
	glColor3f(1.0, 1.0, 0.0);
	glBegin(GL_QUADS);
			//side view - closest
		glVertex3f(-0.18, -0.07, -0.05);
		glVertex3f(-0.18, -0.12, -0.05);
		glVertex3f(-0.08, -0.12, -0.05);
		glVertex3f(-0.08, -0.07, -0.05);
			//side view - furthest
		glVertex3f(-0.18, -0.07, 0.05);
		glVertex3f(-0.18, -0.12, 0.05);
		glVertex3f(-0.08, -0.12, 0.05);
		glVertex3f(-0.08, -0.07, 0.05);
			//top
		glVertex3f(-0.18, -0.12, -0.05);
		glVertex3f(-0.18, -0.12, 0.05);
		glVertex3f(-0.08, -0.12, 0.05);
		glVertex3f(-0.08, -0.12, -0.05);
			//front
		glVertex3f(-0.18, -0.07, -0.05);
		glVertex3f(-0.18, -0.12, -0.05);
		glVertex3f(-0.18, -0.12, 0.05);
		glVertex3f(-0.18, -0.07, 0.05);
			//back
		glVertex3f(-0.08, -0.07, -0.05);
		glVertex3f(-0.08, -0.12, -0.05);
		glVertex3f(-0.08, -0.12, 0.05);
		glVertex3f(-0.08, -0.07, 0.05);
	glEnd();

	glColor3f(0.0, 0.0, 0.0);
	glBegin(GL_TRIANGLES);
			//side view - closest
		glVertex3f(-0.18, -0.12, -0.04001);
		glVertex3f(-0.08, -0.12, -0.04001);
		glVertex3f(-0.13, -0.15, -0.04001);
		glVertex3f(-0.18, -0.12, -0.048);
		glVertex3f(-0.08, -0.12, -0.048);
		glVertex3f(-0.13, -0.15, -0.048);
			//side view - mid closest
		glVertex3f(-0.18, -0.12, -0.02001);
		glVertex3f(-0.08, -0.12, -0.02001);
		glVertex3f(-0.13, -0.15, -0.02001);
		glVertex3f(-0.18, -0.12, -0.012);
		glVertex3f(-0.08, -0.12, -0.012);
		glVertex3f(-0.13, -0.15, -0.012);
			//side view - furthest
		glVertex3f(-0.18, -0.12, 0.04001);
		glVertex3f(-0.08, -0.12, 0.04001);
		glVertex3f(-0.13, -0.15, 0.04001);
		glVertex3f(-0.18, -0.12, 0.048);
		glVertex3f(-0.08, -0.12, 0.048);
		glVertex3f(-0.13, -0.15, 0.048);
			//side view - mid furthest
		glVertex3f(-0.18, -0.12, 0.02001);
		glVertex3f(-0.08, -0.12, 0.02001);
		glVertex3f(-0.13, -0.15, 0.02001);
		glVertex3f(-0.18, -0.12, 0.012);
		glVertex3f(-0.08, -0.12, 0.012);
		glVertex3f(-0.13, -0.15, 0.012);
	glEnd();

	glBegin(GL_QUADS);
			//closest
		glVertex3f(-0.18, -0.12, -0.048);
		glVertex3f(-0.18, -0.12, -0.04001);
		glVertex3f(-0.13, -0.15, -0.04001);
		glVertex3f(-0.13, -0.15, -0.048);
		glVertex3f(-0.08, -0.12, -0.048);
		glVertex3f(-0.08, -0.12, -0.04001);
		glVertex3f(-0.13, -0.15, -0.04001);
		glVertex3f(-0.13, -0.15, -0.048);
			//mid closest
		glVertex3f(-0.18, -0.12, -0.02001);
		glVertex3f(-0.18, -0.12, -0.012);
		glVertex3f(-0.13, -0.15, -0.012);
		glVertex3f(-0.13, -0.15, -0.02001);
		glVertex3f(-0.08, -0.12, -0.02001);
		glVertex3f(-0.08, -0.12, -0.012);
		glVertex3f(-0.13, -0.15, -0.012);
		glVertex3f(-0.13, -0.15, -0.02001);
			//furthest
		glVertex3f(-0.18, -0.12, 0.012);
		glVertex3f(-0.18, -0.12, 0.02001);
		glVertex3f(-0.13, -0.15, 0.02001);
		glVertex3f(-0.13, -0.15, 0.012);
		glVertex3f(-0.08, -0.12, 0.012);
		glVertex3f(-0.08, -0.12, 0.02001);
		glVertex3f(-0.13, -0.15, 0.02001);
		glVertex3f(-0.13, -0.15, 0.012);
			//mid furthest
		glVertex3f(-0.18, -0.12, 0.04001);
		glVertex3f(-0.18, -0.12, 0.048);
		glVertex3f(-0.13, -0.15, 0.048);
		glVertex3f(-0.13, -0.15, 0.04001);
		glVertex3f(-0.08, -0.12, 0.04001);
		glVertex3f(-0.08, -0.12, 0.048);
		glVertex3f(-0.13, -0.15, 0.048);
		glVertex3f(-0.13, -0.15, 0.04001);
	glEnd();
}//drawGunBottom

void Visuals::drawBarrelBottom()
{
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_QUADS);
	//furthest barrel
			//side view - closest
		glVertex3f(-0.14, -0.13, -0.04);
		glVertex3f(-0.14, -0.23, -0.04);
		glVertex3f(-0.12, -0.23, -0.04);
		glVertex3f(-0.12, -0.13, -0.04);
			//side view - furthest
		glVertex3f(-0.14, -0.13, -0.02);
		glVertex3f(-0.14, -0.23, -0.02);
		glVertex3f(-0.12, -0.23, -0.02);
		glVertex3f(-0.12, -0.13, -0.02);
			//bottom
		glVertex3f(-0.14, -0.13, -0.04);
		glVertex3f(-0.14, -0.13, -0.02);
		glVertex3f(-0.12, -0.13, -0.02);
		glVertex3f(-0.12, -0.13, -0.04);
			//top
		glVertex3f(-0.14, -0.23, -0.04);
		glVertex3f(-0.14, -0.23, -0.02);
		glVertex3f(-0.12, -0.23, -0.02);
		glVertex3f(-0.12, -0.23, -0.04);
			//front
		glVertex3f(-0.14, -0.13, -0.04);
		glVertex3f(-0.14, -0.23, -0.04);
		glVertex3f(-0.14, -0.23, -0.02);
		glVertex3f(-0.14, -0.13, -0.02);
			//back
		glVertex3f(-0.12, -0.13, -0.04);
		glVertex3f(-0.12, -0.23, -0.04);
		glVertex3f(-0.12, -0.23, -0.02);
		glVertex3f(-0.12, -0.13, -0.02);

	//closest barrel
			//side view - closest
		glVertex3f(-0.14, -0.13, 0.04);
		glVertex3f(-0.14, -0.23, 0.04);
		glVertex3f(-0.12, -0.23, 0.04);
		glVertex3f(-0.12, -0.13, 0.04);
			//side view - furthest
		glVertex3f(-0.14, -0.13, 0.02);
		glVertex3f(-0.14, -0.23, 0.02);
		glVertex3f(-0.12, -0.23, 0.02);
		glVertex3f(-0.12, -0.13, 0.02);
			//bottom
		glVertex3f(-0.14, -0.13, 0.04);
		glVertex3f(-0.14, -0.13, 0.02);
		glVertex3f(-0.12, -0.13, 0.02);
		glVertex3f(-0.12, -0.13, 0.04);
			//top
		glVertex3f(-0.14, -0.23, 0.04);
		glVertex3f(-0.14, -0.23, 0.02);
		glVertex3f(-0.12, -0.23, 0.02);
		glVertex3f(-0.12, -0.23, 0.04);
			//front
		glVertex3f(-0.14, -0.13, 0.04);
		glVertex3f(-0.14, -0.23, 0.04);
		glVertex3f(-0.14, -0.23, 0.02);
		glVertex3f(-0.14, -0.13, 0.02);
			//back
		glVertex3f(-0.12, -0.13, 0.04);
		glVertex3f(-0.12, -0.23, 0.04);
		glVertex3f(-0.12, -0.23, 0.02);
		glVertex3f(-0.12, -0.13, 0.02);
	glEnd();
}//drawBarrelBottom

///////////////drawCamera and drawCameraLocalAxes///////////////
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

void Visuals::drawCameraLocalAxes()
{
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0.0, 0.0);
		glVertex3f(X_MAX/3, 0.0, 0.0);

		//y-axis - green
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0, 0.0);
		glVertex3f(0.0, Y_MAX/3, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(0.0, 0.0, Z_MAX/3);
	glEnd();
}//end drawCameraLocalAxes

/////////////////drawWorldAxes - both viewports/////////////////
void Visuals::drawLeftWorldAxes()
{
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0.0, 0.0);
		glVertex3f(X_MAX, 0.0, 0.0);

		//y-axis - green
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0, 0.0);
		glVertex3f(0.0, Y_MAX, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0);
		glVertex3f(0.0, 0.0, Z_MAX);
	glEnd();
}//end drawLeftWorldAxes

void Visuals::drawRightWorldAxes()
{
	glBegin(GL_LINES);
		//x-axis - red
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0, 0.0, 0.0);
		glVertex3f(X_MAX, 0.0, 0.0);

		//z-axis - blue
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0);
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