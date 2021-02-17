#!/bin/bash

gcc dinoball.c  -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++  -o dinoball