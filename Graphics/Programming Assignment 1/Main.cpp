//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Nick Handelman																								//
//CMPS 515																										//
//																												//
//Programming Assignment 1																						//
//																												//
//This program implements triangle scan conversion to a simulated frame buffer and smoothly shades triangles	//
//from three different vertex colors. Vertices are specified by mouse vertices.									//
//																												//
//																												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////Libraries, Constants, Global Variables////////////////////////////////////////
#include <stdio.h>
#include <GL/glut.h>
#include <math.h>
#define WIDTH 700		// width of window (also frame buffer's width)
#define HEIGHT 500		// height of window (also frame buffer's height)
static GLubyte frame_buffer[HEIGHT][WIDTH][3];
static int vertices[3][2];
const int r0 = 255, r12 = 0, g1 = 255, g02 = 0, b2 = 255, b01 = 0;
float red_bottom = r0, red_top = r0, green_bottom = g1, green_top = g1, blue_bottom = b2, blue_top = b2;
float rb_incr = 0, rt_incr = 0, gb_incr = 0, gt_incr = 0, bb_incr = 0, bt_incr = 0;

////////////////////////////////////////////////Function - Display////////////////////////////////////////////////
//																												//
//Called by GLUT when a display event occurs.																	//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void display(void) 
{
	/*
		Set the raster position to the lower-left corner to avoid a problem 
		(with glDrawPixels) when the window is resized to smaller dimensions.
	*/
	glRasterPos2i(-1,-1);

	// Write the information stored in "frame_buffer" to the color buffer
	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, frame_buffer);
	glFlush();
}//end display


//////////////////////////////////////////////Function - orderpoints//////////////////////////////////////////////
//																												//
//Order points by increasing x-value.																			//
//If two or more points have the same x-value, arrange those points by increasing y-value.						//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void orderpoints()
{
	int temp[1][2];

	//arrange points in order of increasing x-value
	for(int r = 0; r < 2; r++)
	{
		for(int c = 0; c < 2; c++)
		{
			if( vertices[c][0] > vertices[c+1][0] )
			{
				temp[0][0] = vertices[c][0];
				temp[0][1] = vertices[c][1];
				vertices[c][0] = vertices[c+1][0];
				vertices[c][1] = vertices[c+1][1];
				vertices[c+1][0] = temp[0][0];
				vertices[c+1][1] = temp[0][1];
			}//if
		}//for
	}//for

	//if two or more vertices have the same x-value, arrange only those vertices by increasing y-value
	if( vertices[0][0] == vertices[2][0] ) //all three points have same x-value
	{
		for(int r = 0; r < 2; r++)
		{
			for(int c = 0; c < 2; c++)
			{
				if( vertices[c][1] > vertices[c+1][1] )
				{
					temp[0][0] = vertices[c][0];
					temp[0][1] = vertices[c][1];
					vertices[c][0] = vertices[c+1][0];
					vertices[c][1] = vertices[c+1][1];
					vertices[c+1][0] = temp[0][0];
					vertices[c+1][1] = temp[0][1];
				}//if
			}//for
		}//for
	}//if
	else if( vertices[0][0] == vertices[1][0] )	// points 0 and 1 have same x-value
	{
		if( vertices[0][1] > vertices[1][1] ) //if point 0 has a greater y-value, swap it with point 1
		{
			temp[0][0] = vertices[0][0]; 
			temp[0][1] = vertices[0][1];
			vertices[0][0] = vertices[1][0];
			vertices[0][1] = vertices[1][1];
			vertices[1][0] = temp[0][0];
			vertices[1][1] = temp[0][1];
		}//if
	}//
	else if( vertices[1][0] == vertices[2][0] ) //points 1 and 2 have same x-value
	{
		if( vertices[1][1] > vertices[2][1] ) //if point 1 has a greater y-value, swap it with point 2
		{
			temp[0][0] = vertices[1][0]; 
			temp[0][1] = vertices[1][1];
			vertices[1][0] = vertices[2][0];
			vertices[1][1] = vertices[2][1];
			vertices[2][0] = temp[0][0];
			vertices[2][1] = temp[0][1];
		}//if
	}//else if
}//end orderpoints

////////////////////////////////////////////Function - colorframebuffer///////////////////////////////////////////
//																												//
//This function sets the RGB colors of the frame buffer and handles both left and right triangles.				//
//It receives beginning and end x-coordinate, the top y and bottom y and the top slope and bottom slope.		//
//It increments color bottom and top values.																	//
//It increments the color values within a column.																//
//																												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void colorframebuffer( int x_begin, int x_end, float *y_bottom, float *y_top, float slope_bottom, float slope_top)
{
		for(int x = x_begin; x < x_end; x++)
		{
			//delta R values
			float red_change = float(red_top - red_bottom)/(*y_top - *y_bottom);
			float green_change = float(green_top - green_bottom)/(*y_top - *y_bottom);
			float blue_change = float(blue_top - blue_bottom)/(*y_top - *y_bottom);

			//color values for input into the frame_buffer
			float red = red_bottom + (ceil(*y_bottom) - *y_bottom) * red_change;
			float green = green_bottom + (ceil(*y_bottom) - *y_bottom) * green_change;
			float blue = blue_bottom + (ceil(*y_bottom) - *y_bottom) * blue_change;

			//color column ceiling(y_bottom) through floor(y_top)
			for(int y = ceil(*y_bottom); y <= floor(*y_top); y++)
			{
				frame_buffer[HEIGHT-y][x][0] = floor(red + 0.5);
				frame_buffer[HEIGHT-y][x][1] = floor(green + 0.5);
				frame_buffer[HEIGHT-y][x][2] = floor(blue + 0.5);

				//increment color values by delta r
				red += red_change;
				green += green_change;
				blue += blue_change;
			}//for

			red_bottom += rb_incr;
			green_bottom += gb_incr;
			blue_bottom += bb_incr;

			red_top += rt_incr;
			green_top += gt_incr;
			blue_top += bt_incr;
			
			*y_bottom += slope_bottom;
			*y_top += slope_top;
		}//for
}//end colorframebuffer

/////////////////////////////////////////////Function - setframebuffer////////////////////////////////////////////
//																												//
//This function decides the triangle type based on the points.													//
//It initializes starting values and sends them to function "colorframebuffer".									//
//																												//
//slope_01 - slope between point 0 and point 1																	//
//slope_02 - slope between point 0 and point 2																	//
//slope_12 - slope between point 1 and point 2																	//
//																												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setframebuffer()
{
	float slope_01, slope_02, slope_12, yb, yt, y_mid;

	if(vertices[0][0] != vertices[2][0]) //atleast two points have different x-values
	{
		yb = float(vertices[0][1]);
		yt = float(vertices[0][1]);

		slope_02 = float(vertices[2][1] - vertices[0][1])/float(vertices[2][0] - vertices[0][0]); //slope between point 0 and point 2
		
		if(vertices[0][0] == vertices[1][0]) //right half only
		{
			slope_12 = float(vertices[2][1] - vertices[1][1])/float(vertices[2][0] - vertices[1][0]); //slope between point 1 and point 2
			yt = float(vertices[1][1]);
			red_bottom = r0; red_top = r12;
			green_bottom = g02; green_top = g1;
			blue_bottom = b01; blue_top = b01;

			rb_incr = float(r12 - r0)/float( vertices[2][0] - vertices[0][0] );
			gb_incr = 0;
			bb_incr = float(b2 - b01)/float( vertices[2][0] - vertices[0][0] );

			rt_incr = 0;
			gt_incr = float(g02 - g1)/float( vertices[2][0] - vertices[1][0] );
			bt_incr = float(b2 - b01)/float( vertices[2][0] - vertices[1][0] );

			colorframebuffer(vertices[0][0], vertices[2][0], &yb, &yt, slope_02, slope_12); 
		}//if
		else if(vertices[1][0] == vertices[2][0]) //left half only
		{	
			slope_01 = float(vertices[1][1] - vertices[0][1])/float(vertices[1][0] - vertices[0][0]); //slope between point 0 and point 1
			red_bottom = r0; red_top = r0;
			green_bottom = g02; green_top = g02;
			blue_bottom = b01; blue_top = b01;

			rb_incr = float(r12 - r0)/float( vertices[1][0] - vertices[0][0] );
			gb_incr = float(g1 - g02)/float( vertices[1][0] - vertices[0][0] );
			bb_incr = 0;

			rt_incr = float(r12 - r0)/float( vertices[2][0] - vertices[0][0] );
			gt_incr = 0;
			bt_incr = float(b2 - b01)/float( vertices[2][0] - vertices[0][0] );

			colorframebuffer(vertices[0][0], vertices[2][0], &yb, &yt, slope_01, slope_02); 
		}//else if
		else //all three points have different x-values, p1<p2<p3
		{
			slope_01 = float(vertices[1][1] - vertices[0][1])/float(vertices[1][0] - vertices[0][0]); //slope between point 0 and point 1
			slope_12 = float(vertices[2][1] - vertices[1][1])/float(vertices[2][0] - vertices[1][0]); //slope between point 1 and point 2
			red_bottom = red_top = r0;
			green_bottom = green_top = g02;
			blue_bottom = blue_top = b01;

			//y-value of point 0 is greater than y-value of point 1
			if(vertices[0][1] > vertices[1][1])
			{
				rt_incr = float(r12 - r0)/float( vertices[2][0] - vertices[0][0] );
				gt_incr = float(g1 - g02)/float( vertices[2][0] - vertices[0][0] );
				bt_incr = float(b2 - b01)/float( vertices[2][0] - vertices[0][0] );
				
				//left half
				rb_incr = float(r12 - r0)/float( vertices[1][0] - vertices[0][0] );
				gb_incr = float(g1 - g02)/float( vertices[1][0] - vertices[0][0] );
				bb_incr = 0;
				colorframebuffer(vertices[0][0], vertices[1][0], &yb, &yt, slope_01, slope_02); 

				//right half
				rb_incr = 0;
				gb_incr = float(g02 - g1)/float(vertices[2][0] - vertices[1][0] );
				bb_incr = float(b2 - b01)/float( vertices[2][0] - vertices[1][0] );
				colorframebuffer(vertices[1][0], vertices[2][0], &yb, &yt, slope_12, slope_02); 

			}//if
			else	//y-value of point 1 is greater than or equal to y-value of point 0
			{
				rb_incr = float(r12 - r0)/float( vertices[2][0] - vertices[0][0] );
				gb_incr = float(g1 - g02)/float( vertices[2][0] - vertices[0][0] );
				bb_incr = float(b2 - b01)/float( vertices[2][0] - vertices[0][0] );

				//left half
				rt_incr = float(r12 - r0)/float( vertices[1][0] - vertices[0][0] );
				gt_incr = float(g1 - g02)/float( vertices[1][0] - vertices[0][0] );
				bt_incr = 0;
				colorframebuffer(vertices[0][0], vertices[1][0], &yb, &yt, slope_02, slope_01);

				//right half
				rt_incr = 0;
				gt_incr = float(g02 - g1)/float( vertices[2][0] - vertices[1][0] );
				bt_incr = float(b2 - b01)/float( vertices[2][0] - vertices[1][0] );
				colorframebuffer(vertices[1][0], vertices[2][0], &yb, &yt, slope_02, slope_12); 
			}//else
		}//else
	}//if
	else
	{
		for(int y = vertices[0][1]; y <= vertices[2][1]; y++)
		{
				frame_buffer[HEIGHT-y][vertices[0][0]][0] = 255 - y;
				frame_buffer[HEIGHT-y][vertices[0][0]][1] = 127 + y/2;
				frame_buffer[HEIGHT-y][vertices[0][0]][1] = 127 - y/2;
				frame_buffer[HEIGHT-y][vertices[0][0]][2] = 0 + y;
		}
	}//else
}//end setframebuffer


////////////////////////////////////////////Function - Mousebuttonhandler/////////////////////////////////////////
//																												//
//Called when mouse button pressed																				//
//Assigns x and y values for mouse button clicks																//
//Calls function "setframebuffer" after three points have been stored											//
//Resets frame_buffer and clears screen to black with the first click for a shape								//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void mousebuttonhandler(int button, int state, int x, int y)
{
	static int click_count = 0;

	if( !state )
	{	
		printf("Mouse button event, button=%d, state=%d, x=%d, y=%d\n", button, state, x, y);

		vertices[click_count][0] = x;
		vertices[click_count][1] = y;     

		if(click_count == 2)
		{
			printf("Points before: (%d,%d) (%d,%d) (%d,%d)\n", vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1], vertices[2][0], vertices[2][1]); 
			orderpoints();
			printf("Points  after: (%d,%d) (%d,%d) (%d,%d)\n", vertices[0][0], vertices[0][1], vertices[1][0], vertices[1][1], vertices[2][0], vertices[2][1]); 

			setframebuffer(); 

			glutPostRedisplay(); // cause a display event to occur for GLUT

		}//end if

		click_count = (click_count + 1) % 3;

		//reset frame_buffer and clear screen to black
		if(click_count == 1)
		{
			for(int x = 0; x < WIDTH; x++)
			{
				for(int y = 0; y < HEIGHT; y++)
				{
					frame_buffer[y][x][0] = 0;
					frame_buffer[y][x][1] = 0;
					frame_buffer[y][x][2] = 0;
				}//for
			}//for
			glutPostRedisplay(); // cause a display event to occur for GLUT
		}//if
	}//end if
}//end mousebuttonhandler

													
////////////////////////////////////////////////Function - Main///////////////////////////////////////////////////
//																												//
// Initialize display and mouse functions and start glut loop													//
//																												//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Frame Buffer Example");

	// Specify which functions get called for display and mouse events:
	glutDisplayFunc(display);
    glutMouseFunc(mousebuttonhandler);

	glutMainLoop();

	return 1;
}//end main
