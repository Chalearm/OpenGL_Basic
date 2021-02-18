# Animation #
	the following information are about purpose of the source code with C programming

## Robot Arm ##
File name: glRobotarm.c and glRobotarm4.c

	* Lets you move a robot arm, with a shoulder and an elbow, with the arrow keys.
    * Is a very simple program that manages to illustrate the very important concept of hierarchical modeling.
    * Shows that to draw an object in the context of a "parent" object you arrange drawing and matrix manipulation code in a certain order.
    * Illustrates glPushMatrix and glPopMatrix (note how this makes wireBox "reusable" in any context).
    * Shows that the keyboard callback, for keys other than normal printable characters, is called the "special" callback in GLUT.
