/**
	Date : 2021 Febuary 17
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/

	This is OpenGL Torus.
	- Display a static picture of a torus and the coordinate system axes.
    - Instead of using a primitive GL function (such as glOrtho or glFrustum) to set the viewing volume, it instead uses gluPerspective. gluPerspective must be done when the matrix mode is GL_PROJECTION.
    - Instead of using translates and rotates to position the objects into the view volume, it instead uses gluLookAt to position and orient the camera so that it is looking at the objects. gluLookAt must be done when the matrix mode is GL_MODELVIEW.
    - Shows that the combination of gluPerspective and gluLookAt is considerably more intuitive than the more primitive mechanism of glFrustum and plain object transformations.
    - Draws the torus using the fancy glutWireTorus function (which internally calls glVertex a zillion times).

	How to build :

	MAC OS :
	gcc glSkeleton.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glSkeleton

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

// Clears the window and draw the torus.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// Put drawing code here 

	// Draw a white torus of outer radius 3, inner radius 0.5 with 15 stacks
	// and 30 slices.
	glColor3d(1.0,1.0,1.0);
	glutWireTorus(0.5,3,15,30);

	// Draw a red x-axis, a green y-axis, and a blue z-axis. Each of the 
	// axes are ten units long.
	glBegin(GL_LINES);
	glColor3f(1,0,0);glVertex3f(0,0,0);glVertex3f(10,0,0);
	glColor3f(0,1,0);glVertex3f(0,0,0);glVertex3f(0,10,0);
	glColor3f(0,0,1);glVertex3f(0,0,0);glVertex3f(0,0,10);
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
	// Set the current clear color to black and the current drawing color to 
	// white
	glClearColor(0.0,0.0,0.0,1.0);
	glColor3f(1.0,0.5,1.0);

	// Set the camera lens to have a 60 degree (vertical) field of view, an
	// aspect ratio of 4/3, and have everything closer than 1 unit to the
	// camera and greater than 40 units distant clipped away.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0,4.0/3.0,1,40);

	// Position camera at (4, 6, 5) looking at (0, 0, 0) with the vector
	// <0, 1, 0> pointing upward.
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(4,6,5,0,0,0,0,1,0);
}
// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,600);
	glutCreateWindow("A Simple Torus");

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}