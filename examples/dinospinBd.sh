#!/bin/bash

gcc trackball.c dinospin.c -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++  -o dinospin