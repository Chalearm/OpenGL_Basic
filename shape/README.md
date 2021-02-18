# Shape #
	the following information are about purpose of the source code with C programming

## Triangle ##
File name: glTriangle.c

	* just a static picture of a colored triangle.
	* Shows how to use GLUT.
	* Has minimal structure: only main() and a display callback.
	* Uses only the default viewing parameters (in fact, it never mentions viewing at all). This is an orthographic view volume with bounds of -1..1 in all three dimensions.
	* Draws only using glColor and glVertex within glBegin and glEnd in the display callback.
	* Uses only the GL_POLYGON drawing mode.
	* Illustrates glClear and glFlush.

## Torus ##
File name: glTorus.c

	* Display a static picture of a torus and the coordinate system axes.
    * Instead of using a primitive GL function (such as glOrtho or glFrustum) to set the viewing volume, it instead uses gluPerspective. gluPerspective must be done when the matrix mode is GL_PROJECTION.
    * Instead of using translates and rotates to position the objects into the view volume, it instead uses gluLookAt to position and orient the camera so that it is looking at the objects. gluLookAt must be done when the matrix mode is GL_MODELVIEW.
    * Shows that the combination of gluPerspective and gluLookAt is considerably more intuitive than the more primitive mechanism of glFrustum and plain object transformations.
    * Draws the torus using the fancy glutWireTorus function (which internally calls glVertex a zillion times).

## Tetrahedron ##
File name: glTetrahedron.c
    *  Displays a static picture of a tetrahedron sitting on a grid "floor".
	* Sets up a viewing volume at the beginning of the program. Uses glFrustum in order to make a perspective projection.
	* Shows that you have to define the viewing volume when the matrix mode is GL_PROJECTION.
	* Draws the object in the display callback using GL_LINES and GL_TRIANGLE_STRIP modes.
	* Defines the object at the origin then moves it INTO the view volume.
	* Shows that the rotations and translations that move the object into the view volume must be performed when the matrix mode is GL_MODELVIEW.
	* Shows that if you are not doing animation it is best to define the transformations that position and orient the object should be done only once, and not inside the display callback.
	* Shows that transformations are cumulative.
	* Shows that before setting up view volumes or object transformations, you usually want to do a glLoadIdentity or else transforamtions from previous manipulations will accumulate.
	* Does backface culling as a very cheap way to do hidden surface removal. This does not work in all cases.