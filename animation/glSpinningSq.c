/**
	Date : 2021 Febuary 18
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/

	This is OpenGL skeleton source code.
	- Animates a spinning square.
    - Illustrates animation with double buffering. This requires passing GLUT_DOUBLE to glutInitDisplayMode, generating the parameters for the next animation frame in the timer callback which posts a redisplay request, and finishing off the display callback with a call to glutSwapBuffers.
    - Shows that it is better to accumulate transformation parameters, rather than the transformations themselves, when doing cyclical animation.
    - Illustrates the mouse callback.
    - Shows that callbacks can be removed. In this case, the idle callback gets removed.
    - Shows that maintaining the aspect ratio of a drawing when an orthographic projection is used is a bigger pain than doing so with a perspective projection using gluPerspective.
	How to build :

	MAC OS :
	gcc glSpinningSq.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glSpinningSq

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

#define FALSE 0
#define TRUE 1

// Set this to true to animate
static unsigned char isSpinning = TRUE;

// This is the number of frames per second to render.
static const int FPS =80;

// This global var keeps track of the current orientation of the square.
static GLfloat currentAngleOfRotation = 0.0;

// Handles the window reshape event by first ensuring that the viewport fills 
// the entire drawing surface. Then we use a simple orthographic projection
// with a logical coordinate system ranging from -50 .. 50 in the smaller of 
// the width or height, and scale the larger dimension to make the whole
// window isotropic.
void reshape(GLint w, GLint h)
{
	glViewport(0,0,w,h);
	GLfloat aspect = (GLfloat)w/(GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
	{
		// width is smaller, go from -50 ... 50 in width
		glOrtho(-50.0, 50.0, -50.0/aspect, 50.0/aspect,-1.0,1.0);
	}
	else
	{
		// height is smaller, go from -50 .. 50 in height
		glOrtho(-50.0*aspect,50.0*aspect, -50.0, 50.0,-1.0,1.0);
	}
}

// Handles the display callback as follows: first clears the window, then draws
// a 50x50 rectangle centered at the origin and rotated the correct number
// of degrees around the vector <0,0,1>. This function ends with a
// 'glutSwapBuffers' call because when the display mode is double buffered,
// drawing takes place on the back buffer; we have to call glutSwapBuffers()
// to show what we have drawn.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(currentAngleOfRotation,0.0,0.0,1.0);
	glRectf(-25.0,-25.0,25.0,25.0);
	glFlush();

	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));

	glutSwapBuffers();
}

// Handles the timer by incrementing the angle of rotation and requesting the
// window to diplay again, provided the prgram is in the spinning state.
// Since the timer function is only called once, it sets the same function to
// be called again.
void timer(int v)
{
	if (isSpinning == TRUE)
	{
		currentAngleOfRotation += 6.0;
		if (currentAngleOfRotation > 360.0)
		{
			currentAngleOfRotation -= 360.0;
		}
		glutPostRedisplay();
	}
	glutTimerFunc(1000/FPS, timer,v);
}
// Handle mouse events as follows : when the left mouse is pressed, generate
// new animation frames while the application is idle; 
// when the right button is pressed, remove any idle-time callbacks, thus stopping the 
// animation.
void mouse(int button, int state, int x,int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		isSpinning = TRUE;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		isSpinning = FALSE;
	}
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
	glutInitDisplayMode(GLUT_RGB|GLUT_DOUBLE);
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,500);
	glutCreateWindow("Spinning Sq");
	glutReshapeFunc(reshape);
	glutTimerFunc(60,timer,0);
	glutMouseFunc(mouse);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}