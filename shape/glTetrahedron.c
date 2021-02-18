/**
	Date : 2021 Febuary 17
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/

	- Displays a static picture of a tetrahedron sitting on a grid "floor".
	- Sets up a viewing volume at the beginning of the program. Uses glFrustum in order to make a perspective projection.
	- Shows that you have to define the viewing volume when the matrix mode is GL_PROJECTION.
	- Draws the object in the display callback using GL_LINES and GL_TRIANGLE_STRIP modes.
	- Defines the object at the origin then moves it INTO the view volume.
	- Shows that the rotations and translations that move the object into the view volume must be performed when the matrix mode is GL_MODELVIEW.
	- Shows that if you are not doing animation it is best to define the transformations that position and orient the object should be done only once, and not inside the display callback.
	- Shows that transformations are cumulative.
	- Shows that before setting up view volumes or object transformations, you usually want to do a glLoadIdentity or else transforamtions from previous manipulations will accumulate.
	- Does backface culling as a very cheap way to do hidden surface removal. This does not work in all cases.

	How to build :

	MAC OS :
	gcc glTetrahedron.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glTetrahedron

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

void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	// Put drawing code here 

	// Draw a white grid "floor" for the tetrahedron to sit on.
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_LINES);
	for (GLfloat i = -2.5; i <= 2.5;i +=0.25)
	{
		glVertex3f(i,0,2.5);glVertex3f(i,0,-2.5);
		glVertex3f(2.5,0,i);glVertex3f(-2.5,0,i);
	}
	glEnd();

	// Draw the tetrahedron. It is a four sided figure, so when defining it
	// with a triangle strip we have to repeat the last two vertices.
	glBegin(GL_TRIANGLE_STRIP);
	glColor3f(1,1,1);glVertex3f(0,2,0);
	glColor3f(1,0,0);glVertex3f(-1,0,1);
	glColor3f(0,1,0);glVertex3f(1,0,1);
	glColor3f(0,0,1);glVertex3f(0,0,-1.4);
	glColor3f(1,1,1);glVertex3f(0,2,0);
	glColor3f(1,0,0);glVertex3f(-1,0,1);
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

// Sets up global attributes like clear color and drawing color, enables and
// initializes any needed modes (in this case we want backfaces culled), and
// sets up the desired projection and modelview matrices. It is cleaner to
// define these operations in a funciton separate from main().
void init()
{
	// Set the current clear color to sky blue and the current drawing color to
	// white
	glClearColor(0.1,0.39,0.88,1.0);
	glColor3f(1.0,1.0,1.0);

	// Tell the rendering engine not to draw backfaces. Without this code,
	// all four faces of the tetrahedron would be drawn and it is possible
	// that faces farther away could be drawn after nearer to the viewer.
	// Since there is only one closed polyhedron in the whole scene,
	// eliminating the drawing of backfaces gives us the realism we need.
	// ** This does not work in general.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Set the camera lens so that we have a perspective vieweing volume whose 
	// horizontal bounds at the near clipping plane are -2..2 and vertical
	// bound are -1.5..1.5. The near clipping plane is 1 unit from the camera
	// and the far clipping plane is 40 units away.

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-2,2,-1.5,1.5,1,40);
	// Set up transforms so that the tetrahedron which is defined right at
	// the origin will be rotated and moved into the view volume. First we
	// rotate 70 degrees around y so we can see a lot of the left side.
	// Then we rotate 50 degrees around x to "drop" the top of the pyramid
	// down a bit. Then we move the object back 3 units "into the screen".
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0,0,-3);
	glRotatef(50,1,0,0);
	glRotatef(70,0,1,0);
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutCreateWindow("Tetrahedron");
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,600);
	glutDisplayFunc(display);
	init();
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}