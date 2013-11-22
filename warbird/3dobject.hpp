/*
3dobject.hpp
*/

# ifndef __INCLUDES465__
# include "../includes465/include465.h"
# define __INCLUDES465__
# endif


class Object3D {
  public:
    Object3D();
    Object3D(unsigned satellites);
    void update();
    unsigned getSatellites();
    glm::mat4 getModelMatrix();
    glm::mat4 getView();
    void setOrbit(glm::vec3 orbit, float angle);
    void setOrbitPos(glm::vec3 pos);
    void setOrbitAngle(float angle);
    void setOrbitAxis(glm::vec3 axis);
    void setScale(glm::vec3 scale);
    void setCameraOffset(glm::vec3 offset);
    void setTranslation(glm::vec3 pos);
    void setCamera(bool cam);
    void makeSatellites(unsigned number);
    void move(glm::vec3 amount);
    void yaw(float radians);
    void pitch(float radians);
    void roll(float radians);
    Object3D * getSatellite(unsigned number);
    ~Object3D();

  private:
    void makeModelMat();

    unsigned satellites;
    bool visible;
    Object3D **satellites_p;
    glm::mat4 orbitMat;
    glm::mat4 scaleMat;
    glm::mat4 rotateMat;
    glm::mat4 translationMat;
    glm::mat4 baseTranslate;
    glm::mat4 viewMat;
    glm::mat4 modelMat;
    glm::vec3 rotateAxis;
    glm::vec3 orbitAxis;
    glm::vec3 orbitPos;
    glm::vec3 satPos;
    glm::vec3 distance;
    glm::vec3 cameraOffset;
    float rotateAngle;
    float orbitAngle;

    glm::vec3 eye;
    glm::vec3 at;
    glm::vec3 up;
    };

Object3D::Object3D() : satellites(0), visible(false), satellites_p(NULL),
        rotateAngle(0), orbitAngle(0) {

    rotateAxis = orbitAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraOffset = orbitPos = satPos = glm::vec3(0.0f);
    rotateMat = orbitMat = scaleMat = translationMat = glm::mat4(1.0f);
    eye = glm::vec3(0.0f, 0.0f, 0.0f);
    at = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    }

Object3D::Object3D(unsigned satellites) : satellites(satellites),
        visible(false), rotateAngle(0), orbitAngle(0) {
    rotateAxis = glm::vec3(0.0f, 1.0f, 0.0f);
    cameraOffset = orbitAxis = satPos = glm::vec3(0.0f);
    rotateMat = orbitMat = scaleMat = translationMat = glm::mat4(1.0f);
    eye = glm::vec3(0.0f, 0.0f, 0.0f);
    at = glm::vec3(0.0f, 0.0f, -1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);

    satellites_p = new Object3D * [satellites];

    for (unsigned i = 0; i < satellites; i++) {
        satellites_p[i] = new Object3D;
        satellites_p[i]->setOrbit(satPos, 0);
        }

    printf("Made %u satellites for object.\n", satellites);
    }

Object3D::~Object3D() {
    if (satellites != 0) {
        for (unsigned i = 0; i < satellites; i++)
            delete satellites_p[i];

        delete satellites_p;
        printf("Object deleted\n");
        }
    }

void Object3D::update() {
    makeModelMat();

    for (unsigned i = 0; i < satellites; i++) {
        satellites_p[i]->setOrbitPos(satPos);
        satellites_p[i]->update();
        }
    }

unsigned Object3D::getSatellites() {
    return satellites;
    }

glm::mat4 Object3D::getModelMatrix() {
    return modelMat;
    }

glm::mat4 Object3D::getView() {
    return viewMat;
    }

void Object3D::makeModelMat() {
    orbitMat = glm::rotate(orbitMat, orbitAngle, rotateAxis);
    
    baseTranslate = glm::translate(glm::mat4(1.0f), distance);
    translationMat = glm::translate(glm::mat4(1.0f), orbitPos);
    modelMat = translationMat * orbitMat * baseTranslate * rotateMat * scaleMat;

    satPos = eye = glm::vec3(modelMat[3]);
    at = glm::vec3(modelMat[2]) + orbitPos + cameraOffset;
    up = glm::vec3(modelMat[1]);
        
    viewMat = glm::lookAt(eye, at, up);
    }

void Object3D::setOrbit(glm::vec3 pos, float angle) {
    orbitPos = pos;
    orbitAngle = angle;
    }

void Object3D::setOrbitPos(glm::vec3 pos) {
    orbitPos = pos;
    }

void Object3D::setOrbitAngle(float angle) {
    orbitAngle = angle;
    }

void Object3D::setOrbitAxis(glm::vec3 axis) {
    rotateAxis = axis;
    }
    
void Object3D::setScale(glm::vec3 scale) {
    scaleMat = glm::scale(glm::mat4(1.0f), scale);
    }

void Object3D::setCameraOffset(glm::vec3 offset) {
    cameraOffset = offset;
    }

void Object3D::setTranslation(glm::vec3 pos) {
    orbitAxis = glm::vec3(0.0f);
    satPos = distance = pos;
    }
    
void Object3D::move(glm::vec3 amount) {
    rotateMat = glm::translate(rotateMat, amount);
    }
    
void Object3D::yaw(float radians) {
    rotateMat = glm::rotate(rotateMat, radians, glm::vec3(0.0f, 1.0f, 0.0f));
    }

void Object3D::pitch(float radians) {
    rotateMat = glm::rotate(rotateMat, radians, glm::vec3(1.0f, 0.0f, 0.0f));
    }

void Object3D::roll(float radians) {
    rotateMat = glm::rotate(rotateMat, radians, glm::vec3(0.0f, 0.0f, 1.0f));
    }

void Object3D::setCamera(bool cam) {
    visible = !cam;
    }

void Object3D::makeSatellites(unsigned number) {
    if (satellites != 0) {
        printf("Satellites already exist for object!\n");
        return;
        }
    satellites_p = new Object3D * [number];

    makeModelMat();

    for (unsigned i = 0; i < number; i++) {
        satellites_p[i] = new Object3D;
        satellites_p[i]->setOrbit(satPos, 0);
        }

    satellites = number;
    printf("Made %u satellites for object.\n", satellites);
    }

Object3D * Object3D::getSatellite(unsigned number) {
    return satellites_p[number];
    }

/*
class List_node {
  public:
    List_node();
    ~List_node();
    List_node * add();
    List_node * nextItem();

  private:
    void * current;
    List_node * next;
    };

List_node::List_node() : current(NULL), next(NULL) {}

List_node::~List_node() {
    if (current != NULL)
        delete current;
    printf("List node deleted.\n");
    }

List_node * List_node::add() {
    if (next == NULL) {
        next = new List_node;
        return next;
        }
    else
        printf("Tried to add to middle of list!\n");
    return NULL;
    }

List_node * List_node::nextItem() {
    return next;
    }*/