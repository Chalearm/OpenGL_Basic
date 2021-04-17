#!/usr/bin/env python
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
#------------------------------ light setting---------------------
#2) Add pop-up menu to switch between different light properties (ambient,
#diffuse, specular and position, point lights, directional lights, spotlights).
AMBIENT_ON,AMBIENT_OFF,DIFFUSE_ON,DIFFUSE_OFF,SPEC_ON,SPEC_OFF,POS_ON,POS_OFF,POINT_ON,POINT_OFF,DIREC_ON,DIREC_OFF,SPOT_ON,SPOT_OFF = range(14)
class lightSetting:
    isEnableAmbient = False
    isEnableDiffuse = False
    isEnableSpecular = False
    isEnablePosition = False
    isEnablePoint = False
    isEnableDirectLight = False
    isEnableSpotLights = False

    def resetLight(self):
        lightZeroColor = [0.0,0.0,0.0,1.0]
        lightZeroPosition = [0.0,0.0,0.0,1.0]
        glLightfv(GL_LIGHT0, GL_SPECULAR, lightZeroColor)
        glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor)
        glLightfv(GL_LIGHT0, GL_AMBIENT, lightZeroColor)
        glLightfv(GL_LIGHT0, GL_POSITION, lightZeroPosition)
      #  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, lightZeroPosition)
       # glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 0.0)
        
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, lightZeroColor)
        glMaterialfv(GL_FRONT, GL_DIFFUSE, lightZeroColor)
        glMaterialfv(GL_FRONT, GL_AMBIENT, lightZeroColor)

      #  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0)
     #   glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0)
     #   glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 5.0)


    def lightUpdate(self):
        lightPos = [10.0,1.0,0.0,1.0]
        lightMagentaColor = [0.8,0.0,0.8,0.1] #green tinged
        diffuseColor = [0.0,0.9,0.0,0.5]
        lightZeroColor = [0.0,0.0,0.0,0.0]
        specColor = [0.8,0.0,0.0,1.0]
        spot_direction = [-1.0,0,-1.0,1.0]
        light1_ambient = [ 0.2, 0.2, 0.2, 1.0 ]
        light1_diffuse = [ 1.0, 1.0, 1.0, 1.0 ]
        light1_specular = [1.0, 1.0, 1.0, 1.0]
        light1_position = [-2.0, 2.0, 1.0, 1.0]
        isEnableLight = ((self.isEnableAmbient == True) or (self.isEnableDiffuse == True) or (self.isEnableSpecular == True) or (self.isEnablePosition == True) or (self.isEnablePoint == True) or (self.isEnableDirectLight == True) or(self.isEnableSpotLights == True))
        glPushMatrix()
        glLoadIdentity()
        glDisable(GL_LIGHT0)
        glDisable(GL_LIGHTING)
        if (isEnableLight == True):
            self.resetLight()

        if(self.isEnableAmbient == True):
         #   glMaterialfv(GL_FRONT, GL_AMBIENT, lightMagentaColor)
            glLightfv(GL_LIGHT0, GL_AMBIENT, lightMagentaColor)
        if (self.isEnableDiffuse == True):
         #   glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseColor)
            glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseColor)
      #  glMaterialf(GL_FRONT, GL_SHININESS, 50)
        if (self.isEnableSpecular == True):
            glLightfv(GL_LIGHT0, GL_SPECULAR, specColor)
        if (self.isEnablePosition == True):
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos)
        if (self.isEnableSpotLights == True):
            glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient)
            glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse)
            glLightfv(GL_LIGHT1, GL_SPECULAR, light1_specular)
            glLightfv(GL_LIGHT1, GL_POSITION, light1_position)
            glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1.5)
            glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.5)
            glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.2)
            glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 45.0)
            glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, spot_direction)
            glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0)
            glEnable(GL_LIGHT1)
        else:
            glDisable(GL_LIGHT1)

        if (isEnableLight == True):
            glEnable(GL_LIGHTING)
            glEnable(GL_LIGHT0)
            glClearColor(0.1, 0.1, 0.1, 0.0)

        glPopMatrix()


    def ambientOn(self): 
        glutChangeToMenuEntry(1, "Ambient off", AMBIENT_OFF)
        self.isEnableAmbient = True
        glutPostRedisplay()

    def ambientOff(self): 
        glutChangeToMenuEntry(1, "Ambient on", AMBIENT_ON)
        self.isEnableAmbient = False
        glutPostRedisplay()

    def diffuseOn(self):
        glutChangeToMenuEntry(2, "Diffuse off", DIFFUSE_OFF)
        self.isEnableDiffuse = True
        glutPostRedisplay()


    def diffuseOff(self):
        glutChangeToMenuEntry(2, "Diffuse on", DIFFUSE_ON)
        self.isEnableDiffuse = False
        glutPostRedisplay()

    def specularOn(self):
        glutChangeToMenuEntry(3, "Specular off", SPEC_OFF)
        self.isEnableSpecular = True
        glutPostRedisplay()


    def specularOff(self):
        glutChangeToMenuEntry(3, "Specular on", SPEC_ON)
        self.isEnableSpecular = False
        glutPostRedisplay()

    def positionOn(self):
        glutChangeToMenuEntry(4, "Position off", POS_OFF)
        self.isEnablePosition = True
        glutPostRedisplay()


    def positionOff(self):
        glutChangeToMenuEntry(4, "Position on", POS_ON)
        self.isEnablePosition = False
        glutPostRedisplay()

    def pointOn(self):
        glutChangeToMenuEntry(5, "Point off", POINT_OFF)
        self.isEnablePoint = True
        glutPostRedisplay()


    def pointOff(self):
        glutChangeToMenuEntry(5, "Point on", POINT_ON)
        self.isEnablePoint = False   
        glutPostRedisplay() 

    def directionOn():
        glutChangeToMenuEntry(6, "direction off", DIREC_OFF)
        self.isEnableDirectLight = True
        glutPostRedisplay()


    def directionOff(self):
        glutChangeToMenuEntry(6, "direction on", DIREC_ON)
        self.isEnableDirectLight = False
        glutPostRedisplay()

    def spotLightOn(self):
        glutChangeToMenuEntry(7, "Spotlight off", SPOT_OFF)
        self.isEnableSpotLights = True
        glutPostRedisplay()


    def spotLightOff(self):
        glutChangeToMenuEntry(7, "Spotlight on", SPOT_ON)
        self.isEnableSpotLights = False
        glutPostRedisplay()

    menudict ={AMBIENT_ON : ambientOn,
               AMBIENT_OFF : ambientOff,
               DIFFUSE_ON : diffuseOn,
               DIFFUSE_OFF : diffuseOff,
               SPEC_ON : specularOn,
               SPEC_OFF : specularOff,
               POS_ON : positionOn,
               POS_OFF : positionOff,
               POINT_ON : pointOn,
               POINT_OFF : pointOff,
               DIREC_ON : directionOn,
               DIREC_OFF : directionOff,
               SPOT_ON : spotLightOn,
               SPOT_OFF : spotLightOff}

    def dmenu(self,item):
        self.menudict[item](self)
        return 0

    def createMenu(self):
        glutCreateMenu(self.dmenu)
        glutAddMenuEntry("Ambient on", AMBIENT_ON)
        glutAddMenuEntry("Diffuse on", DIFFUSE_ON)
        glutAddMenuEntry("Specular on", SPEC_ON)
        glutAddMenuEntry("Position on", POS_ON)
        glutAddMenuEntry("Point on", POINT_ON)
        glutAddMenuEntry("direction on", DIREC_ON)
        glutAddMenuEntry("Spotlight on", SPOT_ON)
        glutAttachMenu(GLUT_RIGHT_BUTTON)

