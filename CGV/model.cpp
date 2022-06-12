#include "model.h"

Model::Model(const char* objFile, Material* material, Texture* texture, glm::vec3 color, std::string type, glm::vec3 position){
    std::vector<vertex> mdl = loadOBJ(objFile, color);
    this->meshes.push_back(new Mesh(mdl.data(), mdl.size(), material, texture, type));
    this->position = position;
}

Model::~Model(){
    for (auto*& i : this->meshes){
        delete i;
    }
}

void Model::draw(Shader *shader, int r) {
    for (auto& i : this->meshes){
        i->draw(shader, this->position, this->angle, r);
    }
}

void Model::scale(glm::vec3 scale){
    for (auto& i : this->meshes) {
        i->scale(scale);
    }
}

void Model::rotate(glm::vec3 rot, float angle) {
    for (auto& i : this->meshes) {
        i->rotate(rot, angle);
    }
}

void Model::translate(glm::vec3 trans) {
    for (auto& i : this->meshes) {
        i->translate(trans);
    }
}

void Model::addMesh(const char* objFile, Material* material, Texture* texture, glm::vec3 color, std::string type) {
    std::vector<vertex> mdl = loadOBJ(objFile, color);
    this->meshes.push_back(new Mesh(mdl.data(), mdl.size(), material, texture, type));
}

void Model::translateMesh(unsigned mesh, glm::vec3 trans) {
    this->meshes[mesh]->translate(trans);    
}

void Model::rotateMesh(unsigned mesh, glm::vec3 rot, float angle) {
    this->meshes[mesh]->rotate(rot, angle);
}

void Model::scaleMesh(unsigned mesh, glm::vec3 sca) {
    this->meshes[mesh]->scale(sca);
}

void Model::changeRotation(glm::vec3 rot, glm::vec3 angle) {
    this->rotation = rot;
    this->angle += glm::radians(angle);
}

void Model::changePosition(glm::vec3 move, float dist, bool forward) {
    if (!forward) {
        this->position += move;
    }
    else {
        this->position += this->fronts[this->frontAngle] * dist;
    }
    
}

void Model::setPosition(glm::vec3 move) {
    this->position = move;
}

void Model::updateFront(int index) {
    this->frontAngle -= index;
    if (this->frontAngle == -1) this->frontAngle = 11;
    if (this->frontAngle == 12) this->frontAngle = 0;
}

glm::vec3 Model::getPos() {
    return this->position;
}

glm::vec3 Model::getFront() {
    return this->fronts[frontAngle];
}

unsigned Model::getHealth() {
    return this->health;
}

void Model::changeHealth(int damage) {
    this->health += damage;
}

float Model::getAngle() {
    return this->angle.y;
}

void Model::setFront(unsigned index) {
    this->frontAngle = index;
}

unsigned Model::getFrontIndex() {
    return this->frontAngle;
}

void Model::setAngle(float angle) {
    this->angle.y = angle;
}

float Model::findHeight(float x, float z) {
    return this->meshes[0]->findHeight(x, z);
}

void Model::setHeight(float y) {
    this->position.y = y;
}