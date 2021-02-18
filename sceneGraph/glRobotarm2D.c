/**
	Date : 2021 Febuary 19
	This is from http://titan.csit.rmit.edu.au/~e20068/teaching/i3dg&a/2020/robot-arm-2D.c
	
	This is Robot arm a joint , glRobotarm.c

	How to build :

	MAC OS :
	gcc glRobotarm2D.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glRobotarm2D

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

#define SHOULDER 0
#define ELBOW 1
#define WRIST 2
#define LEFT_PLATE 3
#define RIGHT_PLATE 4
#define N_JOINTS 5
float robot_arm[N_JOINTS] = {0.0,0.0,0.0,0.0,0.0};
float increment[N_JOINTS] = {1.0,1.0,0.0,0.01,0.01};

void myWireRect(float l, float h)
{
	glPushMatrix();
	glScalef(l,h,1.0);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.5,-0.5);
	glVertex2f(0.5,-0.5);
	glVertex2f(0.5,0.5);
	glVertex2f(-0.5,0.5);
	glEnd();
	glPopMatrix();
}
 void myWireRhombus(float l, float h)
 {
 	glPushMatrix();
 	glScalef(l,h,1.0);
 	glScalef(1.0/sqrt(2.0),1.0/sqrt(2.0), 1.0);
 	glRotatef(45.0,0.0,0.0,1.0);
 	myWireRect(1.0,1.0);
 	glPopMatrix();
 }

 void drawArm()
 {
 	glPushMatrix();
 	// Shoulder 
 	glTranslatef(0.0,0.0,0.0);
 	glRotatef(robot_arm[SHOULDER],0.0,0.0,1.0);
 	// Upper arm
 	glTranslatef(0.2,0.0,0.0);
 	myWireRhombus(0.4,0.2);
 	// Elbow
 	glTranslatef(0.2,0.0,0.0);
 	glRotatef(robot_arm[ELBOW],0.0,0.0,1.0);
 	// Lower arm
 	glTranslatef(0.0,-0.2,0.0);
 	myWireRhombus(0.2,0.4);
 	// Wrist
 	glTranslatef(0.0,-0.2,0.0);
 	glRotatef(robot_arm[WRIST],0.0,0.0,1.0);
 	// Hand
 	glTranslatef(0.0,-0.025,0.0);
 	myWireRect(0.2,0.05);
 	// Gripper
 	glPushMatrix();
 	glTranslatef(-0.07+robot_arm[LEFT_PLATE],-0.1,0.0);
 	myWireRect(0.05,0.125);
 	glPopMatrix();
 	glPushMatrix();
 	glTranslatef(0.075-robot_arm[RIGHT_PLATE],-0.1,0.0);
 	myWireRect(0.05,0.125);
 	glPopMatrix();

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

	drawArm();
	glutSwapBuffers();



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
	glOrtho(-1.0,1.0,-1.0,1.0,-1,1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x,int y)
{
	switch(key)
	{
		case 'S':
			robot_arm[SHOULDER] += increment[SHOULDER];
			glutPostRedisplay();
		break;
		case 's':
			robot_arm[SHOULDER] -= increment[SHOULDER];
			glutPostRedisplay();
		break;
		case 'E':
			robot_arm[ELBOW] += increment[ELBOW];
			glutPostRedisplay();
		break;
		case 'e':
			robot_arm[ELBOW] -= increment[ELBOW];
			glutPostRedisplay();
		break;
		case 'L':
			robot_arm[LEFT_PLATE] += increment[LEFT_PLATE];
			glutPostRedisplay();
		break;
		case 'l':
			robot_arm[LEFT_PLATE] -= increment[LEFT_PLATE];
			glutPostRedisplay();
		break;
		case 'R':
			robot_arm[RIGHT_PLATE] += increment[RIGHT_PLATE];
			glutPostRedisplay();
		break;
		case 'r':
			robot_arm[RIGHT_PLATE] -= increment[RIGHT_PLATE];
			glutPostRedisplay();
		break;
		case 27:
		case 'q':
		   exit(EXIT_SUCCESS);
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
	glClearColor(0.0,0.0,0.0,0.0);
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutCreateWindow("Robot Arm 2D");
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,600);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}