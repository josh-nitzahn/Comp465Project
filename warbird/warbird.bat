set PATH=%PATH%;..\MinGW\bin
g++ warbird.cpp -L. -l freeglut -l glew32 -l opengl32 -l glu32 -o warbird.exe
pause
warbird.exe
pause