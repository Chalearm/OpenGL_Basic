/**
	Date : 2021 Febuary 19
	This is from https://cs.lmu.edu/~ray/notes/openglexamples/

	This is Lit Solids

	How to build :

	MAC OS :
	gcc glLitSolds.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o glLitSolds

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
// Clears the window and depth buffer and draws three solids.
//
// The solids are placed so that they either sit or float above the x-z plane:
// therefore note noe of the first things that is done is to rotate the whole
// scene 20 degrees about x to turn the top of the scene toward the viewer.
// This lets the viewer see how the torus goes around the cone.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	// Rotate the scene so we can see the tops of the shapes.
	glRotatef(-20.0,1.0,0.0,0.0);

	// Make a torus floating 0.5 above the x-z plane. THe standard torus in
	// the GLUT library is, perhaps surprisingly, a stack of circles which
	// encircle the z-axis, so we need to rotate it 90 degrees about x to
	// get it the way we want.
	glPushMatrix();
	glTranslatef(-0.75,0.5,0.0);
	glRotatef(90.0,1.0,0.0,0.0);
	glutSolidTorus(0.275,0.85,16,40);
	glPopMatrix();

	// Make a cone, The standard cone "points" along z; we want it pointing
	// along y, hence the 270 degree rotation about x.
	glPushMatrix();
	glTranslatef(-0.75,-0.5,0.0);
	glRotatef(270.0,1.0,0.0,0.0);
	glutSolidCone(1.0,2.0,70,12);
	glPopMatrix();

	// Add a sphere to the scene.
	glPushMatrix();
	glTranslatef(0.75,0.0,-1.0);
	glutSolidSphere(1.0,30,30);
	glPopMatrix();

	glPopMatrix();
	glFlush();


	// Always check for errors
	int err;
	while((err = glGetError()) != GL_NO_ERROR)
	printf("display: %s\n",gluErrorString(err));

	glutSwapBuffers();
}


// We don't want the scene to get distorted when the window size changes, so 
// we need a reshape callback. We'll always maintain a range of -2.5 .. 2.5 in
// the smaller of the width and height for our viewbox, and a range of -10..10
// for the viewbox depth
void reshape(GLint w,GLint h)
{
	glViewport(0,0,w,h);
	glMatrixMode(GL_PROJECTION);
	GLfloat aspect = ((GLfloat)w)/((GLfloat)h);
	glLoadIdentity();
	if (w<=h)
	{
		// width is smaller, so stretch out the height
		glOrtho(-2.5, 2.5, -2.5/aspect, 2.5/aspect,-10.0,10.0);
	}
	else
	{
		glOrtho(-2.5*aspect,2.5*aspect,-2.5,2.5,-10.0,10.0);
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
// Performs application specific initialization, It defines lighting
// parameters for light source GL_LIGHT0: black for ambient, yellow for
// diffuse, white for specular, and makes it a directional source
// shining along <-1,-1,-1>. It also sets a couple material properties
// to make cyan colored objects with a fairly low shininess value. Lighting
// and depth buffer hidden surface removal are enabled here.
void init()
{
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
	glEnable(GL_DEPTH_TEST);

}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_SINGLE|GLUT_DEPTH);
	glutCreateWindow("Cyan Shapes in Yellow Light");
	glutInitWindowPosition(80,80);
	glutInitWindowSize(800,600);
	glutReshapeFunc(reshape);

	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	init();
	glutMainLoop();

	return EXIT_SUCCESS;
}