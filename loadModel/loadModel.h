/*


	Developer : Chalearm Saelim
	Purpose   : this lib is for reading the mesh and texture from Blender model file (*.obj or *.mtl) 

	this is C structure.
	
*/

#include <stdio.h>
#include <string.h>
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
#define V_ARRAY_SIZE 40000
#define VT_ARRAY_SIZE 40000
#define VN_ARRAY_SIZE 40000
#define F_ARRAY_SIZE 40000

#define FILENAME_SIZE 40

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

	struct vector3Df v[V_ARRAY_SIZE];
	int numV;

	struct vector2Df vt[VT_ARRAY_SIZE];
	int numVt;

	struct vector3Df vn[VN_ARRAY_SIZE];
	int numVn;

	struct vector3DI f[F_ARRAY_SIZE];
	int numF;
};

void constructorModelLoader(struct modelLoader *obj,const char *objFilename,const char *imgFile,const char *mtlFilename);
void loadObj(struct modelLoader *obj);
void loadTexture(struct modelLoader *obj);
void drawModel(struct modelLoader *obj);