/**
	Date : 2021 Febuary 18
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/
	
	This is Robot arm 4 joints , glRobotarm4.c

	How to build :

	MAC OS :
	gcc glRobotarm4.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glRobotarm4

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


// The robot arm is specified by (1) the angle that the upper arm makes
// relative to the x-axis, called shoulderAngle, and (2) the angle that the 
// lower arm makes relative to the upper arm, called elbowAngle. These angles
// are adjusted in 5 degree increments by a keyboard callback.
static int shoulderAngle = 0, elbowAngle = 0, elbowAngle2 = 0, elbowAngle3 = 0, elbowAngle4 = 0;

// Handle the keyboard event: the left and right arrows bend the elbow, the
// up and down keys bend the shoulder.
void special(int key, int a,int b)
{
	switch(key)
	{

		case GLUT_KEY_LEFT: elbowAngle2 = (elbowAngle2 +5)%360; break;
		case GLUT_KEY_RIGHT: elbowAngle2 = (elbowAngle2 -5)%360; break;
		case GLUT_KEY_UP: shoulderAngle = (shoulderAngle +5)%360; break;
		case GLUT_KEY_DOWN: shoulderAngle = (shoulderAngle -5)%360; break;
		default: return;
	}
	glutPostRedisplay();
}

// wireBox(w, h, d) makes a wireframe box with width w, height h and 
// depth d centered at the origin. It uses the GLUT wire cube function.
// The calls to glPushMatrix and glPopMatrix are essential here; they enable
// this function to be called from just about anywhere and guarantee that
// the glScalef call does not pollute code that follows a call to myWireBox.
void wireBox(GLdouble width, GLdouble height, GLdouble depth)
{
	glPushMatrix();
	glScalef(width, height, depth);
	glutWireCube(1.0);
	glPopMatrix();
}
// Displays the arm in its current position and orientation. The whole
// function is bracketed by glPushMatrix and glPopMatrix calls because every
// time we call it we are in an "environment" in which a gluLookAt is in 
// effect. (Note that in particular, replacing glPushMatrix with
// glLoadIdentity makes you lose the camera setting from gluLookAt).
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Draw the upper arm, rotated shoulder degrees about the z-axis. Note that
	// the thing about glutWireBox is that normally its origin is in the middle
	// of the box, but we want the "origin" of our box to be at the left end of
	// the box, so it needs to first be shifted 1 unit in the x direction, then
	// rotated.
	glRotatef((GLfloat)shoulderAngle,0.0,0.0,1.0);
	glTranslatef(0.75,0.0,0.0);
	wireBox(0.75,0.4,1.0);

	// Now we are ready to draw the 4 lower arms. Since the lower arms are attached
	// to the upper arm we put the code here so that all rotations we do are
	// relative to the rotation that we already made above to orient the upper
	// arm. So we want to rotate elbow degrees about the z-axis. But, like
	// before, the anchor point for the rotation is at the end of the box, so
	// we translate <1,0,0> before rotating. But after rotating we have to
	// position the lower arm at the end of the upper arm, so we have to 
	// translate it <1,0,0> agian.
	glTranslatef(0.25,0.0,0.0);
	glRotatef((GLfloat)elbowAngle,0.0,0.0,1.0);
	glTranslatef(0.5,0.0,0.0);
	wireBox(0.75,0.4,1.0);

	// second joint
	glTranslatef(0.25,0.0,0.0);
	glRotatef((GLfloat)elbowAngle2,0.0,0.0,1.0);
	glTranslatef(0.5,0.0,0.0);
	wireBox(0.75,0.4,1.0);	

	// third joint
	glTranslatef(0.5,0.0,0.0);
	glRotatef((GLfloat)elbowAngle3,0.0,0.0,1.0);
	glTranslatef(0.25,0.0,0.0);
	wireBox(0.75,0.4,1.0);
	// fourth joint
	glTranslatef(0.5,0.0,0.0);
	glRotatef((GLfloat)elbowAngle4,0.0,0.0,1.0);
	glTranslatef(0.25,0.0,0.0);
	wireBox(0.75,0.4,1.0);

	glPopMatrix();
	glFlush();



	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));
}

// Handles the reshape event by setting the viewpoint so that it takes up the
// whole visible region, then sets the projection matrix to something reason-
// able that maintains proper aspect ration.
void reshape(GLint w, GLint h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0, ((GLfloat)w)/((GLfloat)h),1.0,20.0);
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
		{
			elbowAngle = (elbowAngle +5)%360; 
			glutPostRedisplay();
		} 
		break;
		case 'm':
		{
			elbowAngle = (elbowAngle -5)%360; 
			glutPostRedisplay();
		} 
		break;
		case 'k':
		{
			elbowAngle3 = (elbowAngle3 +5)%360; 
			glutPostRedisplay();
		} 
		break;
		case 'l':
		{
			elbowAngle3 = (elbowAngle3 -5)%360; 
			glutPostRedisplay();
		} 
		break;
		case 'h':
		{
			elbowAngle4 = (elbowAngle4 +5)%360; 
			glutPostRedisplay();
		} 
		break;
		case 'j':
		{
			elbowAngle4 = (elbowAngle4 -5)%360; 
			glutPostRedisplay();
		} 
		break;
		default:
		 break;
	}

}
// Performs application sepcific initialization: turn off smooth shading,
// sets the viewing transformation once and for all. In this application we
// won't be moving the camera at all, so it makes sense to do this.
void init()
{
	glShadeModel(GL_FLAT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(1,2,8,0,0,0,0,1,0);
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutCreateWindow("Robot Arm");
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,600);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}