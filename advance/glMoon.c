/**
	Date : 2021 Febuary 19
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/
	
	Phases of the Moon

	How to build :

	MAC OS :
	gcc glMoon.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glMoon

	Linux :


	Windows:


**/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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
// A class for the moon. A moon is really just an Open GL display list. The
// display list is created with create(), and the draw() method calls it.
// The reason we don't create the display list in the constructor is that
// clients may want to declare moon objects before the call to initializaing
// OpenGL.
//
// The moon is a sphere of radius 1 centered at the origin, built from 25
// slices and 25 stacks, lit with GL_LIGHT0 as a directional light pointing
// along <1,1,1>
struct Moon
{
	int displayListId;
};

void createMoon(struct Moon *obj)
{
	obj->displayListId = glGenLists(1);
	glNewList(obj->displayListId,GL_COMPILE);
	GLfloat direction[] = {-1.0,-1.0,-1.0,0.0};
	glLightfv(GL_LIGHT0, GL_POSITION,direction);
	glutSolidSphere(1.0,25,25);
	glEndList();
}
void drawMoon(struct Moon *obj)
{
	glCallList(obj->displayListId);
}

// An orbiter is an object that flies on a circle of a certain radius on the
// xz plane. You supply the radius at construction time.
struct Orbiter
{
	double radius;
	double u;
};

void advanceOrbiter(struct Orbiter *obj, double delta)
{
	obj->u += delta;
}
void getPositionOrbiter(struct Orbiter *obj, double *x, double *y, double *z)
{
	*x = obj->radius*cos(obj->u);
	*y = 0;
	*z = obj->radius*sin(obj->u);
}

//The one and only moon.
static struct Moon moon;
// The one and only orbiter.
static struct Orbiter orbiter = {5.0,0};

// Enable depth testing, enables lighting for a bright yellowish diffuse
// light, and creates a mmon.
void init()
{
	glEnable(GL_DEPTH_TEST);
	GLfloat yellow[] = {1.0,1.0,0.5,1.0};
	glLightfv(GL_LIGHT0,GL_DIFFUSE,yellow);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	createMoon(&moon);
}

// Clears the window (and the depth buffer ) and draws the moon as viewed from
// the current position of the orbiter.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	double x,y,z;
	getPositionOrbiter(&orbiter,&x,&y,&z);
	gluLookAt(x,y,z,0.0,0.0,0.0,0.0,1.0,0.0);
	drawMoon(&moon);
	glPopMatrix();
	glutSwapBuffers();

	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));
}

// reshaped() fixes up the projection matrix so that we always see a sphere 
// instead of an ellipsoid.
void reshape(GLint w, GLint h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, ((GLfloat)w)/((GLfloat)h),1.0,10.0);
}

// Advances the orbiter and requests to draw the next frame.
void timer(int v)
{
	advanceOrbiter(&orbiter,0.01);
	glutPostRedisplay();
	glutTimerFunc(1000/60,timer,v);
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

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE|GLUT_DEPTH);
	glutCreateWindow("The Moon");
	glutInitWindowPosition(80,80);
	glutInitWindowSize(500,500);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(100,timer,0);
	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}