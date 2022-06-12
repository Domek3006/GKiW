#ifndef MATERIAL
#define MATERIAL

#include "libraries.h"

class Material{
    private:
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        float shine;
    public:
        Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shine);
        glm::vec3 getAmb();
        glm::vec3 getDiff();
        glm::vec3 getSpec();
        float getShine();
};

void freeMaterial(std::vector<Material*> materials);

#endif