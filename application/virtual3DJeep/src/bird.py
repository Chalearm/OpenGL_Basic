from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import math, time, random
import ImportObject
import PIL.Image as Image


class bird:
    obj = 0
    displayList = 0
    textureID = 0
    
    posX = 0.0
    posY = 0.0
    posZ = 0.0

    sizeX = 1.0
    sizeY = 1.0
    sizeZ = 1.0

    rotation = 0.0

    rotation = 0.0
    animatedState = 0 # 0 - 5
    lastAngle = 0.0
    
    def __init__(self, x, z):
        self.obj = ImportObject.ImportedObject("../objects/bird")
     #   self.textureID = loadTexture("../img/bird.jpg")
        self.textureID = 0 
        self.posX = x
        self.posZ = z
        
    def makeDisplayLists(self):
        self.obj.loadOBJ()

        self.displayList = glGenLists(1)
        glNewList(self.displayList, GL_COMPILE)
        self.obj.drawObject()
        glEndList()
    
    def getTextureID(self,id):
        self.textureID = id

    def draw(self):

        glEnable(GL_TEXTURE_2D)
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1)
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL)
        glBindTexture(GL_TEXTURE_2D, self.textureID)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR)
        glPushMatrix()

        glTranslatef(self.posX,self.posY,self.posZ)
        glRotatef(self.rotation+90,0.0,1.0,0.0)
        glScalef(self.sizeX,self.sizeY,self.sizeZ)
        #glRotatef(90, 0.0, 1.0, 0.0)
        glCallList(self.displayList)
        glPopMatrix()

        glDisable(GL_TEXTURE_2D)
    def move(self, rot, val): 
        if rot == False: 
            self.posZ += val * math.cos(math.radians(self.rotation)) #must make more sophisticated to go in direction
            self.posX += val * math.sin(math.radians(self.rotation))
##            self.wheel1LocZ += val * math.cos(math.radians(self.rotation))
##            self.wheel1LocX += val * math.sin(math.radians(self.rotation))
##            self.wheel2LocZ += val * math.cos(math.radians(self.rotation))
##            self.wheel2LocX += val * math.sin(math.radians(self.rotation))

           # print(self.posZ,self.posX)
        elif rot == True: 
            self.rotation+= val
    def animate(self):
        if(self.animatedState == 0):
            if (self.posZ < 106.0):
                self.move(False,random.randint(2,12)/10.0)
                self.sizeX += 0.02
                self.sizeZ += 0.02
                self.sizeY += 0.02
            else:
                self.animatedState = 1
                self.lastAngle = self.rotation
        elif (self.animatedState == 1): # rotate
            if (self.rotation - self.lastAngle < 180.0):
                self.move(True,random.randint(5, 20)*1.0)
            else:
                self.animatedState = 2
                self.rotation = 180.0
        elif (self.animatedState == 2):
            if (self.posZ > 0.0):
                self.move(False,random.randint(2,12)/10.0)
                if (self.sizeX > 0.1):
                    self.sizeX -= 0.02
                else:
                    self.sizeX  = 0.15
                if (self.sizeY > 0.1):
                    self.sizeY -= 0.02
                else:
                    self.sizeY  = 0.15
                if (self.sizeZ > 0.1):
                    self.sizeZ -= 0.02
                else:
                    self.sizeZ  = 0.15
            else:
                self.animatedState = 3
                self.lastAngle = self.rotation
        elif (self.animatedState == 3): # rotate
            if (self.rotation - self.lastAngle < 180.0):
                self.move(True,random.randint(5, 20)*1.0)
            else:
                self.animatedState = 0
                self.rotation = 0
        if (self.posX > 10):
            self.posX = 10
        if (self.posX < -10):
            self.posX = -10




            
        
