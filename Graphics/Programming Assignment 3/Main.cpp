//////////////////////////////////////////////////////////////////////
//Nick Handelman													//
//CMPS 515															//
//																	//
//Programming Assignment 3											//
//																	//

//////////////////////////////////////////////////////////////////////


////////////////Libraries, Constants, Global Variables////////////////
#include "Visuals.h"
#include <stdlib.h>
#include <GL/glut.h>
#include <gmtl/gmtl.h>

struct Position_Quaternion 
{
	gmtl::Vec3f p;
	gmtl::Quatf q;
};

Visuals visuals = Visuals();
Position_Quaternion x_trans_pos, x_trans_neg;
Position_Quaternion distance_pos, distance_neg;
Position_Quaternion yaw_cw, yaw_ccw, pitch_up, pitch_down, roll_cw, roll_ccw;
Position_Quaternion plane_pq_1, plane_pq_2;
Position_Quaternion gun_pq_top, gun_pq_bottom, barrel_pq_top, barrel_pq_bottom,
	satellite_pq;
Position_Quaternion camera_pq_world, camera_pq_1, camera_pq_2;

bool show_axes, which_plane;
int which_view, which_subpart;

//////////////////////Function - init/////////////////////////
//Initialize one-time openGL functions						//
//Initialize positive and negative values for transforms:	//
//   translation - positive for plane and camera			//
//	 yaw - positive and negative for plane and camera		//
//	 pitch - positive and negative for plane and camera		//
//	 roll - positive and negative for plane and camera		//
//   azimuth - same as roll									//
//   elevation - same as									//
//////////////////////////////////////////////////////////////
void init(void)
{
	glClearColor(0.7, 0.7, 0.7, 0.0); 
	glEnable(GL_DEPTH_TEST);

	gmtl::Matrix44f temp;

	//set x-translation
	x_trans_pos.p.set(-X_TRANS,0,0); x_trans_pos.q.set(0,0,0,1);
	x_trans_neg.p.set(X_TRANS,0,0); x_trans_neg.q.set(0,0,0,1);
	distance_pos.p.set(0,0,-X_TRANS); x_trans_pos.q.set(0,0,0,1);
	distance_neg.p.set(0,0,X_TRANS); x_trans_neg.q.set(0,0,0,1);

	//convert from matrix to quaternion for yaw
	temp = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,gmtl::Math::deg2Rad(-YAW_DEG),0.f));
	yaw_cw.p.set(0,0,0); gmtl::set(yaw_cw.q, temp);
	temp = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,gmtl::Math::deg2Rad(YAW_DEG),0.f));
	yaw_ccw.p.set(0,0,0); gmtl::set(yaw_ccw.q, temp);

	//convert from matrix to quaternion for pitch
	temp = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,0.f,gmtl::Math::deg2Rad(-PITCH_DEG)));
	pitch_up.p.set(0,0,0); gmtl::set(pitch_up.q, temp);
	temp = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,0.f,gmtl::Math::deg2Rad(PITCH_DEG)));
	pitch_down.p.set(0,0,0); gmtl::set(pitch_down.q, temp);

	//convert from matrix to quaternion for roll
	temp = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(gmtl::Math::deg2Rad(-ROLL_DEG),0.f,0.f));
	roll_cw.p.set(0,0,0); gmtl::set(roll_cw.q, temp);
	temp = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(gmtl::Math::deg2Rad(ROLL_DEG),0.f,0.f));
	roll_ccw.p.set(0,0,0); gmtl::set(roll_ccw.q, temp);
}//end init

/////////////////Function - reset/////////////////////////////
//Initialize global variables that maintain various options //
//Initialize Position_Quaternions with respect to parent    //	
//////////////////////////////////////////////////////////////
void reset(void)
{
	gmtl::Matrix44f temp = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
		(gmtl::Math::deg2Rad(180.0),0.f,0.f));
	
	//initialize global variables
	show_axes = false;
	which_view = 0; // 0 = world, 1 = camera 1, 2 = camera 2
	which_plane = true; //true = plane 1, false = plane 2
	which_subpart = 0; //0 = satellite, 1 = camera 1, 2 = camera 2

	//initialize position_quaternions
	plane_pq_1.p.set(0,1,-2); plane_pq_1.q.set(0,0,0,1); 
	plane_pq_2.p.set(0,0,-2); plane_pq_2.q.set(0,0,0,1); 
	gun_pq_top.p.set(-0.13,0.125,0); gun_pq_top.q.set(0,0,0,1); 
	barrel_pq_top.p.set(0,0.045,0); barrel_pq_top.q.set(0,0,0,1);
	gun_pq_bottom.p.set(-0.12,-0.095,0); gmtl::set(gun_pq_bottom.q, temp); 
	barrel_pq_bottom.p.set(0.0,0.045,0); barrel_pq_bottom.q.set(0,0,0,1);
	satellite_pq.p.set(0.179,0.22,0); satellite_pq.q.set(0,0,0,1);  
	camera_pq_world.p.set(0,0,0); camera_pq_world.q.set(0,0,0,1); 
	camera_pq_1.p.set(0,0,1); camera_pq_1.q.set(0,0,0,1);
	camera_pq_2.p.set(0,0,1); camera_pq_2.q.set(0,0,0,1);
}//end reset

////////////////////////////////Function - display////////////////////////////////////
//Clear color and depth buffers														//
//Set right viewport and draw plane and camera, local axes for both object and		//
//	for both object and	world axes from top-down view								//
//Set left viewport and draw plane, local axes and world axes from the camera view  //	
//////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gmtl::Matrix44f trans_matrix, rot_matrix, inverse_matrix;
				gmtl::Matrix44f temp = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			(0.f,gmtl::Math::deg2Rad(180.f),0.f));
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 10);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef( 0,0,0 );
	visuals.drawLight();
	glLoadIdentity();
	switch(which_view)
	{
		case 0:	
			//load inverse camera matrix as modelview matrix
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_world.p);
			gmtl::set(rot_matrix, camera_pq_world.q);
			inverse_matrix = trans_matrix * rot_matrix;
			gmtl::invert(inverse_matrix);
			glLoadMatrixf(inverse_matrix.getData());
			visuals.drawLeftWorldAxes();
			glPushMatrix();

			//draw plane 1 and local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(plane_pq_1.p);
			gmtl::set(rot_matrix, plane_pq_1.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawPlane();
			if(show_axes) visuals.drawPlaneLocalAxes();
			
			//draw camera 1 local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_1.p);
			gmtl::set(rot_matrix, camera_pq_1.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawCameraLocalAxes();
			glPopMatrix();

			//draw plane 2 and local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(plane_pq_2.p);
			gmtl::set(rot_matrix, plane_pq_2.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawPlane();
			if(show_axes) visuals.drawPlaneLocalAxes();
			glPushMatrix();

			//draw camera 2 local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_2.p);
			gmtl::set(rot_matrix, camera_pq_2.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawCameraLocalAxes();
			glPopMatrix();
			glPushMatrix();

			//draw top gun and barrel
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(gun_pq_top.p);
			gmtl::set(rot_matrix, gun_pq_top.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawGun();
			if(show_axes) visuals.drawSmallAxes();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(barrel_pq_top.p);
			gmtl::set(rot_matrix, barrel_pq_top.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawBarrel();
			if(show_axes) visuals.drawSmallAxes();
			glPopMatrix();
			glPushMatrix();

			//draw bottom gun and barrel
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(gun_pq_bottom.p);
			gmtl::set(rot_matrix, gun_pq_bottom.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawGun();
			if(show_axes) visuals.drawSmallAxes();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(barrel_pq_bottom.p);
			gmtl::set(rot_matrix, barrel_pq_bottom.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawBarrel();
			if(show_axes) visuals.drawSmallAxes();
			glPopMatrix();

			//draw satellite
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(satellite_pq.p);
			gmtl::set(rot_matrix, satellite_pq.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawSatellite();
			if(show_axes) visuals.drawSmallAxes();
		break;

		case 1:
			//invert camera 1 and draw plane 1
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_1.p);
			gmtl::set(rot_matrix, camera_pq_1.q);
			inverse_matrix = trans_matrix * rot_matrix;
			gmtl::invert(inverse_matrix);
			glLoadMatrixf(inverse_matrix.getData());
			visuals.drawPlane();
			if(show_axes) visuals.drawPlaneLocalAxes();

			//multiply by plane 1 inverse
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(plane_pq_1.p);
			gmtl::set(rot_matrix, plane_pq_1.q);
			inverse_matrix = trans_matrix * rot_matrix;
			gmtl::invert(inverse_matrix);
			glMultMatrixf(inverse_matrix.getData());
			glPushMatrix();

			//draw world camera local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_world.p);
			gmtl::set(rot_matrix, camera_pq_world.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawCameraLocalAxes();
			glPopMatrix();

			//draw plane 2 and local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(plane_pq_2.p);
			gmtl::set(rot_matrix, plane_pq_2.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawPlane();
			if(show_axes) visuals.drawPlaneLocalAxes();
			glPushMatrix();

			//draw camera 2
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_2.p);
			gmtl::set(rot_matrix, camera_pq_2.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawCameraLocalAxes();
			glPopMatrix();
			glPushMatrix();

			//draw top gun and barrel
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(gun_pq_top.p);
			gmtl::set(rot_matrix, gun_pq_top.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawGun();
			if(show_axes) visuals.drawSmallAxes();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(barrel_pq_top.p);
			gmtl::set(rot_matrix, barrel_pq_top.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawBarrel();
			if(show_axes) visuals.drawSmallAxes();
			glPopMatrix();
			glPushMatrix();

			//draw bottom gun and barrel
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(gun_pq_bottom.p);
			gmtl::set(rot_matrix, gun_pq_bottom.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawGun();
			if(show_axes) visuals.drawSmallAxes();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(barrel_pq_bottom.p);
			gmtl::set(rot_matrix, barrel_pq_bottom.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawBarrel();
			if(show_axes) visuals.drawSmallAxes();
			glPopMatrix();

			//draw satellite
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(satellite_pq.p);
			gmtl::set(rot_matrix, satellite_pq.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawSatellite();
			if(show_axes) visuals.drawSmallAxes();
		break;
		case 2:
			//invert camera 2 and draw plane 2
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_2.p);
			gmtl::set(rot_matrix, camera_pq_2.q);
			inverse_matrix = trans_matrix * rot_matrix;
			gmtl::invert(inverse_matrix);
			glLoadMatrixf(inverse_matrix.getData());
			visuals.drawPlane();
			if(show_axes) visuals.drawPlaneLocalAxes();
			glPushMatrix();

			//draw top gun and barrel
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(gun_pq_top.p);
			gmtl::set(rot_matrix, gun_pq_top.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawGun();
			if(show_axes) visuals.drawSmallAxes();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(barrel_pq_top.p);
			gmtl::set(rot_matrix, barrel_pq_top.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawBarrel();
			if(show_axes) visuals.drawSmallAxes();
			glPopMatrix();
			glPushMatrix();

			//draw bottom gun and barrel
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(gun_pq_bottom.p);
			gmtl::set(rot_matrix, gun_pq_bottom.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawGun();
			if(show_axes) visuals.drawSmallAxes();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(barrel_pq_bottom.p);
			gmtl::set(rot_matrix, barrel_pq_bottom.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawBarrel();
			if(show_axes) visuals.drawSmallAxes();
			glPopMatrix();
			glPushMatrix();

			//draw satellite
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(satellite_pq.p);
			gmtl::set(rot_matrix, satellite_pq.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawSatellite();
			if(show_axes) visuals.drawSmallAxes();
			glPopMatrix();

			//multiply by plane 2 inverse, now at world
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(plane_pq_2.p);
			gmtl::set(rot_matrix, plane_pq_2.q);
			inverse_matrix = trans_matrix * rot_matrix;
			gmtl::invert(inverse_matrix);
			glMultMatrixf(inverse_matrix.getData());
			glPushMatrix();

			//draw world camera local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_world.p);
			gmtl::set(rot_matrix, camera_pq_world.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawCameraLocalAxes();
			glPopMatrix();

			//draw plane 1 and local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(plane_pq_1.p);
			gmtl::set(rot_matrix, plane_pq_1.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawPlane();
			if(show_axes) visuals.drawPlaneLocalAxes();
			
			//draw camera 1 local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_1.p);
			gmtl::set(rot_matrix, camera_pq_1.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			visuals.drawCameraLocalAxes();
			glPopMatrix();
			break;
	}//switch
	glutSwapBuffers();
}//end display


///////////////////////Function - CalculatePQ/////////////////////////
//Receives a position_quaternion with transformation information	//
//Receives an int designating which object to transform				//
//   0: camera   1: plane   2: subpart level 1   3: subpart level 2	//
//Makes transformation to target object								//
//////////////////////////////////////////////////////////////////////
void calculatePQ(Position_Quaternion temp_pq, int which_obj)
{
  gmtl::Quatf plane_temp;
  switch(which_obj)
  {	//camera transform
    case 0: 
	  switch(which_view)
	  {	//world camera
	    case 0: plane_temp = camera_pq_world.q*gmtl::Quatf(temp_pq.p[0], temp_pq.p[1], temp_pq.p[2], 0)
					*gmtl::makeConj(camera_pq_world.q);
				camera_pq_world.p[0] += plane_temp[0];
				camera_pq_world.p[1] += plane_temp[1];
				camera_pq_world.p[2] += plane_temp[2];
				plane_temp = temp_pq.q * gmtl::Quatf(camera_pq_world.p[0], camera_pq_world.p[1], camera_pq_world.p[2], 0)
					* gmtl::makeConj(temp_pq.q);
				camera_pq_world.p[0] = plane_temp[0]; 
				camera_pq_world.p[1] = plane_temp[1]; 
				camera_pq_world.p[2] = plane_temp[2]; 
				camera_pq_world.q = temp_pq.q * camera_pq_world.q;
		break;
		//camera 1 
	    case 1: plane_temp = camera_pq_1.q*gmtl::Quatf(temp_pq.p[0], temp_pq.p[1], temp_pq.p[2], 0)
					*gmtl::makeConj(camera_pq_1.q);
				camera_pq_1.p[0] += plane_temp[0];
				camera_pq_1.p[1] += plane_temp[1];
				camera_pq_1.p[2] += plane_temp[2];
				plane_temp = temp_pq.q*gmtl::Quatf(camera_pq_1.p[0], camera_pq_1.p[1], camera_pq_1.p[2], 0) 
					*gmtl::makeConj(temp_pq.q);
				camera_pq_1.p[0] = plane_temp[0]; 
				camera_pq_1.p[1] = plane_temp[1]; 
				camera_pq_1.p[2] = plane_temp[2];
				camera_pq_1.q = temp_pq.q * camera_pq_1.q;
		  break;
		//camera 2
	    case 2: plane_temp = camera_pq_2.q*gmtl::Quatf(temp_pq.p[0], temp_pq.p[1], temp_pq.p[2], 0)
					*gmtl::makeConj(camera_pq_2.q);
				camera_pq_2.p[0] += plane_temp[0];
				camera_pq_2.p[1] += plane_temp[1];
				camera_pq_2.p[2] += plane_temp[2];
				camera_pq_2.q = temp_pq.q * camera_pq_2.q;
				plane_temp = temp_pq.q*gmtl::Quatf(camera_pq_2.p[0], camera_pq_2.p[1], camera_pq_2.p[2], 0) 
					*gmtl::makeConj(temp_pq.q);
				camera_pq_2.p[0] = plane_temp[0]; 
				camera_pq_2.p[1] = plane_temp[1]; 
				camera_pq_2.p[2] = plane_temp[2];
				camera_pq_2.q = temp_pq.q * camera_pq_2.q;
		  break;
	  }//switch
	break;
	//plane transform
	case 1: 
	  if(which_plane) //plane 1
	  {  
		plane_temp = plane_pq_1.q*gmtl::Quatf(temp_pq.p[0], temp_pq.p[1], temp_pq.p[2], 0)
			*gmtl::makeConj(plane_pq_1.q);
		plane_pq_1.p[0] += plane_temp[0];
		plane_pq_1.p[1] += plane_temp[1];
		plane_pq_1.p[2] += plane_temp[2];
		plane_pq_1.q *= temp_pq.q;
	  }//if
	  else //plane 2
	  {
		plane_temp = plane_pq_2.q*gmtl::Quatf(temp_pq.p[0], temp_pq.p[1], temp_pq.p[2], 0)
			*gmtl::makeConj(plane_pq_2.q);
		plane_pq_2.p[0] += plane_temp[0];
		plane_pq_2.p[1] += plane_temp[1];
		plane_pq_2.p[2] += plane_temp[2];
		plane_pq_2.q *= temp_pq.q;
	  }//else
	break;
	//subpart level 1 transform
	case 2: 
	  switch(which_subpart)
	  {	
	    case 0: satellite_pq.q *= temp_pq.q; 	
		break;
		case 1: gun_pq_top.q *= temp_pq.q ; 
		break;
		case 2: gun_pq_bottom.q *= temp_pq.q;
	    break;
	  }//switch
	  break;
	  //subpart level 2 transform
	  case 3: 
	    switch(which_subpart)
	    { 
		  case 1: barrel_pq_top.q *= temp_pq.q; 
		  break;
		  case 2: barrel_pq_bottom.q *= temp_pq.q;
		  break;
		}//switch
		break;
	}//switch
}//calculatePQ

//////////////Function - keyboardhandler//////////////
//					Plane:     Subpart:     Camera: //
// Translation(+):	  w								//
// Yaw(+/-):		 a/d         f/h			    //
// Pitch(+/-):		 q/e         t/g				//
// Roll(+/-):		 z/c							//
// Distance(+/-):							  u/o	//
// Azimuth(+/-):							  j/l	//
// Elevation(+/-):							  i/k	//
//													//
// Extra Commands:									//
//	1 - switch among the three camera views			//
//	2 - switch between the two planes				//
//  3 - switch among three subparts on plane 2		//
//	4 - turn coordinate axes on or off				//
//  Spacebar - return to initial settings			//
//////////////////////////////////////////////////////
void keyboardhandler(unsigned char key, int x, int y)
{
	switch(key)
	{
	//camera distance (+/-)
		case 'u':calculatePQ(distance_pos, 0);
			break;
		case 'o':calculatePQ(distance_neg, 0);
			break;
	//camera elevation (+/-)
		case 'i':calculatePQ(roll_ccw, 0);
			break;
		case 'k':calculatePQ(roll_cw, 0);
			break;
	//camera azimuth (+/-)
		case 'j':calculatePQ(yaw_ccw, 0);
			break;
		case 'l':calculatePQ(yaw_cw, 0);
			break;
	//plane translation (+)
		case 'w': calculatePQ(x_trans_pos, 1);
			break;
	//plane yaw (+/-)
		case 'a': calculatePQ(yaw_ccw, 1);
			break;
		case 'd': calculatePQ(yaw_cw, 1);
			break;
	//plane pitch (+/-)
		case 'q': calculatePQ(pitch_down, 1);
			break;
		case 'e': calculatePQ(pitch_up, 1);
			break;
	//plane roll (+/-)
		case 'z': calculatePQ(roll_ccw, 1);
			break;
		case 'c': calculatePQ(roll_cw, 1);
			break;
	//subpart yaw (+/-)
		case 'f': if(!which_plane) calculatePQ(yaw_ccw, 2);
			break;
		case 'h': if(!which_plane) calculatePQ(yaw_cw, 2);
			break;
	//subpart pitch (+/-)
		case 't': if(!which_plane) calculatePQ(pitch_down, 3);
			break;
		case 'g': if(!which_plane) calculatePQ(pitch_up, 3);
			break;
	//switch view
		case '1': which_view = (which_view + 1) % 3;
			break;
	//switch plane
		case '2': which_plane = !which_plane;
			break;
	//switch plane subpart
		case '3': which_subpart = (which_subpart + 1) % 3;
			break;
	//show coordinate axes
		case '4': show_axes = !show_axes;
			break;
	//spacebar - return to initial position and settings
		case 32: reset();
			break;
	//dont redisplay if inappropriate key pressed
		default: return;
	}//switch

	glutPostRedisplay();
}//end keyboardhandler

//////////////////Function - main/////////////////////
// Initialize and create window						//
// Initialize display function						//
// Initialize keyboard function						//
// Initialize init - one time object declarations	//
// Initialize reset - multi time object declarations//
// Start glutMainLoop()								//			
//////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutCreateWindow("Assignment 3");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardhandler);
	init();
	reset();
	glutMainLoop();
	return 0;
}//end main