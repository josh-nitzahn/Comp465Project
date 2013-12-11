set PATH=%PATH%;..\..\MinGW\bin
g++ textureModel.cpp -L. -l freeglut -l glew32 -l opengl32 -l glu32 -o textureModel.exe
pause
textureModel.exe
pause