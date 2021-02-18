/**
	Date : 2021 Febuary 18
	This is from  https://cs.lmu.edu/~ray/notes/openglexamples/

	This is OpenGL Sierpinski3D source code.
	 - Draws points in the iteration of a 3-D Sierpinski Gasket for as long as the program runs.
     - Illustrates the right way to perfom long-running drawing: in bursts during the idle time callback. The display callback doesn't do any drawing, other than clearing the windows (er, color buffer, I mean).
     - Uses depth buffering so that points that would appear behind points already drawn will not themselves be drawn.
     - Shows that depth buffering must be done by specifying the GL_DEPTH flag in glutInitDisplayMode and enable GL_DEPTH_TEST.
     - Illustrates the reshape callback, which is absolutely essential for preserving the aspect ratio of the drawing.
     - Shows that in order to preserve the aspect ration of the drawing, you set up the camera (in this case using gluPerspective) every time the window is reshaped. Note that the reshape callback is automatically called by GLUT shortly after the windowing system creates your window, so you don't need to force a call to a camera setup at the beginning of your program.
     - Sets colors based on depth to make the picture look pretty cool.
	How to build :

	MAC OS :
	gcc glSierpinski3D.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glSierpinski3D

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

// A simple three-dimensional point class to make life easy. It allows you to
// reference points with x and y coordinates instead of array indices and
// encapsulates a midpoint function;
struct Point
{
	GLfloat x,y,z;
};
struct Point midpoint(struct Point p1,struct Point p2){return (struct Point){(p1.x+p2.x)/2.0,(p1.y+p2.y)/2.0,(p1.z+p2.z)/2.0};}

// Handles reshape requests by setting the viewport to exactly match the 
// pixel coordinates, so we can draw in the whold window. Then sets up a simple
// perspective viewing volume to ensure that the pyramid will never
// be distored when the window is reshaped. The particular settings chosen
// ensure that the vertical extent of the pyramid will always be visible.
void reshape(GLint w, GLint h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(100.0, ((GLfloat)w)/((GLfloat)h), 10.0, 1500.0);
}


// Handles display requests. All it has to do is clear the viewpoint because
// the real drawing is done in the idle callback.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

// Draw the "next 500 points". The function contains locally the definitions
// of the vertices and the current point as static objects so that their values.
// are retained between calls.
void generateMorePoints()
{
	// The tetrahedron has four vertices. We also have to keep track of the 
	// current point during the plotting.
	static struct Point vertices[4] = {{-250,-225,-200},{-150,-225,-700},{250,-225,-275},{0,450,-500}};
	static struct Point lastPoint = {-250,-225,-200};

	// Here is the code to draw the "next 500 points". The closer the point is 
	// to the camera, the brighter we make it. The coloring techique is a 
	// quick hack which (unprofessionally) depends on the fact that the range
	// of z-coordinates in the tetrahedron run from -200 to -700. By adding
	// 700 to any z-value and then dividing by 500, we get value in the range
	// 0 to 1 - just perfect for coloring.
	glBegin(GL_POINTS);
	int i = 0;
	for (i=0;i<=500;i++)
	{
		lastPoint = midpoint(lastPoint,vertices[rand()%4]);
		GLfloat intensity = (700 + lastPoint.z)/500.0;
		glColor3f(intensity,intensity,0.25);
		glVertex3f(lastPoint.x,lastPoint.y,lastPoint.z);
	}
	glEnd();
	glFlush();

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

// Performs application-specific initialization. In this program we want to
// do depth buffering, which has to be explicitly enabled in OpenGL.
void init()
{
	glEnable(GL_DEPTH_TEST);
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE|GLUT_DEPTH);
	glutInitWindowSize(500,500);
	glutInitWindowPosition(40,40);
	glutCreateWindow("Sierpinski Triangle 3D");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(generateMorePoints);
	init();
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return EXIT_SUCCESS;
}