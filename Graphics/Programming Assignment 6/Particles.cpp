#include "Particles.h"
#include <gl/glut.h>

Particles::Particles()
{}

Particles::Particles(int la, int ld, gmtl::Point3f ca, gmtl::Point3f cd, float ma, float md,  int na, 
 int nd, float ra, float rd, gmtl::Vec3f pa, gmtl::Vec3f pd, gmtl::Vec3f va, gmtl::Vec3f vd)
{
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		particles[i].lifetime = 0;
	}//end for
	
	//  averages			deviations
	lifetime_avg = la;	lifetime_dev = ld;
	color_avg = ca;		color_dev = cd;
	mass_avg = ma;		mass_dev = md;
	num_parts_avg = na; num_parts_dev = nd;
	radius_avg = ra;	radius_dev = rd;
	pos_avg = pa;		pos_dev = pd;
	vel_avg = va;		vel_dev = vd;
}//end Particles

gmtl::Vec3f Particles::compAccel(int i)
{
	gmtl::Vec3f accel;

	for(int j = 0; j < 3; j++)
	{  //apply f = (mg - k(v - vwind)) and a = f/m
		accel[j] = particles[i].mass*global_accel[j];
		accel[j] -= drag_coeff*(particles[i].vel[j] - wind_vel[j]);
		accel[j] /= particles[i].mass;
	}//end for

	return accel;
}//end compAccel

void Particles::update()
{
	int num_new_parts;
	gmtl::Vec3f accel_vector, temp;

	//calculate number of new particles for this frame
	num_new_parts = (int)gaussian((float)num_parts_avg, (float)num_parts_dev);

	for(int i = 0; i < MAX_PARTICLES; i++)//run through array of particles
	{
		if(particles[i].lifetime == 0)//if particle is "dead"
		{
			if(num_new_parts > 0) //create new particle if a new one is needed
			{
				particles[i].mass = gaussian(mass_avg, mass_dev);
				particles[i].lifetime = (int)gaussian((float)lifetime_avg, (float)lifetime_dev);
				particles[i].radius = gaussian(radius_avg, radius_dev); 
				particles[i].color[0] = gaussian(color_avg[0], color_dev[0]);
				particles[i].color[1] = gaussian(color_avg[1], color_dev[1]);
				particles[i].color[2] = gaussian(color_avg[2], color_dev[2]);
				particles[i].pos[0] = gaussian(pos_avg[0], pos_dev[0]);
				particles[i].pos[1] = gaussian(pos_avg[1], pos_dev[1]);
				particles[i].pos[2] = gaussian(pos_avg[2], pos_dev[2]);
				particles[i].vel[0] = gaussian(vel_avg[0], vel_dev[0]);
				particles[i].vel[1] = gaussian(vel_avg[1], vel_dev[1]);
				particles[i].vel[2] = gaussian(vel_avg[2], vel_dev[2]);
				
				for(int j = 0; j < 3; j++)//ensure color is within 0.0 and 1.0
				{
					if( particles[i].color[j] > 1.0 ) particles[i].color[j] = 1.0;
					else if( particles[i].color[j] < 0.0 ) particles[i].color[j] = 0.0;
				}//end for

				num_new_parts--;
			}//end if
		}//end if
		else //update current particle
		{
			temp = particles[i].vel; //update pos with time_step * velocity
			temp[0] *= time_step;
			temp[1] *= time_step;
			temp[2] *= time_step;
			particles[i].pos += temp;
			accel_vector = compAccel(i); //update velocity
			particles[i].vel += accel_vector;
			particles[i].lifetime--; //update lifetime
		}//end else
	}//end for
}//end update

void Particles::drawParticles()
{
	gmtl::Matrix44f modelview;
	gmtl::Vec3f view_x, view_y;
	gmtl::Point3f top_left, bottom_left, top_right, bottom_right;
	gmtl::Point3f bb_top_left, bb_bottom_left, bb_top_right, bb_bottom_right;
	glGetFloatv(GL_MODELVIEW_MATRIX, modelview.mData);

	//extract billboard corners from modelview and assign them to points
	for(int i = 0; i < 3; i++)
	{
		view_x[i] = modelview.operator()(0,i);
		view_y[i] = modelview.operator()(1,i);
	}//end for

	gmtl::normalize(view_x); gmtl::normalize(view_y);

	top_left = -view_x + view_y;
	bottom_left = -view_x -view_y;
	top_right = view_x + view_y;
	bottom_right = view_x - view_y;

	//draw the particles
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		if(particles[i].lifetime != 0)
		{   //assign billboard corners
			bb_top_left = particles[i].pos + (top_left * particles[i].radius);
			bb_bottom_left = particles[i].pos + (bottom_left * particles[i].radius);
			bb_top_right = particles[i].pos + (top_right * particles[i].radius);
			bb_bottom_right = particles[i].pos + (bottom_right * particles[i].radius);

			//draw the billboard with appropriate color
			glColor3f(particles[i].color[0], particles[i].color[1], particles[i].color[2]);
			glBegin(GL_QUADS);
				glVertex3f(bb_top_left[0], bb_top_left[1], bb_top_left[2]);
				glVertex3f(bb_bottom_left[0], bb_bottom_left[1], bb_bottom_left[2]);
				glVertex3f(bb_bottom_right[0], bb_bottom_right[1], bb_bottom_right[2]);
				glVertex3f(bb_top_right[0], bb_top_right[1], bb_top_right[2]);
			glEnd();
		}//end if
	}//end for
}//end drawParticles

float Particles::rnd()
{
	return rand()/(float)RAND_MAX;
}//end rnd

//gaussian random number as provided on moodle (adjusted to use floats)
float Particles::gaussian(float a, float d)
{
	static float t = 0.0;
	float x,v1,v2,r;

	if(t==0)
	{
		do{
			v1 = 2.0 * rnd() - 1.0;
			v2 = 2.0 * rnd() - 1.0;
			r = v1 * v1 + v2 * v2;
		}while(r >= 1.0);
		r = sqrt((-2.0 * log(r))/r);
		t = v2 * r;
		return (a + v1 * r * d);
	}//end if
	else
	{
		x = t;
		t = 0.0;
		return (a + x * d);
	}//end else
}//end gaussian
