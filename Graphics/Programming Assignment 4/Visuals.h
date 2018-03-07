class Visuals
{
public:
	//window size constants
	#define W_WIDTH  1300
	#define W_HEIGHT 700

	//transformation constants
	#define X_TRANS 0.2
	#define YAW_DEG 10.0
	#define PITCH_DEG 10.0
	#define ROLL_DEG 10.0
	
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

	//draw axes
	void drawLeftWorldAxes();
	void drawRightWorldAxes();
	void drawPlaneLocalAxes();
	void drawCameraLocalAxes();
	void drawSmallAxes();
};