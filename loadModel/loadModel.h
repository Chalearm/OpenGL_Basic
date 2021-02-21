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
#define __gl_h_
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif
#ifdef __APPLE__
#define glGenVertexArrays glGenVertexArraysAPPLE
#define glBindVertexArray glBindVertexArrayAPPLE
#define glDeleteVertexArrays glDeleteVertexArraysAPPLE
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

void constructorModelLoader(struct modelLoader *obj,const char *objFilename,const char *imgFile,const char *mtlFilename);
void loadObj(struct modelLoader *obj);
void loadTexture(struct modelLoader *obj);
void loadObjToVBuffs(struct modelLoader *obj);
void drawModel(struct modelLoader *obj);
void renderModel(struct modelLoader *obj);