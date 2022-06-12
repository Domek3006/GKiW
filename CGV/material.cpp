#include "material.h"

Material::Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shine){
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->shine = shine;
}


void freeMaterial(std::vector<Material*> materials){
    for (auto*& i : materials){
        delete i;
    }
}

glm::vec3 Material::getAmb(){
    return this->ambient;
}

glm::vec3 Material::getDiff(){
    return this->diffuse;
}

glm::vec3 Material::getSpec(){
    return this->specular;
}

float Material::getShine(){
    return this->shine;
}