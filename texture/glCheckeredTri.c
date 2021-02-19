/**
	Date : 2021 Febuary 19
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/
	
	Checkerd Triangles , glCheckeredTri.c

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


// Define a 2x2 red and yellow checkered pattern using RGB colors.
#define red {0xff,0x00,0x00}
#define yellow {0xff,0xff,0x00}
#define megemnta {0xff,0,0xff}
GLubyte texture[][3] = {red,yellow,megemnta,red};

// Draws three textured triangles. Each triangles uses the same texture.
// but the mappings of texture coordinates to vertex coordinates is
// differenct in each triangle.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.5,1.0); glVertex2f(-3,3);
	glTexCoord2f(0.0,0.0); glVertex2f(-3,0);
	glTexCoord2f(1.0,0.0); glVertex2f(0,0);

	glTexCoord2f(4,8);     glVertex2f(3,3);
	glTexCoord2f(0.0,0.0); glVertex2f(0,0);
	glTexCoord2f(8,0.0);   glVertex2f(3,0);

	glTexCoord2f(5,5);     glVertex2f(0,0);
	glTexCoord2f(0.0,0.0); glVertex2f(-1.5,-3);
	glTexCoord2f(4,0.0);   glVertex2f(1.5,-3);
	glEnd();
	glFlush();


	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));
}

// Fixed up camera and remaps texture when window reshaped.
void reshape(GLint w, GLint h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(80, ((GLfloat)w)/h,1,40);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(2,-1,5,0,0,0,0,1,0);
	
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	glTexImage2D(GL_TEXTURE_2D,
				  0,				// level 0
				  3,				// use only R, G, and B components
				  2,2,   			// texture has 2x2 texels
				  0, 				// no border
				  GL_RGB, 			// texels are in RGB format
				  GL_UNSIGNED_BYTE, // texels are in RGB format
				  texture); 		// color components are unsigned bytes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
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
	glutCreateWindow("Textured Triangles");
	glutInitWindowPosition(80,80);
	glutInitWindowSize(520,390);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}