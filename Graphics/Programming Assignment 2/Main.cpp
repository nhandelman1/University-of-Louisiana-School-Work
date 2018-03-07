//////////////////////////////////////////////////////////////////////
//Nick Handelman													//
//CMPS 515															//
//																	//
//Programming Assignment 2											//
//																	//
//This program renders a plane in two viewports. The right viewport	//
//displays a fixed top-down view of the world axes, a plane and its	//
//local axes and a camera and its local axes.						//
//The left viewport displays world axes and a plane and its local	//
//axes.																//
//////////////////////////////////////////////////////////////////////


////////////////Libraries, Constants, Global Variables////////////////
#include "Visuals.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <gmtl/gmtl.h>

Visuals visuals = Visuals();
gmtl::Matrix44f x_trans_pos, yaw_cw, yaw_ccw, pitch_up, 
	pitch_down, roll_cw, roll_ccw;
gmtl::Matrix44f plane_matrix;
gmtl::Matrix44f camera_matrix, camera_inverse_matrix;
gmtl::Matrix44f fixed_matrix, inverse_fixed_matrix;

//////////////////////Function - init/////////////////////////
//Initialize one-time openGL functions						//
//Initialize all matrix states to the appropriate level		//
//Initialize positive and negative values for transforms:	//
//		translation - positive for plane and camera			//
//		yaw - positive and negative for plane and camera	//
//		pitch - positive and negative for plane and camera	//
//		roll - positive and negative for plane and camera	//
//Initialize a plane matrix									//
//Initialize a camera matrix and a camera inverse matrix	//
//Initialize rotation matrix and rotation_inverse_matrix    //
//		for use in top-down viewport						//			
//////////////////////////////////////////////////////////////
void init(void)
{
	glClearColor(0.7, 0.7, 0.7, 0.0); 
	glEnable(GL_DEPTH_TEST);

	//set matrix state to TRANS (2) for translation only, set 
	//forward x-translate to 0.2
	x_trans_pos.setState(2);
	x_trans_pos = gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f
		(-X_TRANS,0.f,0.f));

	//set matrix state to ORTHOGONAL (4) for rotation only, yaw - 
	//rotation about Y-axis set to 15 degrees
	yaw_cw = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,gmtl::Math::deg2Rad(-YAW_DEG),0.f));
	yaw_ccw = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,gmtl::Math::deg2Rad(YAW_DEG),0.f));
	yaw_cw.setState(4);
	yaw_ccw.setState(4);

	//set matrix state to ORTHOGONAL (4) for rotation only, pitch - 
	//rotation about Z-axis set to 15 degrees
	pitch_up = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,0.f,gmtl::Math::deg2Rad(-PITCH_DEG)));
	pitch_down = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,0.f,gmtl::Math::deg2Rad(PITCH_DEG)));
	pitch_up.setState(4);
	pitch_down.setState(4);

	//set matrix state to ORTHOGONAL (4) for rotation only, roll - 
	//rotation about X-axis set 15 degrees
	roll_cw = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(gmtl::Math::deg2Rad(-ROLL_DEG),0.f,0.f));
	roll_ccw = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(gmtl::Math::deg2Rad(ROLL_DEG),0.f,0.f));
	roll_cw.setState(4);
	roll_ccw.setState(4);

	//set matrix state to AFFINE (16) since it will operate on 
	//translations and rotations
	gmtl::identity(plane_matrix);
	gmtl::identity(camera_matrix);
	gmtl::identity(camera_inverse_matrix);
	plane_matrix.setState(16);
	camera_matrix.setState(16);
	camera_inverse_matrix.setState(16);

	//set matrix state to ORTHOGONAL (4) for rotation only
	//these matrices adjust the fixed top-down viewport for the correct 
	//orientation in reference the camera viewport
	inverse_fixed_matrix = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(gmtl::Math::deg2Rad(90.f),0.f,0.f));
	fixed_matrix = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(gmtl::Math::deg2Rad(-90.f),0.f,0.f));
	inverse_fixed_matrix.setState(4);
	fixed_matrix.setState(4);
}//end init

////////////////////////////////Function - display////////////////////////////////////
//Clear color and depth buffers														//
//Set right viewport and draw plane and camera, local axes for both object and		//
//	for both object and	world axes from top-down view								//
//Set left viewport and draw plane, local axes and world axes from the camera view  //	
//////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//draw right viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(X_MIN, X_MAX, Y_MIN, Y_MAX, Z_MIN, Z_MAX);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(W_WIDTH/2, 0, W_WIDTH/2, W_HEIGHT);
		
		//draw world axes
	glLoadMatrixf(inverse_fixed_matrix.getData());
	visuals.drawRightWorldAxes();
		//draw camera and local axes
	glMultMatrixf(camera_matrix.getData());
	visuals.drawCamera();
	visuals.drawCameraLocalAxes();
		//draw plane and local axes
	glLoadMatrixf((inverse_fixed_matrix * plane_matrix).getData());
	visuals.drawPlane();
	visuals.drawPlaneLocalAxes();

	//draw left viewport
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(X_MIN, X_MAX, Y_MIN, Y_MAX, 0, Z_MAX);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glViewport(0, 0, W_WIDTH/2, W_HEIGHT);
	
		//draw world axes
	glLoadMatrixf(camera_inverse_matrix.getData());
	visuals.drawLeftWorldAxes();
		//draw plane and local axes
	camera_inverse_matrix *= plane_matrix;
	glLoadMatrixf(camera_inverse_matrix.getData());
	visuals.drawPlane();
	visuals.drawPlaneLocalAxes();

	glutSwapBuffers();
}//end display

///////////////////////Function - keyboardhandler/////////////////////////
//						Plane motion keys:			Camera motion keys:	//
// Translation(+):				 w							 i	 		//
// Yaw(+/-):					a/d					 		j/l			//
// Pitch(+/-):					q/e							u/o			//
// Roll(+/-):					z/c							m/.			//
//																		//
// Extra Commands:														//
//	Spacebar - return plane to initial position							//
//////////////////////////////////////////////////////////////////////////
void keyboardhandler(unsigned char key, int x, int y)
{
	switch(key)
	{
		//plane translation (+)
		case 'w': plane_matrix *= x_trans_pos;
			break;
		//plane yaw (+/-)
		case 'a': plane_matrix *= yaw_ccw;
			break;
		case 'd': plane_matrix *= yaw_cw;
			break;
		//plane pitch (+/-)
		case 'q': plane_matrix *= pitch_down;
			break;
		case 'e': plane_matrix *= pitch_up;
			break;
		//plane roll (+/-)
		case 'z': plane_matrix *= roll_ccw;
			break;
		case 'c': plane_matrix *= roll_cw;
			break;
		//camera translation (+)
		case 'i':camera_matrix *= x_trans_pos;
			break;
		//camera yaw (+/-)
		case 'j': camera_matrix *= yaw_ccw;
			break;
		case 'l': camera_matrix *= yaw_cw;
			break;
		//camera pitch (+/-)
		case 'u': camera_matrix *= pitch_down;
			break;
		case 'o': camera_matrix *= pitch_up;
			break;
		//camera roll (+/-)
		case 'm': camera_matrix *= roll_ccw;
			break;
		case '.': camera_matrix *= roll_cw;
			break;
		//spacebar - return to initial position
		case 32: gmtl::identity(plane_matrix);
				 gmtl::identity(camera_matrix);
			break;
		//dont redisplay if relevant button not pressed
		default: return; 
	}//switch

	gmtl::invert(camera_inverse_matrix, camera_matrix);
	glutPostRedisplay();
}//end keyboardhandler

//////////////////Function - main/////////////////////
// Initialize and create window						//
// Initialize display function						//
// Initialize keyboard function						//
// Initialize init - one time object declarations	//
// Start glutMainLoop()								//			
//////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutCreateWindow("Assignment 2");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardhandler);
	init();
	glutMainLoop();
	return 0;
}//end main