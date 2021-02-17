#!/bin/bash

gcc dinoshade.c  -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++  -o dinoshade