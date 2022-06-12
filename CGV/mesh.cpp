#include "mesh.h"

Mesh::Mesh(vertex* vertexArray, const unsigned int& vertexCount, Material* material, Texture* texture, std::string type, glm::mat4 M){
    this->material = material;
    this->texture = texture;
    this->type = type;
    this->vertexCount = vertexCount;
    this->vertexArray = new vertex[this->vertexCount];

    this->modelMat = M;
    for (unsigned int i = 0; i < vertexCount; i++){
        this->vertexArray[i] = vertexArray[i];
    }

    this->initVAO();
}

Mesh::~Mesh(){
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    delete[] this->vertexArray;
}

void Mesh::initVAO() {

    glCreateVertexArrays(1, &this->VAO);
    glBindVertexArray(this->VAO);

    glGenBuffers(1, &this->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->vertexCount * sizeof(vertex), this->vertexArray, GL_STATIC_DRAW);
    
    /*glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(vertex), (GLvoid*)(offsetof(vertex, position)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(vertex), (GLvoid*)(offsetof(vertex, normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, sizeof(vertex), (GLvoid*)(offsetof(vertex, texCoord)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(vertex), (GLvoid*)(offsetof(vertex, color)));*/

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 44, (GLvoid*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 44, (GLvoid*)32);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, false, 44, (GLvoid*)24);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, false, 44, (GLvoid*)12);
    
    /*glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);*/

    glBindVertexArray(0);
}

void Mesh::draw(Shader *shader, glm::vec3 trans, glm::vec3 angle, int r){
    //this->rotate(rot, angle);
    if (this->type == "worm") {
        //this->scale(glm::vec3(0.1f));
        this->translate(trans);
        this->rotate(glm::vec3(0.f, 1.f, 0.f), angle.y);
    }
    else if (this->type == "baz"){       
        //this->scale(glm::vec3(0.1f));
        
        if (r == 0)
            this->scale(glm::vec3(0.f));

        else if (r == 1) {
            //this->rotate(glm::vec3(1.f, 0.f, 0.f), glm::radians(10.f));
            this->scale(glm::vec3(0.8f));
            //this->translate(glm::vec3(-5.f, 2.f, 7.f));
        }
        else if (r == 2) {
            this->scale(glm::vec3(0.9f));
            //this->translate(glm::vec3(-3.f, 0.f, 4.f));
        }
        else {
            //this->rotate(glm::vec3(1.f, 0.f, 0.f), glm::radians(-10.f));
            //this->scale(glm::vec3(0.1f));
            //this->translate(glm::vec3(-2.f, -1.f, 3.f));
        }
        this->translate(trans);
        this->rotate(glm::vec3(0.f, 1.f, 0.f), angle.y);
    }
    else if (this->type == "cross") {
        //if (trans.y > 10.f) trans.y = 10.f;
        //else if (trans.y < -10.f) trans.y = -10.f;
        //this->scale(glm::vec3(0.1));
        this->translate(trans);
        this->translate(glm::vec3(0.f, 10.f, 0.f));
        this->rotate(glm::vec3(0.f, 1.f, 0.f), angle.y);
        this->rotate(glm::vec3(0.f, 1.f, 0.f), glm::radians(90.f));
    }

    else if (this->type == "terra") {
        this->translate(glm::vec3(0.f, -0.3f, 0.f));
    }

    else if (this->type == "arrow") {
        this->translate(trans);
    }

    else if (this->type == "rocket") {
        this->translate(trans);
        this->rotate(glm::vec3(0.f, 1.f, 0.f), angle.y);
        if (r > glm::radians(120.f)) r = glm::radians(120.f);
        this->rotate(glm::vec3(0.f, 0.f, 1.f), r);
    }

    else if (type == "sphere") {
        this->translate(trans);
    }

    else if (type == "water") {
        this->scale(glm::vec3(0.4f));
        this->translate(glm::vec3(r, 5.f, r));
    }

    else if (type == "lamp") {
        if (r == 0) this->translate(glm::vec3(-30.f, 11.f, -30.5f));
        else if (r == 1) this->translate(glm::vec3(-10.f, 9.f, 15.f));
        else if (r == 2) this->translate(glm::vec3(28.f, 0.f, -34.f));
    }

    shader->setMat4fv(this->modelMat, "M");
    shader->setVec3f(this->material->getAmb(), "material.ambient");
    shader->setVec3f(this->material->getSpec(), "material.specular");
    shader->setVec3f(this->material->getDiff(), "material.diffuse");
    shader->setFloat(this->material->getShine(), "material.shine");
    shader->use();
    this->texture->bind();
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->vertexCount);
    glBindVertexArray(0);
    glUseProgram(0);
    this->texture->unbind();
    this->modelMat = glm::mat4(1.0f);
}

void Mesh::scale(glm::vec3 scale) {
    this->modelMat = glm::scale(modelMat, scale);
}

void Mesh::rotate(glm::vec3 rot, float angle) {
    this->modelMat = glm::rotate(modelMat, angle, rot);
}

void Mesh::translate(glm::vec3 trans) {
    this->modelMat = glm::translate(modelMat, trans);
}

float Mesh::findHeight(float x, float z) {
    for (int i = 0; i < this->vertexCount; i++) {
        if (pow(pow(this->vertexArray[i].position.x - x, 2.f) + pow(this->vertexArray[i].position.z - z, 2.f),0.5f) <= 2.0f) {
            return this->vertexArray[i].position.y;
        }
    }
    return 0.f;
}

glm::vec3 Mesh::getVertex() {
    return this->vertexArray[3].position;
}