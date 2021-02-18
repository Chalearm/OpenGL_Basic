/**
	Date : 2021 Febuary 19
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/
	
	This is Robot arm a joint , glBouncingBalls.c

	How to build :

	MAC OS :
	gcc glBouncingBalls.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glBouncingBalls

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

// Colors
GLfloat WHITE[] = {1,1,1};
GLfloat RED[] = {1,0,0};
GLfloat GREEN[] = {0,1,0};
GLfloat MAGENTA[] = {1,0,1};

// A camera. It moves horizontally in a circle centered at the origin of 
// radius 10. It moves verticially straight up and down.
struct Camera
{
	double theta;	// determines the x and z positions
	double y;       // the current y position
	double dTheta;  // increment in there for swinging the camera around
	double dy; 		// increment in y for moving the camera up/down
};


double getXCamera(struct Camera *obj){return 10*cos(obj->theta);}
double getYCamera(struct Camera *obj){return obj->y;}
double getZCamera(struct Camera *obj){return 10*sin(obj->theta);}
void moveRightCamera(struct Camera *obj){obj->theta += obj->dTheta;}
void moveLeftCamera(struct Camera *obj){obj->theta -= obj->dTheta;}
void moveUpCamera(struct Camera *obj){obj->y += obj->dy;}
void moveDownCamera(struct Camera *obj){if(obj->y > obj->dy)obj->y -= obj->dy;}

struct Ball 
{
	double radius;
	GLfloat *color;
	double maximumHeight;
	double x;
	double y;
	double z;
	int direction;
};


void updateBall(struct Ball *obj)
{
	obj->y += obj->direction*0.05;
	if (obj->y > obj->maximumHeight)
	{
		obj->y = obj->maximumHeight;
		obj->direction = -1;
	}
	else if ( obj->y < obj->radius)
	{
		obj->y = obj->radius;
		obj->direction = 1;
	}
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE,obj->color);
	glTranslatef(obj->x,obj->y,obj->z);
	glutSolidSphere(obj->radius,30,30);
	glPopMatrix();
}

struct CheckerBoard
{
	int displayListId;
	int width;
	int depth;
};

double centerXCHB(struct CheckerBoard *obj){return obj->width/2;}
double centerZCHB(struct CheckerBoard *obj){return obj->depth/2;}
void createCHB(struct CheckerBoard *obj)
{
	obj->displayListId = glGenLists(1);
	glNewList(obj->displayListId, GL_COMPILE);
	GLfloat lightPosition[] = {4,3,7,1};
	glLightfv(GL_LIGHT0,GL_POSITION,lightPosition);
	glBegin(GL_QUADS);
	glNormal3d(0,1,0);
	int x,z;
	for (x = 0; x <obj->width-1;x++)
		for (z=0;z < obj->depth-1;z++)
		{
			glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,(x+z)%2 ==0?RED:WHITE);
			glVertex3d(x,0,z);
			glVertex3d(x+1,0,z);
			glVertex3d(x+1,0,z+1);
			glVertex3d(x,0,z+1);
		}
	glEnd();
	glEndList();
}
void drawCHB(struct CheckerBoard *obj)
{
	glCallList(obj->displayListId);
}

struct CheckerBoard aCheckerboard = {0,8,8};
struct Camera aCamera = {0,3,0.04,0.2};
struct Ball balls[] = {{1,GREEN,6,2,6,1,-1},{1.5,MAGENTA,3,4,3,4,-1},{0.4,WHITE,1,5,2,7,-1}};

// Application-specific initialization : Set up global lighting parameters
// and create display list.
void init()
{
	glEnable(GL_DEPTH_TEST);
	glLightfv(GL_LIGHT0,GL_DIFFUSE,WHITE);
	glLightfv(GL_LIGHT0,GL_SPECULAR,WHITE);
	glMaterialfv(GL_FRONT,GL_SPECULAR,WHITE);
	glMaterialf(GL_FRONT,GL_SHININESS,30);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	createCHB(&aCheckerboard);
}

// Draws one frame, the checkerboard then the balls , form the  current  camera
// position
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(getXCamera(&aCamera),getYCamera(&aCamera),getZCamera(&aCamera),
		centerXCHB(&aCheckerboard),0.0,centerZCHB(&aCheckerboard),
		0.0,1.0,0.0);
	drawCHB(&aCheckerboard);
	int i = 0;
	for (i =0;i <sizeof(balls)/sizeof(struct Ball);i++)
	{
		updateBall(&balls[i]);
	}
	glFlush();
	glutSwapBuffers();




	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));
}

// On reshape, constructs a camera that perfectly fits the window.
void reshape(GLint w, GLint h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, ((GLfloat)w)/((GLfloat)h),1.0,150.0);
	glMatrixMode(GL_MODELVIEW);
}

//Requests to draw the next frame.
void timer(int v)
{
	glutPostRedisplay();
	glutTimerFunc(1000/60,timer,v);
}

void special(int key, int a,int b)
{
	switch (key)
	{
		case GLUT_KEY_LEFT : 
		moveLeftCamera(&aCamera);
		break;
		case GLUT_KEY_RIGHT:
		moveRightCamera(&aCamera);
		break;
		case GLUT_KEY_UP:
		moveUpCamera(&aCamera);
		break;
		case GLUT_KEY_DOWN:
		moveDownCamera(&aCamera);
		break;
	}
	glutPostRedisplay();
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
	glutCreateWindow("Bouncing Balls");
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,600);
	glutDisplayFunc(display);
	glutSpecialFunc(special);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutTimerFunc(100,timer,0);
	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}