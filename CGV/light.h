#ifndef LIGHT
#define LIGHT

#include "libraries.h"
#include "shader.h"

class Light{
    protected:
        glm::vec3 color;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
    public:
        Light(glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        ~Light();
        glm::vec3 getColor();
        glm::vec3 getAmb();
        glm::vec3 getDiff();
        glm::vec3 getSpec();
};

class DirectionalLight : public Light {
    private:
        glm::vec3 direction;
    public:
        DirectionalLight(glm::vec3 direction, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        glm::vec3 getDir();
};

class PointLight : public Light {
    private:
        glm::vec3 position;
        float constant;
        float linear;
        float quadratic;
    public:
        PointLight(glm::vec3 position, float constant, float linear, float quadratic, glm::vec3 color, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular);
        glm::vec3 getPos();
        float getConst();
        float getLin();
        float getQuad();
        void setPos(glm::vec3 position);
};

#endif