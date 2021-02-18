# Animation #
	the following information are about purpose of the source code with C programming

## Spinning Square: ##
File name: glSpinningSq.c
	* Animates a spinning square.
    * Illustrates animation with double buffering. This requires passing GLUT_DOUBLE to glutInitDisplayMode, generating the parameters for the next animation frame in the timer callback which posts a redisplay request, and finishing off the display callback with a call to glutSwapBuffers.
    * Shows that it is better to accumulate transformation parameters, rather than the transformations themselves, when doing cyclical animation.
    * Illustrates the mouse callback.
    * Shows that callbacks can be removed. In this case, the idle callback gets removed.
    * Shows that maintaining the aspect ratio of a drawing when an orthographic projection is used is a bigger pain than doing so with a perspective projection using gluPerspective.
