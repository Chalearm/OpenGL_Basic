# Animation #
	the following information are about purpose of the source code with C programming

## Spinning Square ##
File name: glSpinningSq.c

	* Animates a spinning square.
    * Illustrates animation with double buffering. This requires passing GLUT_DOUBLE to glutInitDisplayMode, generating the parameters for the next animation frame in the timer callback which posts a redisplay request, and finishing off the display callback with a call to glutSwapBuffers.
    * Shows that it is better to accumulate transformation parameters, rather than the transformations themselves, when doing cyclical animation.
    * Illustrates the mouse callback.
    * Shows that callbacks can be removed. In this case, the idle callback gets removed.
    * Shows that maintaining the aspect ratio of a drawing when an orthographic projection is used is a bigger pain than doing so with a perspective projection using gluPerspective.

## Color Cube Flyby ##
File name: glColorCubeFlyby.c

	* Is our first example of a "flyby" program.
	* Performs animation by moving the camera on a closed orbit around a stationary object, which just happens to be the standard RGB color cube.
	* Shows that on flybys, you set the camera shape (gluPerspective in GL_PROJECTION mode) in the reshape callback, but set the camera position and orientation (gluLookAt in GL_MODELVIEW mode) for every frame you draw.
	* Draws with GL_QUADS and the moderately ugly glVertex3iv function.
	* Animates with a timer doing a delay of 16.6667 ms between finishing the drawing of one frame and starting the next.
