////////////////////////////////////////////////////////////////////
//Program Assignment 6 - Nick Handelman (nrh8883) - CMPS 515	  //
//																  //
//This is project involves the use of particles and an object	  //
//moving along a piecewise cubic path. Many features from		  //
//previous assignments are used:								  //
//	1. A skybox is added										  //
//  2. 3 cameras are switchable and follow the distance/azimuth/  //
//     elevation model: world and 2 plane					      //
//  3. Coordinate frames can be turned on and off				  //
//  4. Plane guns/barrels can be rotated. There are two of them.  //
//  5. Lighting options are the same as from assignment four	  //
//  6. Plane textures are added									  //
//																  //
////////////////////////////////////////////////////////////////////

////////////////Libraries, Constants, Global Variables////////////////
#include <stdlib.h>
#include <math.h>
#include <gmtl/gmtl.h>
#include <GL/glut.h> 
#include "Visuals.h"
#include "Particles.h"

#define USE_DEVELOPMENT_MODE  // Comment it to run the application on the 3d display

struct Cubic_Path_Info
{
	int num_points, t_interval, time;
	gmtl::Point3f points[MAX_SEGMENTS];
	gmtl::Vec3f tangents[MAX_SEGMENTS];
	gmtl::Matrix44f coeff_matrices[MAX_SEGMENTS];
	float scale_factor;
	bool show_pts_tans;
};

Visuals visuals = Visuals();
Particles engine, missile;

//view frustum
enum Eye {LEFT_EYE, RIGHT_EYE};
const float IPD_D2 = 0.07f/2;     // Interpupillary distance divided by 2
const float VF_NEAR = 1;          // Distance to near clipping plane
const float VF_FAR = 100;         // Distance to far clipping plane
const float VF_FD = 2.5;          // Distance to focal plane (focal depth)
const float VF_LEFT = -2.05;      // Left clipping coordinate at focal depth
const float VF_RIGHT = 2.05;      // Right clipping coordinate at focal depth
const float VF_BOTTOM = -1.6;     // Bottom clipping coordinate at focal depth
const float VF_TOP = 0.68;        // Top clipping coordinate at focal depth
const float VF_S = VF_NEAR/VF_FD; // Scale factor

//camera movement, subpart movement
//plane matrices, gun and barrel matrices, camera matrices, light position
gmtl::Matrix44f dist_pos, dist_neg, elev_ccw, elev_cw, azim_ccw, azim_cw;
gmtl::Matrix44f yaw_cw, yaw_ccw, pitch_ccw, pitch_cw;
gmtl::Matrix44f plane_1_world, plane_2_world, plane_1_camera, plane_2_camera; 
gmtl::Matrix44f gun_top, gun_bottom, barrel_top, barrel_bottom;
gmtl::Matrix44f camera_world, camera_1, camera_2;
gmtl::Matrix44f light_pos;

//texture variables
static GLuint ship_tex_1;
static GLuint space_tex_1, space_tex_2, space_tex_3, space_tex_4, space_tex_5, space_tex_6;

//user option variables
bool show_axes, show_ambient, show_diffuse, show_specular;
int which_view, which_subpart, which_particle;
bool draw_left_eye = true;

//light variables
GLfloat color_ambient[4], color_diffuse[4], color_specular[4], white[4], black[4];

//variables used in plane motion
gmtl::Matrix44f spline_basis;
Cubic_Path_Info plane_1_path, plane_2_path;

///////////////////////////////Function - loadPPM///////////////////////////
//loadPPM - a very minimal Portable Pixelformat image file loader		  //
//   fname: name of file to load (input)								  //
//   w: width of loaded image in pixels (output)						  //
//   h: height of loaded image in pixels (output)						  //
//   data: image data address (input or output depending on mallocflag)   //
//   mallocflag: 1 if memory not pre-allocated, 0 if data already points  //
//               to allocated memory that can hold the image.			  //
//			   Note that if new memory is allocated, free() should be used//
//			   to deallocate when it is no longer needed.				  //
////////////////////////////////////////////////////////////////////////////
void loadPPM(char *fname, unsigned int *w, unsigned int *h, 
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

/////////////////////////Function - compCoeffMats/////////////////////////
//Computes the coefficient matrices (C = MG) where M = Bspline basis and//
//G = geometric matrix. M is preset and G is computed in this function. //
//The coefficient matrices are determined for all cubic segments        //
//////////////////////////////////////////////////////////////////////////
void compCoeffMats()
{
	gmtl::Matrix44f geometric_matrix;
	int pt_one, pt_two, pt_three;

	//compute coefficient matrices for plane 1 cubic path segments
	for(int pt_four = 0; pt_four < plane_1_path.num_points; pt_four++)
	{
		pt_one = (pt_four+(plane_1_path.num_points-3))%plane_1_path.num_points;
		pt_two = (pt_four+(plane_1_path.num_points-2))%plane_1_path.num_points;
		pt_three = (pt_four+(plane_1_path.num_points-1))%plane_1_path.num_points;
		
		//set the geometric matrix
		geometric_matrix.set(
	plane_1_path.points[pt_one][0], plane_1_path.points[pt_one][1], plane_1_path.points[pt_one][2], 0,
	plane_1_path.points[pt_two][0],   plane_1_path.points[pt_two][1],   plane_1_path.points[pt_two][2],   0,
	plane_1_path.tangents[pt_three][0], plane_1_path.tangents[pt_three][1], plane_1_path.tangents[pt_three][2], 0,
	plane_1_path.tangents[pt_four][0], plane_1_path.tangents[pt_four][1], plane_1_path.tangents[pt_four][2], 0);
	    
		//set the coefficient matrix
		plane_1_path.coeff_matrices[pt_four] = spline_basis*geometric_matrix;
	}//end for 

	//compute coefficient matrices for plane 2 cubic path segments
	for(int pt_four = 0; pt_four < plane_2_path.num_points; pt_four++)
	{
		pt_one = (pt_four+(plane_2_path.num_points-3))%plane_2_path.num_points;
		pt_two = (pt_four+(plane_2_path.num_points-2))%plane_2_path.num_points;
		pt_three = (pt_four+(plane_2_path.num_points-1))%plane_2_path.num_points;
		
		//set the geometric matrix
		geometric_matrix.set(
	plane_2_path.points[pt_one][0], plane_2_path.points[pt_one][1], plane_2_path.points[pt_one][2], 0,
	plane_2_path.points[pt_two][0],   plane_2_path.points[pt_two][1],   plane_2_path.points[pt_two][2],   0,
	plane_2_path.tangents[pt_three][0], plane_2_path.tangents[pt_three][1], plane_2_path.tangents[pt_three][2], 0,
	plane_2_path.tangents[pt_four][0], plane_2_path.tangents[pt_four][1], plane_2_path.tangents[pt_four][2], 0);
	    
		//set the coefficient matrix
		plane_2_path.coeff_matrices[pt_four] = spline_basis*geometric_matrix;
	}//end for 
}//end compCoeffMats

////////////////////////////Function - init/////////////////////////////
//One time initialization of certain settings						  //
////////////////////////////////////////////////////////////////////////
void init()
{
	gmtl::Matrix44f temp;

	//set light and texture variables
	glClearColor(0.7, 0.7, 0.7, 0.0); 
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	for(int i = 0; i < 4; i++)
	{
		white[i] = 1.0;
		black[i] = 0.0;
	}//end for
	black[3] = 1.0;
	color_ambient[0] =  1.0;  color_ambient[1] =  1.0;
	color_ambient[2] =  0.0;  color_ambient[3] =  1.0;
	color_diffuse[0] =  0.0;  color_diffuse[1] =  1.0;
	color_diffuse[2] =  1.0;  color_diffuse[3] =  1.0;
	color_specular[0] = 1.0;  color_specular[1] = 1.0; 
	color_specular[2] = 1.0;  color_specular[3] = 1.0; 

	//set camera movement matrices
	dist_pos = gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0,0,TRANS));
	dist_neg = gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0,0,-TRANS));
	elev_ccw = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			   (gmtl::Math::deg2Rad(-ELEV_DEG),0.f,0.f));
	elev_cw =  gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			   (gmtl::Math::deg2Rad(ELEV_DEG),0.f,0.f));
	azim_ccw = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			   (0.f,gmtl::Math::deg2Rad(-AZIM_DEG),0.f));
	azim_cw =  gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			   (0.f,gmtl::Math::deg2Rad(AZIM_DEG),0.f));

	//set gun and barrel movement matrices
	yaw_ccw =   gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			    (0.f,gmtl::Math::deg2Rad(-YAW_DEG),0.f));
	yaw_cw =    gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			    (0.f,gmtl::Math::deg2Rad(YAW_DEG),0.f));
	pitch_ccw = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
				(0.f,0.f,gmtl::Math::deg2Rad(-PITCH_DEG)));
	pitch_cw =  gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
				(0.f,0.f,gmtl::Math::deg2Rad(PITCH_DEG)));

	//set cubic path information
	gmtl::Point3f pt;
	spline_basis.set(-0.166, 0.5, -0.5, 0.166, 0.5, -1, 0.5, 0, -0.5, 0, 0.5, 0, 0.166, 0.666, 0.166, 0);

	pt.set( 0, 1, -2); plane_1_path.points[2] = pt;
	pt.set( 0, 0, -5); plane_1_path.points[0] = pt;
	pt.set( -1, -2, -3); plane_1_path.points[1] = pt;
	pt.set( 2, -4, 0); plane_1_path.points[3] = pt;
	pt.set( 5, 4, -6); plane_1_path.points[4] = pt;
	pt.set( -1, 10, 10); plane_1_path.points[5] = pt;
	plane_1_path.num_points = 6;

	pt.set( -2, 0, 1); plane_2_path.points[0] = pt;
	pt.set( 0, -5, 0); plane_2_path.points[1] = pt;
	pt.set( -2, -3, -1); plane_2_path.points[2] = pt;
	pt.set( 2, -4, 0); plane_1_path.points[3] = pt;
	pt.set( 5, 4, -6); plane_1_path.points[4] = pt;
	pt.set( -1, 10, 10); plane_1_path.points[5] = pt;
	plane_2_path.num_points = 6;

	compCoeffMats();
}//end init

//////////////////////Function - initTextures//////////////////////////
//One-time initialization of textures from .ppm files				 //
///////////////////////////////////////////////////////////////////////
void initTextures()
{
	unsigned texwidth, texheight;
	unsigned char *imagedata;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
	glGenTextures(1, &ship_tex_1);  
    glBindTexture(GL_TEXTURE_2D, ship_tex_1);
    loadPPM("spaceship texture 1.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_1);  
    glBindTexture(GL_TEXTURE_2D, space_tex_1);
    loadPPM("space texture 1.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_2);  
    glBindTexture(GL_TEXTURE_2D, space_tex_2);
    loadPPM("space texture 2.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_3);  
    glBindTexture(GL_TEXTURE_2D, space_tex_3);
    loadPPM("space texture 3.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_4);  
    glBindTexture(GL_TEXTURE_2D, space_tex_4);
    loadPPM("space texture 4.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_5);  
    glBindTexture(GL_TEXTURE_2D, space_tex_5);
    loadPPM("space texture 5.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glGenTextures(1, &space_tex_6);  
    glBindTexture(GL_TEXTURE_2D, space_tex_6);
    loadPPM("space texture 6.ppm", &texwidth, &texheight, &imagedata, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, 
	             GL_RGB, GL_UNSIGNED_BYTE, imagedata);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);        
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP); 
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}//end initTextures

////////////////////////////Function - reset/////////////////////////////
//Initialization of certain settings. May be initialized more than once//
//during program execution.											   //
/////////////////////////////////////////////////////////////////////////
void reset()
{	
	gmtl::Matrix44f rot_180;
	gmtl::Point3f cavg, cdev, pavg, pdev;
	gmtl::Vec3f vavg, vdev;

	rot_180 = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
		(gmtl::Math::deg2Rad(180.0),0.f,0.f));

	//initialize global variables
	which_view = 0; //0 = world, 1 = plane 1, 2 = plane 2
	show_axes = false; 
	which_subpart = 0; //light = 0, gun 1 (top) = 1, gun 2 (bottom) = 2
	show_ambient = true;
	show_diffuse = true;
	show_specular = true;
	which_particle = 0;
	draw_left_eye = true;

	//set plane matrices
	plane_1_world = gmtl::makeTrans<gmtl::Matrix44f>(plane_1_path.points[0]);
	plane_1_camera = gmtl::makeTrans<gmtl::Matrix44f>(plane_1_path.points[0]);
	plane_2_world = gmtl::makeTrans<gmtl::Matrix44f>(plane_2_path.points[0]);
	plane_2_camera = gmtl::makeTrans<gmtl::Matrix44f>(plane_2_path.points[0]);

	//set gun and barrel matrices
	gun_top =	    gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(-0.13,0.125,0));
	gun_bottom =    gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(-0.12,-0.095,0));
	gun_bottom =    gun_bottom * rot_180;
	barrel_top =    gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0,0.045,0));
	barrel_bottom = gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0,0.045,0));

	//set camera matrices
	camera_world = gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0,0,3));
	camera_1 =	   gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0,0,2));
	camera_2 =     gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(0,0,2));

	//set light position
	light_pos = gmtl::makeTrans<gmtl::Matrix44f>(gmtl::Vec3f(1,1,-1));

	//set cubic path variables
	plane_1_path.show_pts_tans = false;
	plane_2_path.show_pts_tans = false;
	plane_1_path.scale_factor = 1;
	plane_2_path.scale_factor = 1;
	plane_1_path.t_interval = T_INTERVAL;
	plane_2_path.t_interval = T_INTERVAL;
	plane_1_path.time = 0;
	plane_2_path.time = 0;

	//set particle variables for engine particles
	cavg.set(0.0, 0.0, 1.0); cdev.set(0.0, 0.0, 0.0);
	pavg.set(0.0, 0.0, 0.0); pdev.set(0.02, 0.02, 0.02);
	vavg.set(0.0, 0.0, 0.1); vdev.set(0.1, 0.1, 0.1);
	engine = Particles(20, 1, cavg, cdev, 1, 0, 10, 2, 0.1, 0.02, pavg, pdev, vavg, vdev);
	engine.time_step = 1;
	engine.global_accel.set(0.1, 0.0, 0.0);
	engine.drag_coeff = 1;
	engine.wind_vel.set(0.0, 0.0, 0.0);

	//set particle variables for missile particles
	cavg.set(0.5, 0.5, 0.5); cdev.set(0.2, 0.2, 0.2);
	pavg.set(0.0, 0.2, 0.0); pdev.set(0.2, 0.0, 0.2);
	vavg.set(0.0, 0.02, 0.0); vdev.set(0.01, 0.0, 0.01);
	missile = Particles(1000, 0, cavg, cdev, 10, 2, 20, 5, 0.1, 0.02, pavg, pdev, vavg, vdev);
	missile.time_step = 1;
	missile.global_accel.set(0.0, 0.0, 0.0);
	missile.drag_coeff = 0;
	missile.wind_vel.set(0.0, 0.0, 0.0);
}//end reset

///////////////////////////Function - setlights//////////////////////////////
//set ambient, diffuse and specular light terms							   //
/////////////////////////////////////////////////////////////////////////////
void setLights(void)
{
	gmtl::Vec4f temp;

	for(int i = 0; i < 4; i++)
		temp[i] = light_pos.operator()(i,3);

	glLightfv(GL_LIGHT0, GL_POSITION, temp.getData());
	
	//turn light components on or off
	if(show_ambient) glLightfv(GL_LIGHT0, GL_AMBIENT, white);
	else glLightfv(GL_LIGHT0, GL_AMBIENT, black); 
	
	if(show_diffuse) glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	else glLightfv(GL_LIGHT0, GL_DIFFUSE, black); 
	
	if(show_specular) glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	else glLightfv(GL_LIGHT0, GL_SPECULAR, black); 

	glEnable(GL_LIGHTING);
}//setlights

/////Functions - computePlaneMotion, getTC, compPos, compVel, compAccel//////
//The following four functions compute position, velocity and acceleration.//
//The fifth function computes a plane's new position and orientation using //
// the previous four functions.											   //
//																		   //
//getTC() returns the correct coefficient matrix for the plane's current   //
//cubic path segment and the plane's time.								   //
//																		   //
// C = [ax ay az]   M = [ 2 -2  1  1]   G = [P0x P0y P0z]				   //
//	   [bx by bz]       [-3  3 -2 -1]       [P1x P1y P1z]				   //
//	   [cx cy cz]       [ 0  0  1  0]	    [V0x V0y V0z]				   //
//	   [dx dy dz]       [ 1  0  0  0]       [V1x V1y V1z]				   //
//																		   //
// Q(t) = TMG, C = MG,													   //
// Position: T = [t^3 t^2 t 1] - applied by Horner's rule				   //
// Velocity: T = [3t^2 2t 1 0]											   //
// Acceleration: T = [6t 2 0 0]											   //
/////////////////////////////////////////////////////////////////////////////
gmtl::Matrix44f getTC(int p, float* time)
{
	gmtl::Matrix44f mat;
	int a;

	if(p == 1) //plane 1
	{
		a = plane_1_path.time/T_RANGE;
		mat = plane_1_path.coeff_matrices[a];
		*time = (float)plane_1_path.time / T_RANGE - a;
	}//end if
	else //plane 2
	{
		a = plane_2_path.time/T_RANGE;
		mat = plane_2_path.coeff_matrices[a];
		*time = (float)plane_2_path.time / T_RANGE - a;
	}//end else

	return mat;
}//end getTC

gmtl::Point3f compPos(int p)
{
	gmtl::Point3f new_pt;
	gmtl::Matrix44f c_mat;
	float t;

	//get coefficient matrix and time parameter
	c_mat = getTC(p, &t);

	//calculate x(t), y(t), z(t)
	for(int i = 0; i < 3; i++)
		new_pt[i] = ((c_mat.operator()(0,i) * t + c_mat.operator()(1,i)) *
			t + c_mat.operator()(2,i)) * t + c_mat.operator()(3,i);

	return new_pt;
}//end compXYZ

gmtl::Vec3f compVel(int p)
{
	gmtl::Vec3f new_tan;
	gmtl::Matrix44f c_mat;
	float t; 

	//get coefficient matrix and time parameter
	c_mat = getTC(p, &t);

	//calculate x'(t), y'(t), z'(t)
	for(int i = 0; i < 3; i++)
		new_tan[i] = 3*c_mat.operator()(0,i)*t*t + 2*c_mat.operator()(1,i)*t + 
				 c_mat.operator()(2,i);
	gmtl::normalize(new_tan);

	return new_tan;
}//end compTan

gmtl::Vec3f compAccel(int p)
{
	gmtl::Vec3f new_tan;
	gmtl::Matrix44f c_mat;
	float t; 

	//get coefficient matrix and time parameter
	c_mat = getTC(p, &t);

	//calculate x''(t), y''(t), z''(t)
	for(int i = 0; i < 3; i++)
		new_tan[i] = 6*c_mat.operator()(0,i)*t + 2*c_mat.operator()(1,i);
	gmtl::normalize(new_tan);

	return new_tan;
}//end compAccel

void computePlaneMotion()
{
	gmtl::Vec3f y_world, z_column, x_column, y_column, accel_vector, cx_column;
	gmtl::Point3f new_pos;
	gmtl::Matrix44f rot_mat;
	float roll_angle;

	y_world.set(0,1,0);

	//compute new x,y,z position for planes 1 and 2
	new_pos = compPos(1);
	for(int i = 0; i < 3; i++)
	{
		plane_1_world.operator()(i,3) = new_pos[i];
		plane_1_camera.operator()(i,3) = new_pos[i];
	}//end for

	new_pos = compPos(2);
	for(int i = 0; i < 3; i++)
	{
		plane_2_world.operator()(i,3) = new_pos[i];
		plane_2_camera.operator()(i,3) = new_pos[i];
	}//end for

	//compute velocity and accleration at both planes' current points
	//to determine a banking angle, add a 90 degree local y rotation 
	//to point plane in DOF
	z_column = compVel(1);
	x_column = gmtl::makeCross(y_world, z_column);
	cx_column = gmtl::makeCross(z_column, y_world);
	y_column = gmtl::makeCross(z_column, x_column);
	for(int i = 0; i < 3; i++)
	{
		plane_1_world.operator()(i,0) = x_column[i];
		plane_1_world.operator()(i,1) = y_column[i];
		plane_1_world.operator()(i,2) = z_column[i];
		plane_1_camera.operator()(i,0) = cx_column[i];
		plane_1_camera.operator()(i,1) = y_world[i];
		plane_1_camera.operator()(i,2) = z_column[i];
	}//end for
	accel_vector = compAccel(1);
	roll_angle = gmtl::dot(accel_vector, x_column);
	roll_angle = asin(roll_angle/13);
	rot_mat = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			  (0.f,gmtl::Math::deg2Rad(90.0),roll_angle));
	plane_1_world *= rot_mat;
	rot_mat = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			  (0.f,gmtl::Math::deg2Rad(90.0),0.f));
	plane_1_camera *= rot_mat;

	z_column = compVel(2);
	x_column = gmtl::makeCross(y_world, z_column);
	cx_column = gmtl::makeCross(z_column, y_world);
	y_column = gmtl::makeCross(z_column, x_column);
	for(int i = 0; i < 3; i++)
	{
		plane_2_world.operator()(i,0) = x_column[i];
		plane_2_world.operator()(i,1) = y_column[i];
		plane_2_world.operator()(i,2) = z_column[i];
		plane_2_camera.operator()(i,0) = cx_column[i];
		plane_2_camera.operator()(i,1) = y_world[i];
		plane_2_camera.operator()(i,2) = z_column[i];
	}//end for
	accel_vector = compAccel(2);
	roll_angle = gmtl::dot(accel_vector, x_column);
	roll_angle = asin(roll_angle/13);
	rot_mat = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			  (0.f,gmtl::Math::deg2Rad(90.0),roll_angle));
	plane_2_world *= rot_mat;
	rot_mat = gmtl::makeRot<gmtl::Matrix44f>(gmtl::EulerAngleXYZf
			  (0.f,gmtl::Math::deg2Rad(90.0),0.f));
	plane_2_camera *= rot_mat;
}//end computePlaneMotion

/////////////////////Function - drawPtsTans, drawSkyBox/////////////////////
//The function drawPtsTans draws the input points and tangents results	  //
//from the nearest two points.											  //
//The function drawSkyBox draws the sky box.							  //
////////////////////////////////////////////////////////////////////////////
void drawPtsTans()
{
	gmtl::Matrix44f temp;
	float pt_color[3] = {1.0, 0.0, 0.0};
	if(plane_1_path.show_pts_tans)//draw points for plane 1
	{
		for(int i = 0; i < plane_1_path.num_points; i++)
		{
			glPushMatrix();
			temp = gmtl::makeTrans<gmtl::Matrix44f>(plane_1_path.points[i]);
			glMultMatrixf(temp.getData());
			visuals.drawSpherePts(0.1, pt_color);
			//visuals.drawTangents(plane_1_path.tangents[i], pt_color);
			glPopMatrix();
		}//end for
	}//end if
	if(plane_2_path.show_pts_tans) //draw points for plane 2
	{
		for(int i = 0; i < plane_2_path.num_points; i++)
		{
			glPushMatrix();
			temp = gmtl::makeTrans<gmtl::Matrix44f>(plane_2_path.points[i]);
			glMultMatrixf(temp.getData());
			visuals.drawSpherePts(0.1, pt_color);
			//visuals.drawTangents(plane_2_path.tangents[i], pt_color);
			glPopMatrix();
		}//end for
	}//end if
}//end drawPtsTans

void drawSkyBox()
{
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
	glEnable(GL_DEPTH_TEST);
}//end drawSkyBox

void drawScene(int eye)
{
	gmtl::Matrix44f inverse;
	float light_color[3] = {1.0, 0.0, 0.0};
	static int max_time_1 = plane_1_path.num_points * T_RANGE;
	static int max_time_2 = plane_2_path.num_points * T_RANGE;
	
	// Specify a color buffer to draw to
	#ifdef USE_DEVELOPMENT_MODE
		glDrawBuffer(GL_BACK);
	#else
		if (eye == LEFT_EYE)
			 glDrawBuffer(GL_BACK_LEFT);
		else glDrawBuffer(GL_BACK_RIGHT);
	#endif
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Set view frustum
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (eye == LEFT_EYE)
		 glFrustum( (VF_LEFT+IPD_D2)*VF_S, (VF_RIGHT+IPD_D2)*VF_S, VF_BOTTOM*VF_S, VF_TOP*VF_S, VF_NEAR, VF_FAR );
	else glFrustum( (VF_LEFT-IPD_D2)*VF_S, (VF_RIGHT-IPD_D2)*VF_S, VF_BOTTOM*VF_S, VF_TOP*VF_S, VF_NEAR, VF_FAR );

	// Set view transform
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	if (eye == LEFT_EYE) // left/right eye shift for 3D viewing
		 glTranslatef(IPD_D2, 0, 0);
    else glTranslatef(-IPD_D2, 0, 0);

	computePlaneMotion();

	//draw stuff based on the view
	switch(which_view)
	{
	case 0:
		//draw skybox
		glEnable(GL_TEXTURE_2D);
		inverse = gmtl::makeInvert(camera_world);
		inverse.operator()(0,3) = 0;
		inverse.operator()(1,3) = 0;
		inverse.operator()(2,3) = 0;
		glLoadMatrixf(inverse.getData());
		drawSkyBox();
		glDisable(GL_TEXTURE_2D);

		//draw at world
		setLights();
		inverse = gmtl::makeInvert(camera_world);
		glLoadMatrixf(inverse.getData());
		drawPtsTans();
		if(show_axes)
			visuals.drawLeftWorldAxes();

		//draw the light sphere
		glPushMatrix();
		glMultMatrixf(light_pos.getData());
		visuals.drawSpherePts(0.3, light_color); 
		glPopMatrix();

		glEnable(GL_TEXTURE_2D);
		glMaterialf(GL_FRONT, GL_SHININESS, 80.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, color_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color_specular);

		//draw plane 1
		glPushMatrix();
		glMultMatrixf(plane_1_world.getData());
		glBindTexture(GL_TEXTURE_2D, ship_tex_1);
		visuals.drawPlane();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		//draw plane 1 engine particles
		engine.update();
		engine.drawParticles();

		//draw plane 1 missile particles
		missile.update();
		missile.drawParticles();
		glDisable(GL_BLEND);

		//draw plane 1 subparts
		glPushMatrix();
		glMultMatrixf(gun_top.getData());
		visuals.drawGun();
		glMultMatrixf(barrel_top.getData());
		visuals.drawBarrel();
		glPopMatrix();
		glMultMatrixf(gun_bottom.getData());
		visuals.drawGun();
		glMultMatrixf(barrel_bottom.getData());
		visuals.drawBarrel();
		glPopMatrix();

		//draw plane 2
		glPushMatrix();
		glMultMatrixf(plane_2_world.getData());
		glBindTexture(GL_TEXTURE_2D, ship_tex_1);
		visuals.drawPlane();
		glPopMatrix();
		break;
	case 1:
		//draw skybox
		glEnable(GL_TEXTURE_2D);
		inverse = gmtl::makeInvert(camera_1);
		inverse.operator()(0,3) = 0;
		inverse.operator()(1,3) = 0;
		inverse.operator()(2,3) = 0;
		glLoadMatrixf(inverse.getData());
		drawSkyBox();
		glDisable(GL_TEXTURE_2D);

		glMaterialf(GL_FRONT, GL_SHININESS, 80.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, color_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color_specular);

		glEnable(GL_TEXTURE_2D);
		inverse = gmtl::makeInvert(camera_1);
		glLoadMatrixf(inverse.getData());

		//draw plane 1
		glPushMatrix();
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, ship_tex_1);
		visuals.drawPlane();

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		//draw plane 1 engine particles
		engine.update();
		engine.drawParticles();

		//draw plane 1 missile particles
		missile.update();
		missile.drawParticles();
		glDisable(GL_BLEND);

		//draw plane 1 subparts
		glMultMatrixf(gun_top.getData());
		visuals.drawGun();
		glMultMatrixf(barrel_top.getData());
		visuals.drawBarrel();
		glPopMatrix();
		glMultMatrixf(gun_bottom.getData());
		visuals.drawGun();
		glMultMatrixf(barrel_bottom.getData());
		visuals.drawBarrel();
		glPopMatrix();

		inverse = gmtl::makeInvert(plane_1_camera);
		glMultMatrixf(inverse.getData());

		//at the world
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		drawPtsTans();
		if(show_axes)
			visuals.drawLeftWorldAxes();
		glMultMatrixf(light_pos.getData());
		visuals.drawSpherePts(0.3, light_color); 
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);

		//draw plane 2
		glMultMatrixf(plane_2_world.getData());
		glBindTexture(GL_TEXTURE_2D, ship_tex_1);
		visuals.drawPlane();
		break;
	case 2:
		//draw skybox
		glEnable(GL_TEXTURE_2D);
		inverse = gmtl::makeInvert(camera_2);
		inverse.operator()(0,3) = 0;
		inverse.operator()(1,3) = 0;
		inverse.operator()(2,3) = 0;
		glLoadMatrixf(inverse.getData());
		drawSkyBox();
		glDisable(GL_TEXTURE_2D);

		glMaterialf(GL_FRONT, GL_SHININESS, 80.0);
		glMaterialfv(GL_FRONT, GL_AMBIENT, color_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, color_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, color_specular);

		glEnable(GL_TEXTURE_2D);
		inverse = gmtl::makeInvert(camera_2);
		glLoadMatrixf(inverse.getData());

		//draw plane 2
		glBindTexture(GL_TEXTURE_2D, ship_tex_1);
		visuals.drawPlane();

		inverse = gmtl::makeInvert(plane_2_camera);
		glMultMatrixf(inverse.getData());

		//at the world
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
		drawPtsTans();
		if(show_axes)
			visuals.drawLeftWorldAxes();
		glMultMatrixf(light_pos.getData());
		visuals.drawSpherePts(0.3, light_color); 
		glPopMatrix();
		glEnable(GL_TEXTURE_2D);

		//draw plane 1
		glMultMatrixf(plane_1_world.getData());
		glPushMatrix();
		glBindTexture(GL_TEXTURE_2D, ship_tex_1);
		visuals.drawPlane();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		//draw plane 1 engine particles
		engine.update();
		engine.drawParticles();

		//draw plane 1 missile particles
		missile.update();
		missile.drawParticles();
		glDisable(GL_BLEND);

		//draw plane 1 and its subparts
		glMultMatrixf(gun_top.getData());
		visuals.drawGun();
		glMultMatrixf(barrel_top.getData());
		visuals.drawBarrel();
		glPopMatrix();
		glMultMatrixf(gun_bottom.getData());
		visuals.drawGun();
		glMultMatrixf(barrel_bottom.getData());
		visuals.drawBarrel();
		break;
	}//end switch

	//update time for both planes
	plane_1_path.time += plane_1_path.t_interval;
	if(plane_1_path.time >= max_time_1)
		plane_1_path.time -= max_time_1;
	plane_2_path.time += plane_2_path.t_interval;
	if(plane_2_path.time >= max_time_2)
		plane_2_path.time -= max_time_2;

}//end drawScene

///////////////////////////////Function - display///////////////////////////
//displays window based on various settings								  //
////////////////////////////////////////////////////////////////////////////
void display()
{
	#ifdef USE_DEVELOPMENT_MODE
		drawScene( draw_left_eye ? LEFT_EYE : RIGHT_EYE );
	#else
		drawScene(LEFT_EYE);
		drawScene(RIGHT_EYE);
	#endif

	glutSwapBuffers();
}//end display

///////////////////////Function - adjustMatrix////////////////////////
//Receives a matrix with transformation information				    //
//Receives an int designating which object to transform				//
//   0/1: camera     2: subpart level 1     3: subpart level 2	    //
//Makes transformation to target object's matrix					//
//////////////////////////////////////////////////////////////////////
void adjustMatrix(gmtl::Matrix44f temp, int which_obj)
{
  switch(which_obj)
  {	
    case 0: //camera azimuth/elevation transform 
	  switch(which_view)
	  {	//world camera
	    case 0: camera_world = temp * camera_world;
		break; 
		//plane camera 
	    case 1: camera_1 = temp * camera_1;
		  break;
		case 2: camera_2 = temp * camera_2;
		  break;
	  }//switch
	break; //end case 0
	case 1: //camera distance transform
	  switch(which_view)
	  {	//world camera
	    case 0: camera_world *= temp;
		break; 
		//plane camera 
	    case 1: camera_1 *= temp;
		  break;
		case 2: camera_2 *= temp;
		  break;
	  }//switch
	break;
	case 2: //subpart level 1 transform
	  switch(which_subpart)
	  {	
		case 1: gun_top *= temp; 
		break;
		case 2: gun_bottom *= temp;
	    break;
	  }//switch
	break; //end case 2
	case 3: //subpart level 2 transform
	  switch(which_subpart)
	  { 
		case 1: barrel_top *= temp; 
		break;
		case 2: barrel_bottom *= temp;
		break;
	  }//switch
	break; //end case 3
  }//switch
}//adjustMatrix

/////////////////////////Functions - adjustPath/////////////////////////////
//The following functions all deal with variables involved with each      //
//planes cubic path: set scale factor, set t_interval, show pts/tans	  //
////////////////////////////////////////////////////////////////////////////
void adjustPath(int option, bool b)
{
	switch(option)
	{
		case 0: //set Scale Factor
			if(which_view == 1)
			{
				if(b) plane_1_path.scale_factor++;
				else plane_1_path.scale_factor--; 
			}//end if
			else if(which_view == 2)
			{
				if(b) plane_2_path.scale_factor++;
				else plane_2_path.scale_factor--;
			}//end else if
			break;
		case 1: //set T-interval
			if(which_view == 1)
			{
				if(b)
				{
					if(plane_1_path.t_interval < T_RANGE) 
						plane_1_path.t_interval += T_INTERVAL;
				}//end if
				else
				{
					if(plane_1_path.t_interval > 0) 
						plane_1_path.t_interval -= T_INTERVAL;
				}//end else
			}//end if
			else if(which_view == 2)
			{
				if(b)
				{
					if(plane_2_path.t_interval < T_RANGE) 
						plane_2_path.t_interval += T_INTERVAL;
				}//end if
				else
				{
					if(plane_2_path.t_interval > 0) 
						plane_2_path.t_interval -= T_INTERVAL;
				}//end else
			}//end else if
			break;
		case 2: //show points and tangents
			switch(which_view)
			{
				case 0: plane_1_path.show_pts_tans = !plane_1_path.show_pts_tans;
						plane_2_path.show_pts_tans = !plane_2_path.show_pts_tans;
					break;
				case 1: plane_1_path.show_pts_tans = !plane_1_path.show_pts_tans;
					break;
				case 2: plane_2_path.show_pts_tans = !plane_2_path.show_pts_tans;
					break;
			}//end switch
			break;
	}//end switch
}//end adjustPath

void adjustParticles(int option, bool b)
{
	if( which_particle == 0 )
	{
		switch(option)
		{
			case 0: if(b) engine.time_step++;
					else engine.time_step--;
				break;
			case 1: if(b)
					{
						engine.global_accel[0] += 0.01;
						engine.global_accel[1] += 0.01;
						engine.global_accel[2] += 0.01;
					}//end if
					else 
					{
						engine.global_accel[0] -= 0.01;
						engine.global_accel[1] -= 0.01;
						engine.global_accel[2] -= 0.01;
					}//end else
				break;
			case 2: if(b) engine.drag_coeff += 0.1;
					else engine.drag_coeff -= 0.1;
				break;
			case 3: if(b) engine.gen_rate++;
					else engine.gen_rate--;
				break;
			case 4: if(b)
					{
						engine.wind_vel[0] += 0.01;
						engine.wind_vel[1] += 0.01;
						engine.wind_vel[2] += 0.01;
					}//end if
					else 
					{
						engine.wind_vel[0] -= 0.01;
						engine.wind_vel[1] -= 0.01;
						engine.wind_vel[2] -= 0.01;
					}//end else
				break;
		}//end switch
	}//end if
	else if( which_particle == 1 )
	{
		switch(option)
		{
			case 0: if(b) missile.time_step++;
					else missile.time_step--;
				break;
			case 1: if(b)
					{
						missile.global_accel[0] += 0.01;
						missile.global_accel[1] += 0.01;
						missile.global_accel[2] += 0.01;
					}//end if
					else 
					{
						missile.global_accel[0] -= 0.01;
						missile.global_accel[1] -= 0.01;
						missile.global_accel[2] -= 0.01;
					}//end else
				break;
			case 2: if(b) missile.drag_coeff += 0.1;
					else missile.drag_coeff -= 0.1;
				break;
			case 3: if(b) missile.gen_rate++;
					else missile.gen_rate--;
				break;
			case 4: if(b)
					{
						missile.wind_vel[0] += 0.01;
						missile.wind_vel[1] += 0.01;
						missile.wind_vel[2] += 0.01;
					}//end if
					else 
					{
						missile.wind_vel[0] -= 0.01;
						missile.wind_vel[1] -= 0.01;
						missile.wind_vel[2] -= 0.01;
					}//end else
				break;
		}//end switch
	}//end else if
}//end adjustParticles

//////////////////////////////Function - keyboardhandler///////////////////////////////
//Updates settings based on user input.												 //
//					     Subpart:     Camera:     Light:							 //
// Yaw(+/-):		       f/h														 //
// Pitch(+/-):		       t/g														 //
// Distance(+/-):					    u/o											 //
// Azimuth(+/-):					    j/l											 //
// Elevation(+/-):						i/k											 //
// Trans-x(+/-):									f/h								 //
// Trans-y(+/-):									t/g								 //
// Trans-z(+/-):                                    r/y								 //
//																					 //
// Extra Commands:																	 //
//	1 - switch between left/right eye			6 - show pts on/off					 //
//  2 - switch among the 3 camera views			8 - switch ambient light on/off	     //
//	3 - switch among two subparts on plane 1	9 - switch diffuse light on/off		 //
//		and the light							0 - switch specular light on/off	 //
//  4 - turn coordinate axes on or off    		Spacebar - return to initial settings//	
//  5 - switch among particle systems			Escape - exit						 //
//  Scale Factor (+/-): w/s															 //
//  Time step (+/-): e/d						t-interval (+/-): q/a				 //
//  Global acceleration: c/v					Drag coefficient (+/-): b/n			 //
//  Generation rate: m/,                        Avg init velocity (+/-): .//         //
///////////////////////////////////////////////////////////////////////////////////////
void keyboardHandler(unsigned char key, int x, int y)
{
	switch(key)
	{
    //camera distance (+/-)
		case 'u': adjustMatrix(dist_pos, 1);
			break;
		case 'o': if(camera_world.operator()(2,3) > 0) adjustMatrix(dist_neg, 1);
			break;
	//camera elevation (+/-)
		case 'i': adjustMatrix(elev_ccw, 0);
			break;
		case 'k': adjustMatrix(elev_cw, 0);
			break;
	//camera azimuth (+/-)
		case 'j': adjustMatrix(azim_ccw, 0);
			break;
		case 'l': adjustMatrix(azim_cw, 0);
			break;
	//subpart yaw (+/-) or light x-trans
		case 'f': if(which_subpart == 0) light_pos.operator()(0, 3) -= TRANS;
			      else if(which_view == 1) adjustMatrix(yaw_ccw, 2);
			break;
		case 'h': if(which_subpart == 0) light_pos.operator()(0, 3) += TRANS;
				  else if(which_view == 1) adjustMatrix(yaw_cw, 2);
			break;
	//subpart pitch (+/-) or light y-trans
		case 't': if(which_subpart == 0) light_pos.operator()(1, 3) += TRANS;
				  else if(which_view == 1) adjustMatrix(pitch_ccw, 3);
			break;
		case 'g': if(which_subpart == 0) light_pos.operator()(1, 3) -= TRANS;
				  else if(which_view == 1) adjustMatrix(pitch_cw, 3);
			break;
	//light z_trans, no need to redisplay if light is not selected
		case 'r': if(which_subpart == 0) light_pos.operator()(2, 3) -= TRANS;
			break;
		case 'y': if(which_subpart == 0) light_pos.operator()(2, 3) += TRANS;
			break;
	//scale factor
		case 'w': adjustPath(0, true);
			break;
		case 's': adjustPath(0, false);
			break;
	//t-interval
		case 'q': adjustPath(1, true);
			break;
		case 'a': adjustPath(1, false); 
			break;
	//time step for particles:
		case 'e': adjustParticles(0, true);
			break;
		case 'd': adjustParticles(0, false); 
			break;
	//global acceleration
		case 'c': adjustParticles(1, true); 
			break;
		case 'v': adjustParticles(1, false); 
			break;
	//drag coefficient
		case 'b': adjustParticles(2, true);
			break;
		case 'n': adjustParticles(2, false); 
			break;
	//generation rate
		case 'm': adjustParticles(3, true);
			break;
		case ',': adjustParticles(3, false); 
			break;
	//average initial velocity
		case '.': adjustParticles(4, true);
			break;
		case '/': adjustParticles(4, false); 
			break;
	//number keys - switch among various options
		case '1': draw_left_eye = !draw_left_eye;
			break;
		case '2': which_view = (which_view + 1) % 3;
			break; 
		case '3': which_subpart = (which_subpart + 1) % 3;
			break;
		case '4': show_axes = !show_axes;
			break;
		case '5': which_particle = (which_particle + 1) % 2;
			break;
		case '6': adjustPath(2, true);
			break;
		case '8': show_ambient = !show_ambient;
			break;
		case '9': show_diffuse = !show_diffuse;
			break;
		case '0': show_specular = !show_specular;
			break;
	//Escape - exit	
		case 27: exit(0);
			break;
	//spacebar - return to initial position and settings
		case 32: reset();
			break;
	}//end switch

	glutPostRedisplay();
}//end keyboardhandler

////////////////////////////Function - idle////////////////////////////
//Calls display if no user input is provided						 //
///////////////////////////////////////////////////////////////////////
void idle()
{
	glutPostRedisplay();
}//end idle

int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	
	#ifdef USE_DEVELOPMENT_MODE     // Enable quad-buffered stereo rendering only when running on the 3d display
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	#else
		glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_STEREO);    
	#endif
	
	glutInitWindowSize(W_WIDTH, W_HEIGHT);
	glutCreateWindow("Assignment 6");
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboardHandler);
	glutIdleFunc(idle);
	
	#ifndef USE_DEVELOPMENT_MODE    // Enable full screen rendering only when running on the 3d display
		glutFullScreen();
	#endif
	
	init();
	initTextures();
	reset();
	glutMainLoop();
	return 0;
}//end Main