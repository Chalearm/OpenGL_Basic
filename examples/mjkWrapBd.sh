#!/bin/bash

gcc mjkimage.c mjkwarp.c  -framework GLUT -framework OpenGL -L"/System/Library/Frameworks/OpenGL.framework/Libraries" -lm -lobjc -lstdc++  -o mjkwarp
