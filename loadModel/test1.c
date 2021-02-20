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

#include "loadModel.h"
struct modelLoader aModelLoader;
float g_rotation = 0;
float g_rotation2 = 0;
float g_xDistance = 0;
float g_yDistance = 0;

// Clears the window and draw the torus.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// Put drawing code here 

	// Draw a white torus of outer radius 3, inner radius 0.5 with 15 stacks
	// and 30 slices.


	glPushMatrix();
		glRotatef(g_rotation,0,0.0,g_rotation2);
		glTranslatef(g_xDistance,0.0,0.0);
		//glRotatef(90,0,1,0);
	drawModel(&aModelLoader);

	glPopMatrix();
	glFlush();

	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));

	glutSwapBuffers();
}
// Advances the orbiter and requests to draw the next frame.
void timer(int v)
{
	g_rotation = (float)((((int)g_rotation)+10)%360);
	g_rotation2 = (float)((((int)g_rotation2)+1)%360);
	g_xDistance = 2*cos(g_rotation*3.14);
	g_yDistance = 2*sin(g_rotation*3.14);
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

void init()
{
	// Set the current clear color to black and the current drawing color to 
	// white
	glClearColor(0.0,0.0,0.0,1.0);
	glColor3f(1.0,0.5,1.0);

	GLfloat black[] = {0.0,0.0,0.0,1.0};
	GLfloat yellow[] = {1.0,1.0,0.0,1.0};
	GLfloat cyan[] = {0.0,1.0,1.0,1.0};
	GLfloat white[] = {1.0,1.0,1.0,1.0};
	GLfloat direction[] = {1.0,1.0,1.0,0.0};

	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,cyan);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf(GL_FRONT,GL_SHININESS,30);

	glLightfv(GL_LIGHT0,GL_AMBIENT,black);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,yellow);
	glLightfv(GL_LIGHT0,GL_SPECULAR,white);
	glLightfv(GL_LIGHT0,GL_POSITION,direction);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
   // glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
	glEnable(GL_DEPTH_TEST);

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
	gluLookAt(4,2,2,0,0,0,0,1,0);
}
// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char **argv)
{
		constructorModelLoader(&aModelLoader,"brickCyclin.obj","brickCyclin.mtl");
	loadObj(&aModelLoader);
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,600);
	glutCreateWindow("A Simple Torus");	
	glutTimerFunc(100,timer,0);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}