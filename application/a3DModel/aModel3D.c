

#if _WIN32
#define _CRT_SECURE_NO_WARNINGS
#include "Include/glew.h"
#include "Include/freeglut.h"
#endif
#if __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#define __gl_h_
#include <GLUT/glut.h>
#endif
#if __LINUX__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define V_ARRAY_SIZE 30000
#define VT_ARRAY_SIZE 30000
#define VN_ARRAY_SIZE 30000
#define F_ARRAY_SIZE 30000

#define FILENAME_SIZE 40

// Robot control

#define ROBOT_TIMER_UPDATE -6
#define RESET_ARM_PARAM -5
#define WHICH_IS_CONTROL -4
#define DISABLE_AUTO_ARM_MOVE -1
#define ENABLE_AUTO_ARM_MOVE -2
#define IS_AUTOMOVE_ARM -3
#define CONTROL_JOINT1 0

#define NUM_ARM_JOINT 6
#define CONTROL_FINGER NUM_ARM_JOINT

#define CHANGE_CONTROL_PART 12
#define CONTROL_PARAM1 0
#define CONTROL_PARAM2 2
#define CONTROL_PARAM3 3


// Camera control
#define DISABLE_AUTOMOVE_CAMERA 1
#define ENABLE_AUTOMOVE_CAMERA 2
#define IS_AUTOMOVE_CAMERA 3
#define INITIAL_CAMERA 4
#define RESET_CAMERA_POS 5
#define UPDATE_EYE_X 6
#define UPDATE_EYE_Y 7
#define UPDATE_EYE_Z 8
#define UPDATE_CENT_X 9
#define UPDATE_CENT_Y 10
#define UPDATE_CENT_Z 11
#define UPDATE_UP_X 12
#define UPDATE_UP_Y 13
#define UPDATE_UP_Z 14
#define NOT_CAMERA_CONTROL_MODE 15
#define CAMERA_CONTROL_MODE 16
#define CAMERA_TIMER_UPDATE 17
#define SET_MODE_CAMERA 18
#define GET_MODE_CAMERA 19


// Change information page
#define HOME_INFO 0
#define ROBOT_SETTING_INFO 1
#define CAMERA_SETTING_INFO 2

struct vector3Df
{
	GLfloat dat[3];
};
struct vector3DI
{
	int x;
	int y;
	int z;
};
struct vector2Df
{
	GLfloat dat[2];
};

struct modelLoader
{
	char  objFilename[FILENAME_SIZE];
	char  mtlFilename[FILENAME_SIZE];
	char  imageFile[FILENAME_SIZE];
	struct vector3DI f[F_ARRAY_SIZE];
	struct vector3Df vn[VN_ARRAY_SIZE];
	struct vector2Df vt[VT_ARRAY_SIZE];
	struct vector3Df v[V_ARRAY_SIZE];
	int numV;
	int numVn;
	int numVt;
	int numF;
};
struct armControlParameter
{
	double jointRadian[NUM_ARM_JOINT+1][3]; // x,y ,z
	double fingersWidth;
};
void constructorModelLoader(struct modelLoader* obj, const char* objFilename, const char* imgFile);
void loadObj(struct modelLoader* obj);
void loadTexture(struct modelLoader* obj);
void drawModel(struct modelLoader* obj);
double cameraControl(int controlParameter, int controlParameter2);
double armControl(int controlParameter, int controlParameter2);
void drawRobotArm(struct armControlParameter* obj);
double boundValue(const double max, const double min, const double original);
int inRange(const double max, const double min, const double value);
void displayInterfaceText();
void setColorLight(GLfloat* color);
struct modelLoader aModelLoader;
struct modelLoader aJointPart;
struct modelLoader aFinger;
struct armControlParameter armParam;
int enableWireFrame = 0;
int enableCullBack = 0;
int changeLight = 0;
int pageStatus = HOME_INFO;

#define white  { 1.0,1.0,1.0,1.0 }
#define yellow  { 1.0,1.0,0.0,1.0 }
#define cyan  { 0.0,1.0,1.0,1.0 }
#define red  {1.0,0.0,0.0,1.0}
#define blue  {0.0,0.0,1.0,1.0}
#define green {0.0,1.0,0.0,1.0}
GLfloat colorSet[][4] = { white,yellow,cyan,red,blue,green};

int signVal = 1;


void
output(GLfloat x, GLfloat y, char *format,...)
{
  va_list args;
  char buffer[200], *p;

  va_start(args, format);
  vsprintf(buffer, format, args);
  va_end(args);
  glPushMatrix();
  glTranslatef(x, y, 0);
  for (p = buffer; *p; p++)
    glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
  glPopMatrix();
}


// Clears the window and draw the torus.
void display()
{

		glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	if( cameraControl(GET_MODE_CAMERA,0) == NOT_CAMERA_CONTROL_MODE)
	{
		displayInterfaceText();
	}
	else
	{
	//	  glMatrixMode(GL_MODELVIEW);
	//	  glPushMatrix();
		//		  glMatrixMode(GL_PROJECTION);
		 // glPushMatrix();
		 // glLoadIdentity();

	//	glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
		//glPopMatrix();
		cameraControl(INITIAL_CAMERA, 0);
	}
	if (enableCullBack == 1)
	{

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}

	if (enableWireFrame == 1)
	{
		//glPolygonMode(GL_FRONT, GL_LINE);
		//glPolygonMode(GL_BACK, GL_LINE);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	}

	//  glMatrixMode(GL_MODELVIEW);
	 // glLoadIdentity();
	//glPushMatrix();
	//glTranslatef(0,0,-31);
	drawRobotArm(&armParam);

	//glPopMatrix();

	if (enableWireFrame == 1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	glFlush();
	// Always check for errors
	int err;
	while ((err = glGetError()) != GL_NO_ERROR)
		printf("display: %s\n", gluErrorString(err));

	glutSwapBuffers();

	//glLoadIdentity();
}
// Advances the orbiter and requests to draw the next frame.
void timer(int v)
{

	cameraControl(CAMERA_TIMER_UPDATE, 0);
	armControl(ROBOT_TIMER_UPDATE, 0);
	glutPostRedisplay();
	glutTimerFunc(1000 / 60.0, timer, v);
}
void special(int key, int a, int b)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		//moveLeftCamera(&aCamera);
		cameraControl(UPDATE_EYE_X, -5);
		break;
	case GLUT_KEY_RIGHT:
		cameraControl(UPDATE_EYE_X, 5);
		break;
	case GLUT_KEY_UP:
		cameraControl(UPDATE_EYE_Y, -5);
		break;
	case GLUT_KEY_DOWN:
		cameraControl(UPDATE_EYE_Y, 5);
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	int temp = 0;
	switch (key)
	{
	case 27:
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case 'a':
		cameraControl(DISABLE_AUTOMOVE_CAMERA + (((int)cameraControl(IS_AUTOMOVE_CAMERA, 0) + 1) % 2), 0);
		break;
	case 's':
		armControl(DISABLE_AUTO_ARM_MOVE - (((int)armControl(IS_AUTOMOVE_ARM, 0) + 1) % 2), 0);
		break;
	case 'z':
		armControl(CHANGE_CONTROL_PART,  ((int)armControl(WHICH_IS_CONTROL, 0)+1) % (NUM_ARM_JOINT));
		break;
	case 'x':
		armControl(CONTROL_PARAM1, -5);
		break;
	case 'c':
		armControl(CONTROL_PARAM1, 5);
		break;
	case 'v':
		armControl(CONTROL_PARAM2, -5);
		break;
	case 'b':
		armControl(CONTROL_PARAM2, 5);
		break;
	case 'n':
		armControl(CONTROL_PARAM3, -5);
		break;
	case 'm':
		armControl(CONTROL_PARAM3, 5);
		break;
	case ',':
		temp = (int)armControl(WHICH_IS_CONTROL,0);
		armControl(CHANGE_CONTROL_PART,NUM_ARM_JOINT);
		armControl(CONTROL_PARAM1, -5);
		armControl(CHANGE_CONTROL_PART,temp);
		break;
	case '.':
		temp = (int)armControl(WHICH_IS_CONTROL,0);
		armControl(CHANGE_CONTROL_PART,NUM_ARM_JOINT);
		armControl(CONTROL_PARAM1, 5);
		armControl(CHANGE_CONTROL_PART,temp);
		break;
	case 'j':
		cameraControl(UPDATE_EYE_Z, 5 * signVal);
		break;
	case 'e':
		armControl(RESET_ARM_PARAM, 0);
		break;
		case 'w':
		cameraControl(SET_MODE_CAMERA,NOT_CAMERA_CONTROL_MODE + ((1+ (int)cameraControl(GET_MODE_CAMERA,0)-NOT_CAMERA_CONTROL_MODE)%2));
		break;
	case 'r':
		cameraControl(RESET_CAMERA_POS, 0);
		break;
	case 'f':
		cameraControl(UPDATE_CENT_X, signVal);
		break;
	case 'g':
		cameraControl(UPDATE_CENT_Y, signVal);
		break;
	case 'h':
		cameraControl(UPDATE_CENT_Z, signVal);
		break;//UPDATE_UP_Z
	case 't':
		cameraControl(UPDATE_UP_X, signVal);
		break;
	case 'y':
		cameraControl(UPDATE_UP_Y, signVal);
		break;
	case 'u':
		cameraControl(UPDATE_UP_Z, signVal);
		break;
	case '-':
		signVal = -1;
		break;
	case 'i':
		changeLight = (changeLight + 1) % 5;
		setColorLight(colorSet[changeLight]);
		break;
	case 'o':
		enableCullBack = (enableCullBack + 1) % 2;
		break;
	case 'p':
		enableWireFrame = (enableWireFrame + 1) % 2;
		break;
	case '+':
		signVal = 1;
		break;
		case '0':
			if(pageStatus != CAMERA_SETTING_INFO)
			pageStatus = CAMERA_SETTING_INFO;
			else
			pageStatus = HOME_INFO;
		break;
		case '9':
			if(pageStatus != ROBOT_SETTING_INFO)
			pageStatus = ROBOT_SETTING_INFO;
			else
			pageStatus = HOME_INFO;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void reshape(GLint w, GLint h)
{

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, ((GLfloat)w) / ((GLfloat)h), 1.0, 150.0);

	
/*
	 GLfloat aspectRatio;
 glViewport(0, 0, w, h);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 aspectRatio = (GLfloat) w / (GLfloat) h;
 if (w <= h)
 glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, 1.0, -1.0);
 else
 glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, 1.0, -1.0);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 */

}
void init()
{
	// for random
	srand(time(0));
	// Set the current clear color to black and the current drawing color to 
	// white
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(1.0, 0.5, 1.0);


	glEnable(GL_DEPTH_TEST);
	setColorLight(colorSet[0]);

	// Set the camera lens to have a 60 degree (vertical) field of view, an
	// aspect ratio of 4/3, and have everything closer than 1 unit to the
	// camera and greater than 40 units distant clipped away.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 4.0 / 3.0, 1, 40);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cameraControl(SET_MODE_CAMERA,CAMERA_CONTROL_MODE);
	cameraControl(RESET_CAMERA_POS, 0);
	cameraControl(INITIAL_CAMERA, 0);
	cameraControl(SET_MODE_CAMERA,NOT_CAMERA_CONTROL_MODE);



	//loadObjToVBuffs(&aModelLoader);
}

// Initializes GLUT, the display mode, and main window; registers callbacks;
// does application initialization; enters the main event loop.
int main(int argc, char** argv)
{
	constructorModelLoader(&aModelLoader, "Scene/robotArm.obj", "texture/robottx.jpg");
	constructorModelLoader(&aJointPart, "Scene/jointPoint.obj", "texture/robottx.jpg");
	constructorModelLoader(&aFinger, "Scene/afinger.obj", "texture/robottx.jpg");
	loadObj(&aModelLoader);
	loadObj(&aJointPart);
	loadObj(&aFinger);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowPosition(80, 80);
	glutInitWindowSize(800, 600);
	glutCreateWindow("A Long Robot Arm");
	glutTimerFunc(100, timer, 0);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(special);
	glutKeyboardFunc(keyboard);
	init();

	glutMainLoop();

	return EXIT_SUCCESS;
}


void constructorModelLoader(struct modelLoader* obj, const char* objFilename, const char* imgFile)
{
	int objfilenameLen = strlen(objFilename);
	int imgfilenameLen = strlen(imgFile);
	obj->numVn = 0;
	obj->numVt = 0;
	obj->numV = 0;
	obj->numF = 0;
	memset(obj->objFilename, 0, objfilenameLen + 1);
	memset(obj->imageFile, 0, imgfilenameLen + 1);
	memcpy(obj->objFilename, objFilename, objfilenameLen);
	memcpy(obj->imageFile, imgFile, imgfilenameLen);

}
void loadObj(struct modelLoader* obj)
{
	FILE* fp = fopen(obj->objFilename, "r");
	if ((obj->objFilename[0] != 0) && (fp != NULL))
	{
		char buff[200];
		memset(buff, 0, sizeof(buff));
		while (fgets(buff, sizeof(buff), fp) != NULL)
		{
			//printf("%s",buff); 
			if ((buff[0] == 'v') && (buff[1] == 't'))  // vector texture
			{
				sscanf(&buff[3], "%f %f\n", &(obj->vt[obj->numVt].dat[0]), &(obj->vt[obj->numVt].dat[1]));
				obj->numVt++;
			}
			else if ((buff[0] == 'v') && (buff[1] == 'n'))  // vector normal
			{
				sscanf(&buff[3], "%f %f %f\n", &(obj->vn[obj->numVn].dat[0]), &(obj->vn[obj->numVn].dat[1]), &(obj->vn[obj->numVn].dat[2]));
				obj->numVn++;
			}
			else if (buff[0] == 'v')  // vector 
			{
				sscanf(&buff[2], "%f %f %f\n", &(obj->v[obj->numV].dat[0]), &(obj->v[obj->numV].dat[1]), &(obj->v[obj->numV].dat[2]));
				obj->numV++;
			}
			else if (buff[0] == 'f')
			{
				sscanf(&buff[2], "%d/%d/%d %d/%d/%d %d/%d/%d\n", &(obj->f[obj->numF].x), &(obj->f[obj->numF].z), &(obj->f[obj->numF].y), &(obj->f[obj->numF + 1].x), &(obj->f[obj->numF + 1].z), &(obj->f[obj->numF + 1].y), &(obj->f[obj->numF + 2].x), &(obj->f[obj->numF + 2].z), &(obj->f[obj->numF + 2].y));
				obj->f[obj->numF].x--;
				obj->f[obj->numF].y--;
				obj->f[obj->numF].z--;
				obj->f[obj->numF + 1].x--;
				obj->f[obj->numF + 1].y--;
				obj->f[obj->numF + 1].z--;
				obj->f[obj->numF + 2].x--;
				obj->f[obj->numF + 2].y--;
				obj->f[obj->numF + 2].z--;
				obj->numF += 3;
			}
		}
		fclose(fp);
	}
	else
	{
		printf("Error file to load OBJ model was not found %s\n",obj->objFilename);
	}
}


void drawModel(struct modelLoader* obj)
{
	int index = 0;
	int index2 = 0;
	int indexV = 0;
	int indexVt = 0;
	int indexVn = 0;
	int num = obj->numF;
	for (index = 0; index < num; index += 3)
	{
		glBegin(GL_TRIANGLES);

		for (index2 = 0; index2 < 3; index2++)
		{
			indexV = obj->f[index + index2].x;
			indexVt = obj->f[index + index2].z;
			indexVn = obj->f[index + index2].y;
			glNormal3fv(obj->vn[indexVn].dat);
			glTexCoord2f(obj->vt[indexVt].dat[0], obj->vt[indexVt].dat[1]);
			glVertex3fv(obj->v[indexV].dat);
		}
		glEnd();
	}
}

void loadTexture(struct modelLoader* obj)
{

	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(obj->imageFile, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		//	glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{

	}
	stbi_image_free(data);
}
void drawRobotArm(struct armControlParameter* obj)
{
	int i = 0;

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// set the same texture to the long robot's arm
	loadTexture(&aModelLoader);	glPushMatrix();

	for (i = 0; i < NUM_ARM_JOINT; i++)
	{
		if((i == (int)armControl(WHICH_IS_CONTROL,0) && (armControl(IS_AUTOMOVE_ARM,0) == 0))) setColorLight(colorSet[5]);
		else if((i-1) == (int)armControl(WHICH_IS_CONTROL,0)) setColorLight(colorSet[changeLight]);
		glTranslatef(0.0, 0.0, 3.0);
		drawModel(&aJointPart);
		glRotatef((GLfloat)armParam.jointRadian[i][0], 1.0, 0.0, 0.0);
		glRotatef((GLfloat)armParam.jointRadian[i][1], 0.0, 1.0, 0.0);
		glRotatef((GLfloat)armParam.jointRadian[i][2], 0.0, 0.0, 1.0);
		glTranslatef(0.0, 0.0, 3.0);
		drawModel(&aModelLoader);

	}

	if(((int)armControl(WHICH_IS_CONTROL,0) == (NUM_ARM_JOINT-1) && (armControl(IS_AUTOMOVE_ARM,0) == 0))) setColorLight(colorSet[changeLight]);	
	glTranslatef(0.0, 0.0, 3.0);
	drawModel(&aJointPart);
	glRotatef((GLfloat)armParam.jointRadian[NUM_ARM_JOINT][0], 1.0, 0.0, 0.0);
	glRotatef((GLfloat)armParam.jointRadian[NUM_ARM_JOINT][1], 0.0, 1.0, 0.0);
	glRotatef((GLfloat)armParam.jointRadian[NUM_ARM_JOINT][2], 0.0, 0.0, 1.0);
	glPushMatrix();
	glTranslatef(-0.3, 2.0 - armParam.fingersWidth, 2.0);
	glRotatef((GLfloat)-185, 0.0, 1.0, 1.0);

	drawModel(&aFinger);
	glPopMatrix();
	// Turn off wireframe mode

	glTranslatef(-0.3, -2.0 + armParam.fingersWidth, 2.0);
	glRotatef((GLfloat)90, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)5.0, 0.0, 0.0, 1.0);
	drawModel(&aFinger);

	glPopMatrix();

  	glDisable(GL_TEXTURE_2D);



}

double cameraControl(int controlParameter, int controlParameter2)
{
	double ret = 0;
	int i = 0;
	static int isAutomave = 0;
	static int isAbleToControl = NOT_CAMERA_CONTROL_MODE;
	static GLfloat u = 0.0;
	static GLdouble eye[3];
	static GLdouble center[3];
	static GLdouble up[3];
	//UPDATE_UP_X
		// read parameter
	if(controlParameter == SET_MODE_CAMERA)
	{
		isAbleToControl = controlParameter2;
	}
	else if (controlParameter == GET_MODE_CAMERA)
	{
		ret = isAbleToControl;
	}
	else if ((inRange(UPDATE_EYE_Z, UPDATE_EYE_X, controlParameter) == 1) && (isAbleToControl == CAMERA_CONTROL_MODE))
	{
		i = controlParameter - UPDATE_EYE_X;
		eye[i] = boundValue(100, -50, eye[i] + controlParameter2);
		ret = eye[i];
	}
	else if ((inRange(UPDATE_CENT_Z, UPDATE_CENT_X, controlParameter) == 1) && (isAbleToControl == CAMERA_CONTROL_MODE))
	{

		i = controlParameter - UPDATE_CENT_X;
		center[i] = boundValue(100, -50, center[i] + controlParameter2);
		ret = center[i];
	}
	else if ((inRange(UPDATE_UP_Z, UPDATE_UP_X, controlParameter) == 1) && (isAbleToControl == CAMERA_CONTROL_MODE))
	{

		i = controlParameter - UPDATE_UP_X;
		up[i] = boundValue(100, -50, up[i] + controlParameter2);
		ret = up[i];
	}
	else if ((controlParameter == INITIAL_CAMERA) && (isAutomave == 0))
	{
		glMatrixMode(GL_MODELVIEW);
	}
	else if ((controlParameter == RESET_CAMERA_POS) && (isAbleToControl == CAMERA_CONTROL_MODE))
	{
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		for (i = 0; i < 3; i++)
		{
			center[0] = 0;
			eye[i] = 0;
			up[i] = 0;
		}
		center[2] = -1;
		up[1] =1;
//eye[2] = -10;
			eye[2] = (NUM_ARM_JOINT+2)*5;
		u = 0;
	}
	else if ((controlParameter == ENABLE_AUTOMOVE_CAMERA) && (isAbleToControl == CAMERA_CONTROL_MODE))
	{
		isAutomave = 1;
	}
	else if ((controlParameter == DISABLE_AUTOMOVE_CAMERA) && (isAbleToControl == CAMERA_CONTROL_MODE))
	{
		isAutomave = 0;
	}
	else if (controlParameter == IS_AUTOMOVE_CAMERA)
	{
		ret = isAutomave;
	}


	if ((isAutomave == 1) && (isAbleToControl == CAMERA_CONTROL_MODE))
	{
		u += 0.01;
	}
	else
	{

	}
	if ((isAbleToControl == CAMERA_CONTROL_MODE)|| (controlParameter == INITIAL_CAMERA))
	{
		glLoadIdentity();
		gluLookAt(10 * cos(u) + eye[0], 10 * cos(u) + eye[1], eye[2] + 40 * cos(u / 3) + 2, center[0], center[1], center[2], cos(u) + up[0], up[1], up[2]);
		glutPostRedisplay();	
	}

	return ret;
}
double armControl(int controlParameter, int controlParameter2)
{
	double ret = 0.0;
	int i = 0;
	int j = 0;
	int k = 0;
	static int whichPartIsControled = CONTROL_JOINT1;
	static int isAutomave = 0;
	//armParam

	if ((controlParameter == INITIAL_CAMERA) && (isAutomave == 0))
	{
		//glMatrixMode(GL_MODELVIEW);
	}
	else if (controlParameter == ENABLE_AUTO_ARM_MOVE)
	{
		isAutomave = 1;
	}
	else if (controlParameter == DISABLE_AUTO_ARM_MOVE)
	{
		isAutomave = 0;
	}
	else if (controlParameter == IS_AUTOMOVE_ARM)
	{
		ret = isAutomave;
	}
	else if (controlParameter == WHICH_IS_CONTROL)
	{
		ret = whichPartIsControled;
	}
	else if ((controlParameter == CHANGE_CONTROL_PART) && (isAutomave == 0))
	{
		whichPartIsControled = controlParameter2;
	}
	else if ((controlParameter >= CONTROL_PARAM1) && (controlParameter <= CONTROL_PARAM3) && (isAutomave == 0))
	{
		if ((whichPartIsControled >= CONTROL_JOINT1) && (whichPartIsControled <= (NUM_ARM_JOINT-1)))
		{
			j = whichPartIsControled - CONTROL_JOINT1;
			i = controlParameter - CONTROL_PARAM1;
			armParam.jointRadian[j][i] = (float)(((int)armParam.jointRadian[j][i] + (int)controlParameter2) % 360);
		}
		else if ((whichPartIsControled == CONTROL_FINGER) && (controlParameter == CONTROL_PARAM1))
		{
			armParam.fingersWidth = boundValue(1.2, 0.0, armParam.fingersWidth + controlParameter2 / 50.0);
		}
	}
	else if (controlParameter == RESET_ARM_PARAM)
	{
		whichPartIsControled = CONTROL_JOINT1;
		armParam.fingersWidth = 0.0;
		for (i = 0; i < 3; i++)
			for (j = 0; j < (NUM_ARM_JOINT+1); j++)
				armParam.jointRadian[j][i] = 0.0;

	}
	if ((isAutomave == 1) && (controlParameter == ROBOT_TIMER_UPDATE))
	{

		armParam.fingersWidth = boundValue(1.2, 0.0, armParam.fingersWidth - 0.3 + 0.1 * (rand() % 7));
		for (i = 0; i < 3; i++)
			for (j = 0; j < (NUM_ARM_JOINT+1); j++)
			{
				//k = rand()%2;
				k = ((((i + j) % 2) == 0) - (((i + j) % 2) != 0));
				armParam.jointRadian[j][i] = (float)(((int)armParam.jointRadian[j][i] + k * (1 - (int)rand % 2)) % 360);
			}
		//		glutPostRedisplay();
	}

	return ret;
}

void setColorLight(GLfloat* color)
{
	GLfloat direction[] = { 1.0,1.0,1.0,0.0 };
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	glLightfv(GL_LIGHT0, GL_POSITION, direction);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

int inRange(const double max, const double min, const double value)
{
	int ret = 0;
	if ((value <= max) && (value >= min))
	{
		ret = 1;
	}
	return ret;
}
double boundValue(const double max, const double min, const double original)
{
	double ret = original;
	if (original > max)ret = max;
	if (original < min)ret = min;
	return ret;
}
void displayInterfaceText()
{
		int i = 0;
		char buff[200];
		memset(buff,0,200);
			  glPushAttrib(GL_ENABLE_BIT);
		  glDisable(GL_DEPTH_TEST);
		  glDisable(GL_LIGHTING);
		  
		  glMatrixMode(GL_PROJECTION);
		  glPushMatrix();
		  glLoadIdentity();
		  gluOrtho2D(0, 3000, 0, 3000);

		  glMatrixMode(GL_MODELVIEW);
		  glPushMatrix();
		  glLoadIdentity();

		  if(pageStatus == HOME_INFO)
		  {
				output(80, 2800-(i++)*150, "q : Quit");
				output(80, 2800-(i++)*150, "w : Enter/leave control camera mode");
				output(80, 2800-(i++)*150, "   Show/hide Info");
				output(80, 2800-(i++)*150, "e : Reset Robot arm's postion");
				output(80, 2800-(i++)*150, "s : Robot arm autopilot/control mode");
				output(80, 2800-(i++)*150, "i : Light setting");
				sprintf(buff,"o : On/off face back culling (%s)",(enableCullBack==1)?"on":"off");
				output(80, 2800-(i++)*150, buff);
				output(80, 2800-(i++)*150, "p : On/off wire frame");
				output(80, 2800-(i++)*150, "9 : Robot arm advanced setting info");
				output(80, 2800-(i++)*150, "0 : Camera advanced setting info"); 	
		  }
		  else if (pageStatus == CAMERA_SETTING_INFO)
		  {
				output(80, 2800-(i++)*150, "0   : Main Info");
				output(80, 2800-(i++)*150, "9   : Robot arm advanced setting info");
				output(80, 2800-(i++)*150, "Up/down: Eye Y");
				output(80, 2800-(i++)*150, "Left/right: Eye X");
				output(80, 2800-(i++)*150, "-/+ : Increase/decrease parameters below");
				output(80, 2800-(i++)*150, "f   : Center X");
				output(80, 2800-(i++)*150, "g   : Center Y");
				output(80, 2800-(i++)*150, "h   : Center Z");
				output(80, 2800-(i++)*150, "j   : Eye Z");
				output(80, 2800-(i++)*150, "t   : Up vector X");
				output(80, 2800-(i++)*150, "y   : Up vector Y");
				output(80, 2800-(i++)*150, "u   : Up vector Z");

		  }
		  else if (pageStatus == ROBOT_SETTING_INFO)
		  {
				output(80, 2800-(i++)*150, "9 : Main Info");
				output(80, 2800-(i++)*150, "0 : Camera advanced setting info");
				output(80, 2800-(i++)*150, "In robot munual control mode");
				output(80, 2800-(i++)*150, "   z : Select the green arm to rotate");
				output(80, 2800-(i++)*150, "       Shadow setting");
				output(80, 2800-(i++)*150, " x/c :Rotate X-axis");
				output(80, 2800-(i++)*150, " v/b :Rotate Y-axis");
				output(80, 2800-(i++)*150, " n/m :Rotate Z-axis");
				output(80, 2800-(i++)*150, " ,/. :Control fingers");
		  }
		  glPopMatrix();
		  glMatrixMode(GL_PROJECTION);
		  
		  glPopMatrix();

		  glPopAttrib();
}
