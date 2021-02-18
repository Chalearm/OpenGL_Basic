/**
	Date : 2021 Febuary 17
	https://cs.lmu.edu/~ray/notes/openglexamples/

	Introductory program; just a static picture of a colored triangle.
	Shows how to use GLUT.
	Has minimal structure: only main() and a display callback.
	Uses only the default viewing parameters (in fact, it never mentions viewing at all). This is an orthographic view volume with bounds of -1..1 in all three dimensions.
	Draws only using glColor and glVertex within glBegin and glEnd in the display callback.
	Uses only the GL_POLYGON drawing mode.
	Illustrates glClear and glFlush.

	How to build :

	MAC OS :
	gcc glTriangle.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glTriangle

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

// Clears the current window and draws a triangle.
void display()
{

	// Set every pixel in the frame buffer to the current clear color.
	glClear(GL_COLOR_BUFFER_BIT);

	// Put drawing code here 
	// Drawing is done by specifying a sequence of vertices. The way these
	// Vertices are connected (or not connected) depends on the argument to
	// glBegin. GL_POLYGON constructs a filled polygon.
	glBegin(GL_POLYGON);
	glColor3f(1,0,0);glVertex3f(-0.6,-0.75,0.5);
	glColor3f(0,1,0);glVertex3f(0.6,-0.75,0);
	glColor3f(0,0,1);glVertex3f(0,0.75,0);
	glEnd();

	// Flush drawing command buffer to make drawing happen as soon as possible.
	glFlush();
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
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.0,1.0,-1.0,1.0,-1.0,1.0);
	glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);

	// Position window at (80,80)-(480,380) and give it a title.
	glutInitWindowPosition(300,80);
	glutInitWindowSize(400,400);
	glutCreateWindow("A Simple Triangle");

	init();

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}