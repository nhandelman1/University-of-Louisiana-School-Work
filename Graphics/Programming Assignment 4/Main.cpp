//////////////////////////////////////////////////////////////////////
//Nick Handelman													//
//CMPS 515															//
//																	//
//Programming Assignment 4											//
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

gmtl::Vec4f light_pos;
static GLuint ship_tex_1, ship_tex_2, borg_tex_1;
static GLuint space_tex_1, space_tex_2, space_tex_3, space_tex_4, space_tex_5, space_tex_6;
bool show_axes, which_plane, show_ambient, show_diffuse, show_specular;
int which_view, which_subpart;
const GLfloat color_ambient[]  = { 1.0, 1.0, 0.0, 1.0 }, 
  color_diffuse[]  = { 0.0, 1.0, 1.0, 1.0 },
  color_specular[] = { 1.0, 1.0, 1.0, 1.0 },
  white[] = { 1.0, 1.0, 1.0, 1.0 }, 
  black[] = { 0.0, 0.0, 0.0, 1.0 };

///////////////////////////////Function - loadPPM///////////////////////////
//LoadPPM - a very minimal Portable Pixelformat image file loader		  //
//   fname: name of file to load (input)								  //
//   w: width of loaded image in pixels (output)						  //
//   h: height of loaded image in pixels (output)						  //
//   data: image data address (input or output depending on mallocflag)   //
//   mallocflag: 1 if memory not pre-allocated, 0 if data already points  //
//               to allocated memory that can hold the image.			  //
//			   Note that if new memory is allocated, free() should be used//
//			   to deallocate when it is no longer needed.				  //
////////////////////////////////////////////////////////////////////////////
void LoadPPM(char *fname, unsigned int *w, unsigned int *h, 
					unsigned char **data, int mallocflag)
{
	FILE *fp;
	char P, num;
	int max;
	char s[1000];

	if (!(fp = fopen(fname, "rb")))
	{
		perror("cannot open image file\n");
		exit(0);
	}//if

	fscanf(fp, "%c%c\n", &P, &num);
	if ((P != 'P') || (num != '6'))
	{
		perror("unknown file format for image\n");
		exit(0);
	}//if

	do
	{
		fgets(s, 999, fp);
	}
	while (s[0] == '#');

	sscanf(s, "%d%d", w, h);
	fgets(s, 999, fp);
	sscanf(s, "%d", &max);

	if (mallocflag)
		if (!(*data = (unsigned char *)malloc(*w * *h * 3)))
		{
			perror("cannot allocate memory for image data\n");
			exit(0);
		}//if

	fread(*data, 3, *w * *h, fp);
	fclose(fp);
}//loadPPM

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
	gmtl::Matrix44f temp;

	glClearColor(0.7, 0.7, 0.7, 0.0); 
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_TEXTURE_2D);

	//initialize matrix modes
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 1, 10);
	//glOrtho(-5,5,-5,5,0,5);
	glMatrixMode(GL_MODELVIEW);

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

////////////////////////////Function - initTextures//////////////////////////
// describe how data will be stored in memory
// generate a new "texture object" and select it for setup
// load an image into memory
// describe the image to the graphics system as a texture map
// select methods for "scaling" a texture region to a pixel
// select the method for combining texture color with the lighting equation
// (look up the third parameter)
// NOTE: to have another texture map, generate another texture object and
//       repeat the setup steps. To select which texture is being applied 
//       during drawing, use glBindTexture() to select.
/////////////////////////////////////////////////////////////////////////////
void initTextures()
{
	static unsigned texwidth, texheight;
	static unsigned char *imagedata;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
	glGenTextures(1, &ship_tex_1);  
    glBindTexture(GL_TEXTURE_2D, ship_tex_1);
    LoadPPM("spaceship texture 1.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &ship_tex_2);  
    glBindTexture(GL_TEXTURE_2D, ship_tex_2);
    LoadPPM("spaceship texture 3.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &borg_tex_1);  
    glBindTexture(GL_TEXTURE_2D, borg_tex_1);
    LoadPPM("borg texture 1.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_1);  
    glBindTexture(GL_TEXTURE_2D, space_tex_1);
    LoadPPM("space texture 1.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_2);  
    glBindTexture(GL_TEXTURE_2D, space_tex_2);
    LoadPPM("space texture 2.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_3);  
    glBindTexture(GL_TEXTURE_2D, space_tex_3);
    LoadPPM("space texture 3.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_4);  
    glBindTexture(GL_TEXTURE_2D, space_tex_4);
    LoadPPM("space texture 4.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_5);  
    glBindTexture(GL_TEXTURE_2D, space_tex_5);
    LoadPPM("space texture 5.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_6);  
    glBindTexture(GL_TEXTURE_2D, space_tex_6);
    LoadPPM("space texture 6.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}//end initTextures

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
	which_subpart = 0; //0 = satellite, 1 = camera 1, 2 = camera 2, 3 = light
	show_ambient = true;
	show_diffuse = true;
	show_specular = true;
	
	//initialize light starting position
	light_pos.set(1,1,-1,1);

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

///////////////////////////////Function - setlights///////////////////////////////////
void setlights(void)
{
	glEnable(GL_DEPTH_TEST);

	glLightfv(GL_LIGHT0, GL_POSITION, light_pos.getData());
	if(show_ambient) 
		glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	else glLightfv(GL_LIGHT0, GL_AMBIENT, black); 
	if(show_diffuse) 
		glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	else glLightfv(GL_LIGHT0, GL_DIFFUSE, black); 
	if(show_specular) 
		glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	else glLightfv(GL_LIGHT0, GL_SPECULAR, black); 

	glEnable(GL_LIGHTING);
}//setlights

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

	glLoadIdentity(); 
	glMaterialf(GL_FRONT, GL_SHININESS, 80.0); /* set material properties */
	glMaterialfv(GL_FRONT, GL_AMBIENT, color_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color_specular);

	switch(which_view)
	{
		case 0:
			//draw skybox
			glLoadIdentity();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_world.p);
			gmtl::set(rot_matrix, camera_pq_world.q);
			inverse_matrix = trans_matrix * rot_matrix;
			gmtl::invert(inverse_matrix);
			inverse_matrix[3][0] = 0; inverse_matrix[3][1] = 0; inverse_matrix[3][2] = 0;
			glLoadMatrixf(inverse_matrix.getData());
			glMultMatrixf(trans_matrix.getData());

			glDisable(GL_DEPTH_TEST);
			glBindTexture(GL_TEXTURE_2D, space_tex_1);
			visuals.drawNearFace();
			glBindTexture(GL_TEXTURE_2D, space_tex_2);
			visuals.drawFarFace();
			glBindTexture(GL_TEXTURE_2D, space_tex_3);
			visuals.drawLeftFace();
			glBindTexture(GL_TEXTURE_2D, space_tex_4);
			visuals.drawRightFace();
			glBindTexture(GL_TEXTURE_2D, space_tex_5);
			visuals.drawTopFace();
			glBindTexture(GL_TEXTURE_2D, space_tex_6);
			visuals.drawBottomFace();

			setlights();

			//load inverse camera matrix as modelview matrix
			glLoadIdentity();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(camera_pq_world.p);
			gmtl::set(rot_matrix, camera_pq_world.q);
			inverse_matrix = trans_matrix * rot_matrix;
			gmtl::invert(inverse_matrix);
			glLoadMatrixf(inverse_matrix.getData());
			glPushMatrix();

			//draw world axes and light sphere
			glBindTexture(GL_TEXTURE_2D, borg_tex_1);
			visuals.drawCube();
			visuals.drawLeftWorldAxes();
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(light_pos);
			glMultMatrixf(trans_matrix.getData());
			visuals.drawLight();
			glPopMatrix();
			glPushMatrix();

			//draw plane 1 and local axes
			trans_matrix = gmtl::makeTrans<gmtl::Matrix44f>(plane_pq_1.p);
			gmtl::set(rot_matrix, plane_pq_1.q);
			glMultMatrixf(trans_matrix.getData());
			glMultMatrixf(rot_matrix.getData());
			glBindTexture(GL_TEXTURE_2D, ship_tex_1);
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
			glBindTexture(GL_TEXTURE_2D, ship_tex_2);
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
	    case 0: plane_temp = camera_pq_world.q*gmtl::Quatf(temp_pq.p[0], temp_pq.p[1], 
					temp_pq.p[2], 0) * gmtl::makeConj(camera_pq_world.q);
				camera_pq_world.p[0] += plane_temp[0];
				camera_pq_world.p[1] += plane_temp[1];
				camera_pq_world.p[2] += plane_temp[2];
				plane_temp = temp_pq.q * gmtl::Quatf(camera_pq_world.p[0], camera_pq_world.p[1], 
					camera_pq_world.p[2], 0) * gmtl::makeConj(temp_pq.q);
				camera_pq_world.p[0] = plane_temp[0]; 
				camera_pq_world.p[1] = plane_temp[1]; 
				camera_pq_world.p[2] = plane_temp[2]; 
				camera_pq_world.q = temp_pq.q * camera_pq_world.q;
		break;
		//camera 1 
	    case 1: plane_temp = camera_pq_1.q*gmtl::Quatf(temp_pq.p[0], temp_pq.p[1], 
					temp_pq.p[2], 0) * gmtl::makeConj(camera_pq_1.q);
				camera_pq_1.p[0] += plane_temp[0];
				camera_pq_1.p[1] += plane_temp[1];
				camera_pq_1.p[2] += plane_temp[2];
				plane_temp = temp_pq.q*gmtl::Quatf(camera_pq_1.p[0], camera_pq_1.p[1], 
					camera_pq_1.p[2], 0) * gmtl::makeConj(temp_pq.q);
				camera_pq_1.p[0] = plane_temp[0]; 
				camera_pq_1.p[1] = plane_temp[1]; 
				camera_pq_1.p[2] = plane_temp[2];
				camera_pq_1.q = temp_pq.q * camera_pq_1.q;
		  break;
		//camera 2
	    case 2: plane_temp = camera_pq_2.q*gmtl::Quatf(temp_pq.p[0], temp_pq.p[1], 
					temp_pq.p[2], 0) * gmtl::makeConj(camera_pq_2.q);
				camera_pq_2.p[0] += plane_temp[0];
				camera_pq_2.p[1] += plane_temp[1];
				camera_pq_2.p[2] += plane_temp[2];
				camera_pq_2.q = temp_pq.q * camera_pq_2.q;
				plane_temp = temp_pq.q*gmtl::Quatf(camera_pq_2.p[0], camera_pq_2.p[1], 
					camera_pq_2.p[2], 0) * gmtl::makeConj(temp_pq.q);
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

//////////////////////////////Function - keyboardhandler///////////////////////////////
//					Plane:     Subpart:     Camera:     Light:						 //
// Translation(+):	  w																 //
// Yaw(+/-):		 a/d         f/h												 //
// Pitch(+/-):		 q/e         t/g												 //
// Roll(+/-):		 z/c															 //
// Distance(+/-):							  u/o									 //
// Azimuth(+/-):							  j/l									 //
// Elevation(+/-):							  i/k									 //
// Trans-x(+/-):										  h/f						 //
// Trans-y(+/-):										  t/g						 //
// Trans-z(+/-):                                          r/y                        //
//																					 //
// Extra Commands:																	 //
//	1 - switch among the three camera views		8 - switch ambient light on/off		 //
//	2 - switch between the two planes			9 - switch diffuse light on/off		 //
//  3 - switch among three subparts on plane 2	0 - switch specular light on/off	 //
//      and the light																 //
//	4 - turn coordinate axes on or off			Spacebar - return to initial settings//
///////////////////////////////////////////////////////////////////////////////////////
void keyboardhandler(unsigned char key, int x, int y)
{
	switch(key)
	{
	//camera distance (+/-)
		case 'u': calculatePQ(distance_pos, 0);
			break;
		case 'o': calculatePQ(distance_neg, 0);
			break;
	//camera elevation (+/-)
		case 'i': calculatePQ(roll_ccw, 0);
			break;
		case 'k': calculatePQ(roll_cw, 0);
			break;
	//camera azimuth (+/-)
		case 'j': calculatePQ(yaw_ccw, 0);
			break;
		case 'l': calculatePQ(yaw_cw, 0);
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
	//subpart yaw (+/-) or light x-trans
		case 'f': if(which_subpart == 3) light_pos[0] -= 0.1;
			      else if(!which_plane) calculatePQ(yaw_ccw, 2);
			break;
		case 'h': if(which_subpart == 3) light_pos[0] += 0.1;
				  else if(!which_plane) calculatePQ(yaw_cw, 2);
			break;
	//subpart pitch (+/-) or light y-trans
		case 't': if(which_subpart == 3) light_pos[1] += 0.1;
				  else if(!which_plane) calculatePQ(pitch_down, 3);
			break;
		case 'g': if(which_subpart == 3) light_pos[1] -= 0.1;
				  else if(!which_plane) calculatePQ(pitch_up, 3);
			break;
	//light z_trans
		case 'r': if(which_subpart == 3) light_pos[2] -= 0.1;
			break;
		case 'y': if(which_subpart == 3) light_pos[2] += 0.1;
			break;
		case '1': which_view = (which_view + 1) % 3;
			break;
		case '2': which_plane = !which_plane;
			break;
		case '3': which_subpart = (which_subpart + 1) % 4;
			break;
		case '4': show_axes = !show_axes;
			break;
		case '8': show_ambient = !show_ambient;
			break;
		case '9': show_diffuse = !show_diffuse;
			break;
		case '0': show_specular = !show_specular;
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
	glutCreateWindow("Assignment 4");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardhandler);
	init();
	initTextures();
	reset();
	glutMainLoop();
	return 0;
}//end main