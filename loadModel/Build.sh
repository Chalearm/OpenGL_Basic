#!/bin/bash

gcc $1.c loadModel.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++ -o $1