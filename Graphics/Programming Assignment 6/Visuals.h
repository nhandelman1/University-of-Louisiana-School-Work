#include <gmtl/gmtl.h>

class Visuals
{
public:
	//window size constants
	#define W_WIDTH  800
	#define W_HEIGHT 800

	//transformation constants
	#define TRANS 0.2
	#define ELEV_DEG 10.0
	#define AZIM_DEG 10.0
	#define YAW_DEG 10.0
	#define PITCH_DEG 10.0
	#define ROLL_DEG 10.0

	//maximum number of cubic segments
	#define MAX_SEGMENTS 20

	//t-interval
	#define T_INTERVAL 1
	#define T_RANGE 100
	
	//axis min/max constants
	#define X_MIN -4.0
	#define X_MAX 4.0
	#define Y_MIN -4.0
	#define Y_MAX 4.0
	#define Z_MIN -4.0
	#define Z_MAX 4.0
	
	//draw objects
	void drawPlane();
	void drawGun();
	void drawBarrel();
	void drawSatellite();
	void drawCamera();
	void drawLight();
	void drawCube();
	void drawNearFace();
	void drawFarFace();
	void drawLeftFace();
	void drawRightFace();
	void drawTopFace();
	void drawBottomFace();
	void drawSpherePts(float radius, float c[3]);

	//draw axes
	void drawLeftWorldAxes();
	void drawRightWorldAxes();
	void drawPlaneLocalAxes();
	void drawCameraLocalAxes();
	void drawSmallAxes();
	void drawTangents(gmtl::Vec3f v, float c[3]);
};//end class visuals