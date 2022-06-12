#ifndef CAMERA
#define CAMERA

#include "libraries.h"

enum cameraMove{
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 20.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera{
    public:
        glm::vec3 position;
        glm::vec3 front;
        glm::vec3 up;
        glm::vec3 right;
        glm::vec3 worldUp;
        float yaw;
        float pitch;
        float moveSpeed;
        float mouseSens;
        float zoom;

        glm::vec3 wormPosition;
        glm::vec3 wormFront;
        int wormView = 1;

        Camera(glm::vec3 position = glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
        glm::mat4 getView();
        glm::vec3 getPos();
        void processKeyboard(cameraMove dir, float time);
        void processMouse(float xoffset, float yoffset, GLboolean constrain = true);
        void processZoom(float yoffset);
        float getZoom();
        void setPosition(glm::vec3 position);
        void setFront(glm::vec3 front);
        void changeView(int view);
    private:
        void updateCamera();
        
        
};
#endif