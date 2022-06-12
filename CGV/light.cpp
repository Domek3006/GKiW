#include "light.h"

Light::Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular){
    this->color = color;
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

Light::~Light(){}

glm::vec3 Light::getColor() {
    return this->color;
}

glm::vec3 Light::getAmb() {
    return this->ambient;
}

glm::vec3 Light::getDiff() {
    return this->diffuse;
}

glm::vec3 Light::getSpec() {
    return this->specular;
}

DirectionalLight::DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular):
    Light(color, ambient, diffuse, specular){
    this->direction = direction;
}

glm::vec3 DirectionalLight::getDir() {
    return this->direction;
}

PointLight::PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) :
    Light(color, ambient, diffuse, specular) {
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

glm::vec3 PointLight::getPos() {
    return this->position;
}

float PointLight::getConst() {
    return this->constant;
}

float PointLight::getLin() {
    return this->linear;
}
float PointLight::getQuad() {
    return this->quadratic;
}

void PointLight::setPos(glm::vec3 position) {
    this->position = position;
}