/**
	Date : 2021 Febuary 17
	This is from http://titan.csit.rmit.edu.au/~e20068/teaching/i3dg&a/2020/tute-1.html

	This is OpenGL Sierpinski2D source code.
	- Draws 100000 points of a Sierpinski Gasket.
    - Draws with GL_POINTS.
    - Illustrates poor programming practice: the display callback contains long-running code. We get away with it here because we only have to generate 100000 points. The program will appear to hang if you wanted to draw, say, twenty million points.
    - Ilustrates some C++, namely a struct with a member function.
    - Note that the glFlush call is made after all 100000 points have been drawn, not after each point.

	How to build :

	MAC OS :
	gcc glSierpinski2D.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glSierpinski2D

	Linux :


	Windows:


**/

#include <stdlib.h>
#include <stdio.h>

#if _WIN32
#include <Windows.h>
#endif
#if __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

// A simple two-dimensional point class to make life easy. It allows you to
// reference points with x and y coordinates instead of array indices and
// encapsulates a midpoint function;
struct Point
{
	GLfloat x,y;
};
struct Point midpoint(struct Point p1,struct Point p2){return (struct Point){(p1.x+p2.x)/2.0,(p1.y+p2.y)/2.0};}

// Draws a Sierpinski triangle with a fixed number of points. (Note that the
// number of points is kept fairly small because a display callback should 
// Never run for too long.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Put drawing code here 
	static struct Point vertices[3] = {{0,0},{200,500},{500,0}};

	// Compute and plot 100000 new points, starting (arbitrarily) with one of 
	// the vertices. Each point is halfway between the previous point and a 
	// randomly chesen vertex.
	static struct Point p = {0,0};
	int k = 0;
	glBegin(GL_POINTS);
	for (k = 0; k < 100000; k++)
	{
		p = midpoint(p,vertices[rand()%3]);
		glVertex2f(p.x,p.y);
	}
	glEnd();
	glFlush();
	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));

	glutSwapBuffers();
}

void keyboard(unsigned char key, int x,int y)
{
	switch(key)
	{
		case 27:
		case 'q':
		   exit(EXIT_SUCCESS);
		break;
		default:
		 break;
	}
}

void init()
{
	// Set a deep purple background and draw in a greenish yellow.
	glClearColor(0.25,0.0,0.2,1.0);
	glColor3f(0.6,1.0,0.0);
	// Set up the viewing volume : 500x500x1 window with origin lower left.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0,500.0,0.0,500.0,0.0,1.0);
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(40,40);
	glutCreateWindow("Sierpinski Triangle 2D");

	glutDisplayFunc(display);
	init();
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}