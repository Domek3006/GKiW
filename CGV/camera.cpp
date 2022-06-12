#include "camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch): 
front(glm::vec3(0.0f, 0.0f, -1.0f)),
moveSpeed(SPEED),
mouseSens(SENSITIVITY),
zoom(ZOOM){
    this->position = position;
    this->worldUp = up;
    this->pitch = pitch;
    this->yaw = yaw;
    this->updateCamera();
}


glm::mat4 Camera::getView(){
    if (this->wormView == 1) {
        this->updateCamera();
        return glm::lookAt(this->position, this->position + this->front, this->up);
    }
    else {
        //this->wormPosition.y += 5.f;
        return glm::lookAt(this->wormPosition, this->wormPosition + this->wormFront, this->worldUp);
    }
}

void Camera::processKeyboard(cameraMove dir, float time){
    if (this->wormView == -1) return;
    float vel = this->moveSpeed * time;
    if (dir == FORWARD){
        this->position +=  vel * this->front;
    }
    if (dir == BACKWARD){
        this->position -=  vel * this->front;
    }
    if (dir == LEFT){
        this->position -= this->right * vel;
    }
    if (dir == RIGHT){
        this->position += this->right * vel;
    }
    if (this->position.y < 0.3f) {
        this->position.y = 0.3f;
    }
}

void Camera::processMouse(float xoffset, float yoffset, GLboolean constrain){
    xoffset *= this->mouseSens;
    yoffset *= this->mouseSens;
    this->yaw += static_cast<GLfloat>(xoffset);
    this->pitch += static_cast<GLfloat>(yoffset);

    if (constrain){
        if (this->pitch > 89.0f){
            this->pitch = 89.0f;
        }
        if (this->pitch < -89.0f){
            this->pitch = -89.0f;
        }
    }

    updateCamera();
}

void Camera::processZoom(float yoffset){
    this->zoom -= (float)yoffset;
    if (this->zoom < 1.0f){
        this->zoom = 1.0f;
    }
    if (this->zoom > 45.0f){
        this->zoom = 45.0f;
    }
}

void Camera::updateCamera(){
    glm::vec3 front;
    front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    front.y = sin(glm::radians(this->pitch));
    front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front = glm::normalize(front);
    this->right = glm::normalize(glm::cross(this->front, this->worldUp));
    this->up = glm::normalize(glm::cross(this->right, this->front));
}

glm::vec3 Camera::getPos(){
    return this->position;
}

float Camera::getZoom() {
    return this->zoom;
}

void Camera::setPosition(glm::vec3 position) {
    this->wormPosition = position;
}

void Camera::setFront(glm::vec3 front) {
    this->wormFront = front;
}

void Camera::changeView(int view) {
    this->wormView = view;
}