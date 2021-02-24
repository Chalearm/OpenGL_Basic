

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <time.h>

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define V_ARRAY_SIZE 30000
#define VT_ARRAY_SIZE 30000
#define VN_ARRAY_SIZE 30000
#define F_ARRAY_SIZE 30000

#define FILENAME_SIZE 40

// Robot control
#define DISABLE_AUTO_ARM_MOVE 1
#define ENABLE_AUTO_ARM_MOVE 2
#define IS_AUTOMOVE_ARM 3
#define CONTROL_JOINT1 5
#define CONTROL_JOINT2 6
#define CONTROL_JOINT3 7
#define CONTROL_JOINT4 8
#define CONTROL_JOINT5 9
#define CONTROL_FINGER 10
#define WHICH_IS_CONTROL 11
#define CHANGE_CONTROL_PART 12
#define CONTROL_PARAM1 13
#define CONTROL_PARAM2 14
#define CONTROL_PARAM3 15
#define RESET_ARM_PARAM 16

// Camera control
#define DISABLE_AUTOMOVE_CAMERA 1
#define ENABLE_AUTOMOVE_CAMERA 2
#define IS_AUTOMOVE_CAMERA 3
#define INITIAL_CAMERA 4
#define RESET_CAMERA_POS 5
#define UPDATE_EYE_X 6
#define UPDATE_EYE_Y 7
#define UPDATE_EYE_Z 8

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

	//  render data
	unsigned int VAO, VBO, EBO;
};
struct armControlParameter
{
	double jointRadian[5][3]; // x,y ,z

	//double jointHandRad[3];
	double fingersWidth;
};
void constructorModelLoader(struct modelLoader* obj, const char* objFilename, const char* imgFile);
void loadObj(struct modelLoader* obj);
void loadTexture(struct modelLoader* obj);
void drawModel(struct modelLoader* obj);
double cameraControl(int controlParameter,int controlParameter2);
double armControl(int controlParameter,int controlParameter2);
void drawRobotArm(struct armControlParameter *obj);
double boundValue(const double max,const double min,const double original);
void setColorLight(GLfloat *color);
struct modelLoader aModelLoader;
struct modelLoader aJointPart;
struct modelLoader aFinger;
struct armControlParameter armParam;
float g_rotation = 0;
float g_rotation2 = 0;
float g_xDistance = 6;
float g_yDistance = 6;
float g_fingersEnlarge = 0;
int enableWireFrame = 0;
int enableCullBack = 0;
int changeLight = 0;

#define white  { 1.0,1.0,1.0,1.0 }
#define yellow  { 1.0,1.0,0.0,1.0 }
#define cyan  { 0.0,1.0,1.0,1.0 }
#define red  {1.0,0.0,0.0,1.0}
#define blue  {0.0,0.0,1.0,1.0}
GLfloat colorSet[][4] = {white,yellow,cyan,red,blue};
float kk1 = 0;
float kk2 = 0;
float kk3 = 0;
float kk4 = 0;
float kk5 = 0;
float kk6 = 0;

int signVal = 1;

// Clears the window and draw the torus.
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	drawRobotArm(&armParam);

	cameraControl(INITIAL_CAMERA,0);
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
}
// Advances the orbiter and requests to draw the next frame.
void timer(int v)
{

	cameraControl(0,0); 
	armControl(0,0);
	glutPostRedisplay();
	glutTimerFunc(1000/60.0, timer,v);
}
void special(int key, int a,int b)
{
	switch (key)
	{
		case GLUT_KEY_LEFT : 
		//moveLeftCamera(&aCamera);
		cameraControl(UPDATE_EYE_X,-5);
		break;
		case GLUT_KEY_RIGHT:
		cameraControl(UPDATE_EYE_X,5);
		break;
		case GLUT_KEY_UP:
		cameraControl(UPDATE_EYE_Y,-5);
		break;
		case GLUT_KEY_DOWN:
		cameraControl(UPDATE_EYE_Y,5);
		break;
	}
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
	case 'q':
		exit(EXIT_SUCCESS);
		break;
	case 'a':
		cameraControl(DISABLE_AUTOMOVE_CAMERA+(((int)cameraControl(IS_AUTOMOVE_CAMERA,0)+1)%2),0);
		break;
	case 's':
		armControl(DISABLE_AUTO_ARM_MOVE+(((int)armControl(IS_AUTOMOVE_ARM,0)+1)%2),0);	
	break;
	case 'z':
		armControl(CHANGE_CONTROL_PART, 5+((int)armControl(WHICH_IS_CONTROL,0)-4)%6);
		break;
	case 'x':
		armControl(CONTROL_PARAM1,-5);
		break;
	case 'c':
		armControl(CONTROL_PARAM1,5);
		break;
	case 'v':
		armControl(CONTROL_PARAM2,-5);
		break;
	case 'b':
		armControl(CONTROL_PARAM2,5);
		break;
	case 'n':
		armControl(CONTROL_PARAM3,-5);
		break;
	case 'm':
		armControl(CONTROL_PARAM3,5);
		break;
	case 'h':
		 cameraControl(UPDATE_EYE_Z,5*signVal);
		break;
	case 'e':
		armControl(RESET_ARM_PARAM,0);
		break;
	case 'r':
		cameraControl(RESET_CAMERA_POS,0);
		break;
	case 'f':
		kk1 = (float)(((int)kk1 + 5 * signVal) % 360);
		break;
	case 'g':
		kk2 = (float)(((int)kk2 + 5 * signVal) % 360);
		break;
	case 'd':
		kk3 = (float)(((int)kk3 + 5 * signVal) % 360);
		break;
	case 't':
		kk4 = kk4 + 0.1 * signVal;
		break;
	case 'y':
		kk5 = kk5 + 0.1 * signVal;
		break;
	case 'u':
		kk6 = kk6 + 0.1 * signVal;
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
	default:
		break;
	}
	printf("kk(%f,%f,%f, %f, %f,%f)\n", kk1, kk2, kk3, kk4, kk5, kk6);
	glutPostRedisplay();
}
void reshape(GLint w, GLint h)
{

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, ((GLfloat)w) / ((GLfloat)h), 1.0, 150.0);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// set the same texture to the long robot's arm
	loadTexture(&aModelLoader);
	//loadTexture(&aJointPart);
	//loadTexture(&aFinger);

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
	// glEnable( GL_COLOR_MATERIAL );
   //  glShadeModel( GL_SMOOTH );
	// glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );

	 // Set the camera lens to have a 60 degree (vertical) field of view, an
	 // aspect ratio of 4/3, and have everything closer than 1 unit to the
	 // camera and greater than 40 units distant clipped away.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, 4.0 / 3.0, 1, 40);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	cameraControl(INITIAL_CAMERA,0);
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
void drawRobotArm(struct armControlParameter *obj)
{
	int i = 0;
	glPushMatrix();
 
	for(i=0; i < 4;i++)
	{

		glTranslatef(0.0, 0.0, 3.0);
		drawModel(&aJointPart);
		glRotatef((GLfloat)armParam.jointRadian[i][0], 1.0, 0.0, 0.0);
		glRotatef((GLfloat)armParam.jointRadian[i][1], 0.0, 1.0, 0.0);
		glRotatef((GLfloat)armParam.jointRadian[i][2], 0.0, 0.0, 1.0);	
		glTranslatef(0.0, 0.0, 3.0);
		drawModel(&aModelLoader);

	}

	glTranslatef(0.0, 0.0, 3.0);
	drawModel(&aJointPart);
	glRotatef((GLfloat)armParam.jointRadian[4][0], 1.0, 0.0, 0.0);
	glRotatef((GLfloat)armParam.jointRadian[4][1], 0.0, 1.0, 0.0);
	glRotatef((GLfloat)armParam.jointRadian[4][2], 0.0, 0.0, 1.0);



	glPushMatrix();
	glTranslatef(-0.3 - kk4, 1.9 - armParam.fingersWidth , 2.0 - kk6);
	//glRotatef((GLfloat)kk1,0.0,1.0,0.0);
	//glRotatef((GLfloat)kk2,0.0,0.0,1.0);
	glRotatef((GLfloat)-185, 0.0, 1.0, 1.0);

	//glRotatef((GLfloat)kk2,kk4,kk5,kk6);
	drawModel(&aFinger);
	glPopMatrix();
	// Turn off wireframe mode

	glTranslatef(-0.3 + kk4, -2.0 + armParam.fingersWidth , 2.0 + kk6);
	glRotatef((GLfloat)90, 1.0, 0.0, 0.0);
	glRotatef((GLfloat)5.0, 0.0, 0.0, 1.0);

	//glRotatef((GLfloat)kk1,kk4,kk5,kk6);
	drawModel(&aFinger);

	glPopMatrix();


}

double cameraControl(int controlParameter,int controlParameter2)
{
	double ret = 0;
	static int isAutomave = 0;
	static GLfloat u = 0.0;
	static GLdouble eyex = 0.0;
	static GLdouble eyey = 0.0; 
	static GLdouble eyez = 0.0; 

	// read parameter
	if (controlParameter == UPDATE_EYE_X)
	{
		eyex = boundValue(100,-50,eyex+controlParameter2);	
		printf("eyex :%f \n",eyex);
		ret = eyex;
	}
	else if (controlParameter == UPDATE_EYE_Y)
	{
		eyey = boundValue(100,-50,eyey+controlParameter2);		
		printf("eyey :%f \n",eyey);
		ret = eyey;
	}
	else if (controlParameter == UPDATE_EYE_Z)
	{
		eyez = boundValue(100,-50,eyez+controlParameter2);
		ret = eyez;
	}
	else if ((controlParameter == INITIAL_CAMERA) && (isAutomave == 0))
	{	
		glMatrixMode(GL_MODELVIEW);
	}
	else if(controlParameter == RESET_CAMERA_POS)
	{
		eyex = 0;
		eyey = 0;
		eyez = 0;
		u = 0;
	}
	else if(controlParameter == ENABLE_AUTOMOVE_CAMERA)
	{
		isAutomave = 1;
	}
	else if(controlParameter == DISABLE_AUTOMOVE_CAMERA)
	{
		isAutomave = 0;
	}
	else if(controlParameter == IS_AUTOMOVE_CAMERA)
	{
		ret = isAutomave;
	}


	if (isAutomave == 1)
	{
		u += 0.01;
	}
	else
	{

	}
	glLoadIdentity();
	gluLookAt(10*cos(u)+eyex,10*cos(u)+eyey,eyez+40*cos(u/3)+2,kk1,kk2,kk3,cos(u)+kk4,1+kk5,0+kk6);
	glutPostRedisplay();
	return ret;
}
double armControl(int controlParameter,int controlParameter2)
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
	else if(controlParameter == ENABLE_AUTO_ARM_MOVE)
	{
		isAutomave = 1;
	}
	else if(controlParameter == DISABLE_AUTO_ARM_MOVE)
	{
		isAutomave = 0;
	}
	else if(controlParameter == IS_AUTOMOVE_ARM)
	{
		ret = isAutomave;
	}
	else if(controlParameter == WHICH_IS_CONTROL)
	{
		ret = whichPartIsControled;
	}
	else if(controlParameter == CHANGE_CONTROL_PART)
	{
		whichPartIsControled = controlParameter2;
	}
	else if((controlParameter >= CONTROL_PARAM1) && (controlParameter <= CONTROL_PARAM3))
	{
		if((whichPartIsControled >= CONTROL_JOINT1) && (whichPartIsControled <= CONTROL_JOINT5))
		{
			j = whichPartIsControled-CONTROL_JOINT1;
			i = controlParameter - CONTROL_PARAM1;
			armParam.jointRadian[j][i] =(float) (((int)armParam.jointRadian[j][i] +(int)controlParameter2)%360);
		}
		else if ((whichPartIsControled == CONTROL_FINGER) &&(controlParameter == CONTROL_PARAM1))
		{
			armParam.fingersWidth = boundValue(1.2,0.0,armParam.fingersWidth+ controlParameter2/50.0);
		}
		printf("whihci part : %d \n",(whichPartIsControled-CONTROL_JOINT1));
	}
	else if (controlParameter == RESET_ARM_PARAM)
	{
		armParam.fingersWidth = 0.0;
		for(i = 0;i <3 ;i++)
			for(j=0;j<5;j++)
				armParam.jointRadian[j][i] =0.0;

	}
	if(isAutomave == 1)
	{

		armParam.fingersWidth = boundValue(1.2,0.0,armParam.fingersWidth-0.3+0.1*(rand()%7));
		for(i = 0;i <3 ;i++)
			for(j=0;j<5;j++)
			{
				//k = rand()%2;
				k = ((((i+j)%2)==0) - (((i+j)%2)!=0));
				armParam.jointRadian[j][i] =(float) (((int)armParam.jointRadian[j][i] + k*(6 - (int)rand%12))%360);
			}
	//		glutPostRedisplay();
	}

	return ret;
}

void setColorLight(GLfloat *color)
{
	GLfloat direction[] = { 1.0,1.0,1.0,0.0 }; 
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, color);
	glMaterialf(GL_FRONT, GL_SHININESS, 50);
	//glLightfv(GL_LIGHT0,GL_AMBIENT,black);
	//glLightfv(GL_LIGHT0,GL_DIFFUSE,yellow);
	glLightfv(GL_LIGHT0, GL_SPECULAR, color);
	glLightfv(GL_LIGHT0, GL_POSITION, direction);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}
double boundValue(const double max,const double min,const double original)
{
	double ret  =original;
	if(original > max)ret = max;
	if(original < min)ret = min;
	return ret;
}

