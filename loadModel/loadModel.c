/*


	Developer : Chalearm Saelim
	Purpose   : this lib is for reading the mesh and texture from Blender model file (*.obj or *.mtk) 

	
*/

#include "loadModel.h"


void constructorModelLoader(struct modelLoader *obj,const char *objFilename,const char *mtlFilename)
{
	int objfilenameLen = strlen(objFilename);
	int mtlfilenameLen = strlen(mtlFilename);
	obj->numVn = 0;
	obj->numVt = 0;
	obj->numV = 0;
	obj->numF = 0;
	memset(obj->objFilename,0,objfilenameLen+1);
	memset(obj->mtlFilename,0,mtlfilenameLen+1);
	memcpy(obj->objFilename,objFilename,objfilenameLen);
	memcpy(obj->mtlFilename,mtlFilename,mtlfilenameLen);

}
void loadObj(struct modelLoader *obj)
{
	FILE *fp = fopen(obj->objFilename,"r");
	char buff[200];
	while (fgets(buff, sizeof(buff), fp) != NULL)
	{
		//printf("%s",buff); 
		if ((buff[0] == 'v') && (buff[1] == 't'))  // vector texture
		{
			sscanf(&buff[3],"%f %f\n",&(obj->vt[obj->numVt].dat[0]),&(obj->vt[obj->numVt].dat[1]));
			//printf("vtj %f %f\n",obj->vt[obj->numVt].dat[0],obj->vt[obj->numVt].dat[1]);
			obj->numVt++;
		}
		else if((buff[0] == 'v') && (buff[1] == 'n'))  // vector normal
		{
			sscanf(&buff[3],"%f %f %f\n",&(obj->vn[obj->numVn].dat[0]),&(obj->vn[obj->numVn].dat[1]),&(obj->vn[obj->numVn].dat[2]));
			//printf("vnj %f %f %f\n",obj->vn[obj->numVn].dat[0],obj->vn[obj->numVn].dat[1],obj->vn[obj->numVn].dat[2]);
			obj->numVn++;
		}
		else if(buff[0] == 'v')  // vector 
		{
			sscanf(&buff[2],"%f %f %f\n",&(obj->v[obj->numV].dat[0]),&(obj->v[obj->numV].dat[1]),&(obj->v[obj->numV].dat[2]));
			//printf("vj %f %f %F\n",obj->v[obj->numV].dat[0],obj->v[obj->numV].dat[1],obj->v[obj->numV].datp2]);
			obj->numV++;
		}
		else if(buff[0]=='f')
		{
			sscanf(&buff[2],"%d/%d/%d %d/%d/%d %d/%d/%d\n",&(obj->f[obj->numF].x),&(obj->f[obj->numF].y),&(obj->f[obj->numF].z),&(obj->f[obj->numF+1].x),&(obj->f[obj->numF+1].y),&(obj->f[obj->numF+1].z),&(obj->f[obj->numF+2].x),&(obj->f[obj->numF+2].y),&(obj->f[obj->numF+2].z));
			//printf("fj %d/%d/%d %d/%d/%d %d/%d/%d\n",obj->f[obj->numF].x,obj->f[obj->numF].y,obj->f[obj->numF].z,obj->f[obj->numF+1].x,obj->f[obj->numF+1].y,obj->f[obj->numF+1].z,obj->f[obj->numF+2].x,obj->f[obj->numF+2].y,obj->f[obj->numF+2].z);
			obj->numF += 3;
		}
	}

	fclose(fp);
}


void drawModel(struct modelLoader *obj)
{

	int index = 0;
	int index2 =0;
	int indexV = 0;
	int indexVt = 0;
	int indexVn = 0;
	int num = obj->numF;
	for (index = 0 ;index < num;index += 3)
	{
		glBegin( GL_TRIANGLES);

		for(index2 = 0;index2 <3;index2++)
		{		
			indexV = obj->f[index+index2].x -1;
			indexVt = obj->f[index+index2].y -1;
			indexVn = obj->f[index+index2].z -1;
			//glColor3f(0.5+(float)index/200.0,0.2,0.1+(float)index/100.0);
			glNormal3fv(obj->vn[indexVn].dat);
			glTexCoord2f(obj->vt[indexVt].dat[0], obj->vt[indexVt].dat[1]);
			glVertex3fv(obj->v[indexV].dat);
		}



		glEnd();
	}
}