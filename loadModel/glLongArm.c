/**
	Date : 2021 Febuary 24
	This is from http://titan.csit.rmit.edu.au/~e20068/teaching/i3dg&a/2020/tute-1.html

	This is to show the robot arm part objects loaded from Obj file and mtl file.

	This for make experiment for loading OBJ, texture, matrix tranformation and camera setting.

	How to build :

	MAC OS :
	gcc glLongArm.c loadModel.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glLongArm

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
#define __gl_h_
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "loadModel.h"


struct modelLoader aModelLoader;
struct modelLoader aJointPart;
struct modelLoader aFinger;
float g_rotation = 0;
float g_rotation2 = 0;
float g_xDistance = 6;
float g_yDistance = 6;
float g_fingersEnlarge = 0;

float kk1 = 0;
float kk2 = 0;
float kk3 = 0;
float kk4 = 0;
float kk5=0;
float kk6=0;

int signVal = 1;

// Clears the window and draw the torus.
void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Put drawing code here 

	// Draw a white torus of outer radius 3, inner radius 0.5 with 15 stacks
	// and 30 slices.


	glPushMatrix();

	glRotatef((GLfloat)g_rotation,0.0,1.0,0.0);
	glTranslatef(0.0,0.75,0.0);
	drawModel(&aModelLoader);

		glTranslatef(0.0,0.0,3.0);
	drawModel(&aJointPart);



		glTranslatef(0.0,0.0,1.0);
	glRotatef((GLfloat)g_rotation2,0.0,1.0,0.0);
	glTranslatef(0.0,0.0,2.0);
	drawModel(&aModelLoader);



		glTranslatef(0.0,0.0,3.0);
	drawModel(&aJointPart);


		glTranslatef(0.0,0.0,1.0);
	glRotatef((GLfloat)g_rotation2,0.0,1.0,0.0);
	glTranslatef(0.0,0.0,2.0);
	drawModel(&aModelLoader);

			glTranslatef(0.0,0.0,3.0);
	drawModel(&aJointPart);


		glTranslatef(0.0,0.0,1.0);
	glRotatef((GLfloat)g_rotation2,0.0,1.0,0.0);
	glTranslatef(0.0,0.0,2.0);
	drawModel(&aModelLoader);


			glTranslatef(0.0,0.0,3.0);
	drawModel(&aJointPart);


	glPushMatrix();
	glTranslatef(-0.3-kk4,1.9-g_fingersEnlarge,2.0-kk6);
	//glRotatef((GLfloat)kk1,0.0,1.0,0.0);
	//glRotatef((GLfloat)kk2,0.0,0.0,1.0);
	glRotatef((GLfloat)-185,0.0,1.0,1.0);

	//glRotatef((GLfloat)kk2,kk4,kk5,kk6);
	drawModel(&aFinger);
	glPopMatrix();

	glTranslatef(-0.3+kk4,-2.0+g_fingersEnlarge,2.0+kk6);
	glRotatef((GLfloat)90,1.0,0.0,0.0);
	glRotatef((GLfloat)5.0,0.0,0.0,1.0);

	//glRotatef((GLfloat)kk1,kk4,kk5,kk6);
	drawModel(&aFinger);


//	renderModel(&aModelLoader);
	glPopMatrix();



	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(g_xDistance,g_yDistance,7,0,0,0,0,1,0);

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
	//glutPostRedisplay();
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
		case 'n':
			g_rotation = (float)(((int)g_rotation+signVal*5)%360);
		break;
		case 'm':
			g_rotation2 = (float)(((int)g_rotation2+signVal*5)%360);
		break;		
		case 'h':
			g_xDistance = (float)(((int)g_xDistance+signVal)%100);
		break;
		case 'j':
			g_yDistance = (float)(((int)g_yDistance+signVal)%100);
		break;
		case 'b':
			g_fingersEnlarge = g_fingersEnlarge+0.1*signVal;
			if(g_fingersEnlarge> 1.2)g_fingersEnlarge=1.2;
			if(g_fingersEnlarge< 0.0)g_fingersEnlarge=0.0;
		break;
		case 'f':
			kk1 = (float)(((int)kk1+5*signVal)%360);
		break;
		case 'g':
			kk2 = (float)(((int)kk2+5*signVal)%360);
		break;
		case 'd':
			kk3 = (float)(((int)kk3+5*signVal)%360);
		break;
		case 't':
			kk4 = kk4+0.1*signVal;
		break;
		case 'y':
			kk5 = kk5+0.1*signVal;
		break;
		case 'u':
			kk6 = kk6+0.1*signVal;
		break;
		case '-':
			signVal = -1;
		break;
		case '+':
			signVal = 1;
		default:
		 break;
	}
	printf("kk(%f,%f,%f, %f, %f,%f)\n",kk1,kk2,kk3,kk4,kk5,kk6);
	glutPostRedisplay();
}
void reshape(GLint w, GLint h)
{

	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, ((GLfloat)w)/((GLfloat)h),1.0,150.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	loadTexture(&aModelLoader);
	loadTexture(&aJointPart);
	loadTexture(&aFinger);
	
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

	glEnable(GL_DEPTH_TEST);
	glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,white);
	glMaterialfv(GL_FRONT,GL_SPECULAR,white);
	glMaterialf(GL_FRONT,GL_SHININESS,30);

	//glLightfv(GL_LIGHT0,GL_AMBIENT,black);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,yellow);
	glLightfv(GL_LIGHT0,GL_SPECULAR,white);
	glLightfv(GL_LIGHT0,GL_POSITION,direction);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
   // glEnable( GL_COLOR_MATERIAL );
  //  glShadeModel( GL_SMOOTH );
   // glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );

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
	//loadObjToVBuffs(&aModelLoader);
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char **argv)
{
	constructorModelLoader(&aModelLoader,"wavefrontOBJ/robotArm1.obj","texture/robottx1.jpg","wavefrontOBJ/robotArm1.mtl");
	constructorModelLoader(&aJointPart,"wavefrontOBJ/jointPoint.obj","texture/gearTx1.jpg","wavefrontOBJ/jointPoint.mtl");
	constructorModelLoader(&aFinger,"wavefrontOBJ/afinger.obj","texture/robottx1.jpg","wavefrontOBJ/afinger.mtl");
	loadObj(&aModelLoader);
	loadObj(&aJointPart);
	loadObj(&aFinger);

	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,600);
	//glutSpecialFunc(special);
	glutCreateWindow("A Brick monkey");	
	glutTimerFunc(100,timer,0);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}