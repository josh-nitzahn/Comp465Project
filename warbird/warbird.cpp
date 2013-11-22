/*
warbird.cpp
*/

# define __Windows__

# include "..\includes465\include465.h"
# define __INCLUDES465__

# include "3dobject.hpp"


# define DEFAULT_CAMERA_EYE glm::vec3(0.0f,0.0f,40000.0f)
# define DEFAULT_CAMERA_AT  glm::vec3(0.0f,0.0f,0.0f)
# define DEFAULT_CAMERA_UP  glm::vec3(0.0f,1.0f,0.0f)
        
# define CAMERAS 5
# define PLANETS 2
# define MOONS1 0
# define MOONS2 2
# define BODIES 5
# define GRAVITY 90000000
        
const int nModels = 2;
int currentCam = -1; //front view
Object3D * bodies[BODIES] = {NULL};
Object3D * Ruber = NULL;
Object3D * Unum = NULL;
Object3D * Duo = NULL;
Object3D * Primus = NULL;
Object3D * Secundus = NULL;
Object3D * warbird = NULL;
Object3D * cameras[CAMERAS-1] = {NULL};
Object3D * topCam = NULL;
Object3D * UnumCam = NULL;
Object3D * DuoCam = NULL;
Object3D * warCam = NULL;

char * modelFile[nModels] = {"planet.tri", "ship2.tri"};
float modelBoundingRadius[nModels];
float scaleValue[nModels];
const GLuint nVertices[nModels] = {2867 * 3, 38138 * 3};

char * vertexShaderFile = "vertexReview1.glsl";
char * fragmentShaderFile = "fragmentReview1.glsl";

GLuint shaderProgram;
GLuint VAO[nModels];
GLuint buffer[nModels];

GLuint modelview, project;
GLuint vPosition[nModels], vColor[nModels], vNormal[nModels];

glm::mat4 modelMatrix;
glm::mat4 viewMatrix;
glm::mat4 modelViewMatrix;
glm::mat4 projectionMatrix;

glm::mat4 frontCamMat = glm::lookAt(DEFAULT_CAMERA_EYE, DEFAULT_CAMERA_AT, DEFAULT_CAMERA_UP);

char titleString[256];
char baseTitle[] = "465 Warbird Project : ";
char subTitleWarbird[] = "Warbird ";
char subTitleUnum[] = "Unum ";
char subTitleDuo[] = "Duo ";
char subTitleRate[] = "U/S ";
char subTitleView[] = "View: ";

enum movement {none, forward, backward, left, right, up, down, rollR, rollL};
movement warMod = none;


void display();
void reshape(int width, int height);
void animate(int i);
void keyboard(unsigned char key, int x, int y);
void keyboardSpec(int key, int x, int y);

void init();


int main(int argc, char * argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
	//glutInitContextVersion(3, 0);
    glutInitContextVersion(3, 3);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutCreateWindow("465 Warbird Project");

    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err)
        printf("GLEW Error: %s \n", glewGetErrorString(err));
      else {
        printf("Using GLEW %s \n", glewGetString(GLEW_VERSION));
        printf("OpenGL %s, GLSL %s\n", glGetString(GL_VERSION),
          glGetString(GL_SHADING_LANGUAGE_VERSION));
        }

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
    glutSpecialFunc(keyboardSpec);
    glutTimerFunc(40, animate, 1);
    glutMainLoop();
    
    delete Ruber;


    printf("End of program.\n");
    return 0;
    }

void init() {
    shaderProgram = loadShaders(vertexShaderFile, fragmentShaderFile);
    glUseProgram(shaderProgram);

    glGenVertexArrays(nModels, VAO);
    glGenBuffers(nModels, buffer);
    
    for (int i = 0; i < nModels; i++) {
        modelBoundingRadius[i] = loadModelBuffer(modelFile[i], nVertices[i],
          VAO[i], buffer[i], shaderProgram, vPosition[i], vColor[i],
          vNormal[i], "vPosition", "vColor", "vNormal");
        }
    
    modelview = glGetUniformLocation(shaderProgram, "ModelView");
    project =   glGetUniformLocation(shaderProgram, "Projection");
    
        
    Ruber = new Object3D(PLANETS + 1);    //one camera satellite
    Ruber->setOrbit(glm::vec3(0.0f, 0.0f, 0.0f), 0);
    //Ruber->setRotate(glm::vec3(0.0f, 1.0f, 0.0f), 0);
    Ruber->setScale(glm::vec3(2000.0f / modelBoundingRadius[0]));
    Ruber->setCamera(false);
    
    topCam = Ruber->getSatellite(0);
    topCam->setTranslation(glm::vec3(0.0f, 40000.0f, 0.0f));
    topCam->setOrbit(glm::vec3(0.0f, 0.0f, 0.0f), 0);
    //topCam->setRotate(glm::vec3(1.0f, 0.0f, 0.0f), PI / 2);
    
    Unum = Ruber->getSatellite(1);
    Unum->setTranslation(glm::vec3(4000.0f, 0.0f, 0.0f));
    Unum->setScale(glm::vec3(200.0f / modelBoundingRadius[0]));
    Unum->setOrbitAngle(0.004f);
    Unum->setCamera(false);
    
    Duo = Ruber->getSatellite(2);
    Duo->setTranslation(glm::vec3(-9000.0f, 0.0f, 0.0f));
    Duo->setScale(glm::vec3(400.0f / modelBoundingRadius[0]));
    Duo->setOrbitAngle(0.002f);
    Duo->setCamera(false);
    
    Unum->makeSatellites(MOONS1 + 1);
    
    UnumCam = Unum->getSatellite(0);
    UnumCam->setTranslation(glm::vec3(0.0f, 2000.0f, 0.0f));
    UnumCam->setOrbitAngle(0.0f);
    //UnumCam->setRotate(glm::vec3(0.0f, 0.0f, 1.0f), -PI / 2);
    
    Duo->makeSatellites(MOONS2 + 1);
    
    DuoCam = Duo->getSatellite(0);
    DuoCam->setTranslation(glm::vec3(0.0f, 2000.0f, 0));
    DuoCam->setOrbitAngle(0.0f);
    //DuoCam->setRotate(glm::vec3(1.0f, 0.0f, 0.0f), PI / 2);
    
    Primus = Duo->getSatellite(1);
    Primus->setTranslation(glm::vec3(-900.0f, 0.0f, 0.0f));
    Primus->setScale(glm::vec3(100 / modelBoundingRadius[0]));
    Primus->setOrbitAngle(0.004f);
    Primus->setCamera(false);
        
    Secundus = Duo->getSatellite(2);
    Secundus->setTranslation(glm::vec3(-1750.0f, 0.0f, 0.0f));
    Secundus->setScale(glm::vec3(150 / modelBoundingRadius[0]));
    Secundus->setOrbitAngle(0.002f);
    Secundus->setCamera(false);
    
    warbird = new Object3D(1);
    warbird->setTranslation(glm::vec3(5000.0f, 1000.0f, 5000.0f));
    warbird->setScale(glm::vec3(100.0f / modelBoundingRadius[1]));
    warbird->setCamera(false);
    
    warCam = warbird->getSatellite(0);
    warCam->setTranslation(glm::vec3(0.0f, 500.0f, -1000.0f));
    warCam->setCameraOffset(glm::vec3(0.0f, 250.0f, 0.0f));
    
    bodies[0] = Ruber;
    bodies[1] = Unum;
    bodies[2] = Duo;
    bodies[3] = Primus;
    bodies[4] = Secundus;
    
    cameras[0] = topCam;
    cameras[1] = warCam;
    
    cameras[2] = UnumCam;
    cameras[3] = DuoCam;
    
    viewMatrix = frontCamMat;
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    }
    
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glBindVertexArray(VAO[0]);
    glEnableVertexAttribArray( vPosition[0] );
    glEnableVertexAttribArray( vColor[0] );
    glEnableVertexAttribArray( vNormal[0] );
    
    
    for (unsigned i = 0; i < BODIES; i++) {
        modelMatrix = bodies[i]->getModelMatrix();
        modelViewMatrix = viewMatrix * modelMatrix;
        glUniformMatrix4fv(modelview, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
        glDrawArrays(GL_TRIANGLES, 0, nVertices[0]);
        }
        
    modelMatrix = warbird->getModelMatrix();
    modelViewMatrix = viewMatrix * modelMatrix;
    
    glUniformMatrix4fv(modelview, 1, GL_FALSE, glm::value_ptr(modelViewMatrix));
    glUniformMatrix4fv(project, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glBindVertexArray(VAO[1]);
    glEnableVertexAttribArray( vPosition[1] );
    glEnableVertexAttribArray( vColor[1] );
    glEnableVertexAttribArray( vNormal[1] );
    glDrawArrays(GL_TRIANGLES, 0, nVertices[1]);
    
    glutSwapBuffers();
    }
    
void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    projectionMatrix = glm::perspective(glm::radians(45.0f), (GLfloat) width /
        (GLfloat) height, 1.0f, 100000.0f);
    }
    
void animate(int i) {
    
    glm::vec3 temp;
    float dist;
    
    glutTimerFunc(40, animate, 1);
    bodies[0]->update();
    
    switch(warMod) {
    case forward:
        warbird->move(glm::vec3(0.0f, 0.0f, 10.0f));
        break;
    case backward:
        warbird->move(glm::vec3(0.0f, 0.0f, -10.0f));
        break;
    case left:
        warbird->yaw(0.02f);
        warCam->setOrbitAngle(0.02f);
        break;
    case right:
        warbird->yaw(-0.02f);
        warCam->setOrbitAngle(-0.02f);
        break;
    case up:
        warbird->pitch(-0.02f);
        warCam->setOrbitAxis(glm::vec3(1.0f, 0.0f, 0.0f));
        warCam->setOrbitAngle(-0.02f);
        break;
    case down:
        warbird->pitch(0.02f);
        warCam->setOrbitAxis(glm::vec3(1.0f, 0.0f, 0.0f));
        warCam->setOrbitAngle(0.02f);
        break;
    case rollR:
        warbird->roll(-0.02f);
        warCam->setOrbitAxis(glm::vec3(0.0f, 0.0f, 1.0f));
        warCam->setOrbitAngle(-0.02f);
        break;
    case rollL:
        warbird->roll(0.02f);
        warCam->setOrbitAxis(glm::vec3(0.0f, 0.0f, 1.0f));
        warCam->setOrbitAngle(0.02f);
        break;
    }
    
    warbird->update();
    warCam->setOrbitAxis(glm::vec3(0.0f, 1.0f, 0.0f));
    warCam->setOrbitAngle(0);
        
    temp = glm::vec3(warbird->getModelMatrix()[3]);
    dist = glm::dot(temp, temp);
    warbird->move(GRAVITY / dist * (temp / glm::sqrt(dist)));
    
    warMod = none;
    
    
    
    if(currentCam == -1)
        viewMatrix = frontCamMat;
    else
        viewMatrix = cameras[currentCam]->getView();
    glutPostRedisplay();
    }

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 'v': case 'V':
            currentCam = (currentCam == -1) ? 0 : ((currentCam == 0) ? 1 : -1);

            printf("camera changed.\n");
            break;
            
    case 'p': case 'P':
            currentCam = ( currentCam == 2 ) ? 3 : 2;
            printf("camera changed.\n");
            break;
    }
}

void keyboardSpec(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_UP:
        if(glutGetModifiers() && GLUT_ACTIVE_CTRL)
            warMod = down;
        else
            warMod = forward;
        break;
        
    case GLUT_KEY_DOWN:
        if(glutGetModifiers() && GLUT_ACTIVE_CTRL)
            warMod = up;
        else
            warMod = backward;
        break;
        
    case GLUT_KEY_LEFT:
        if(glutGetModifiers() && GLUT_ACTIVE_CTRL)
            warMod = rollL;
        else
            warMod = left;
        break;
        
    case GLUT_KEY_RIGHT:
        if(glutGetModifiers() && GLUT_ACTIVE_CTRL)
            warMod = rollR;
        else
            warMod = right;
        break;
    }
}