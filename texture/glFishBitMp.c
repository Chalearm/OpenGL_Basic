/**
	Date : 2021 Febuary 19
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/
	
	Fish Bitmaps

	How to build :

	MAC OS :
	gcc glCheckeredTri.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glCheckeredTri

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

// A fish bitmap, size is 27x11, but all rows must have a multiple of 8 bits,
// So we define it like it is 32x11.
GLubyte fish[] = 
{
	0x00, 0x60, 0x01, 0x00,
	0x00, 0x90, 0x01, 0x00,
	0x03, 0xf8, 0x02, 0x80,
	0x1c, 0x37, 0xe4, 0x40,
	0x20, 0x40, 0x90, 0x40,
	0xc0, 0x40, 0x78, 0x80,
	0x41, 0x37, 0x84, 0x80,
	0x1c, 0x1a, 0x04, 0x80,
	0x03, 0xe2, 0x02, 0x40,
	0x00, 0x11, 0x01, 0x40,
	0x00, 0x0f, 0x00, 0xe0,
};

// return a random float in the range 0.0 to 1.0.
GLfloat randomFloat()
{
	return (GLfloat)rand()/RAND_MAX;
}
// Clears the window then plots 20 fish bitmaps in random colors at random
// positions.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	int i = 0;
	for (i = 0;i < 20;i++)
	{
		glColor3f(randomFloat(),randomFloat(), randomFloat());
		glRasterPos3f(randomFloat(),randomFloat(),0.0);
		glBitmap(27,11,0,0,0,0,fish);
	}
	glFlush();
	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));
}

// On reshape, uses an orthographic projection with world coordinates ranging
// from 0 to 1 in the x and y directions, and -1 to 1 in z.
void reshape(GLint w, GLint h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,1,0,1);
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

// Initializes GLUT and enters the main Loop.
int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE);
	glutCreateWindow("Fishies");
	glutInitWindowPosition(80,80);
	glutInitWindowSize(400,300);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}