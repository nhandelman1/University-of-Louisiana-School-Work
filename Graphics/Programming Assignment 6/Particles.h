#include <math.h>
#include <stdlib.h>
#include <gmtl/gmtl.h>

class Particles
{
public:
	#define MAX_PARTICLES 1000
	
	struct Particle_Info
	{
		gmtl::Point3f pos;
		gmtl::Vec3f vel, color;
		float mass;
		float radius;
		int lifetime;
	};

	Particle_Info particles[MAX_PARTICLES];
	int num_parts_avg, num_parts_dev; //number new particels avg/dev
	gmtl::Point3f pos_avg, pos_dev; //position avg/dev
	gmtl::Vec3f vel_avg, vel_dev; //velocity avg/dev
	float mass_avg, mass_dev; //mass avg/dev
	float radius_avg, radius_dev; //radius avg/dev
	int lifetime_avg, lifetime_dev; //lifetime avg/dev
	gmtl::Point3f color_avg, color_dev; //color avg/dev
	int time_step, gen_rate; //time_step (delta) and generation rate
	gmtl::Vec3f wind_vel, global_accel; //misc variables
	float drag_coeff;

	Particles();
	Particles(int la, int ld, gmtl::Point3f ca, gmtl::Point3f cd, float ma, float md, 
	int na, int nd, float ra, float rd, gmtl::Vec3f pa, gmtl::Vec3f pd, gmtl::Vec3f va, gmtl::Vec3f vd);
	gmtl::Vec3f compAccel(int i);
	void update();
	void drawParticles();
	float rnd();
	float gaussian(float a, float d);

};//end class Particles