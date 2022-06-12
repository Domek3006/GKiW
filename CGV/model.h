#ifndef MODEL
#define MODEL

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include "OBJloader.h"
#include "mesh.h"
#include "material.h"
#include "texture.h"

class Model {
    private:
        glm::vec3 position;
        glm::vec3 rotation = glm::vec3(0.f, 1.f, 0.f);
        glm::vec3 angle = glm::vec3(0.f);
        //glm::vec3 front = fronts[0];
        unsigned frontAngle = 0;
        std::vector<Mesh*> meshes;
        std::vector<glm::vec3> fronts = {glm::vec3(-1.f, 0.f, 0.f), glm::vec3(-0.86f, 0.f, -0.5f), glm::vec3(-0.5f, 0.f, -0.86f), 
        glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.5f, 0.f, -0.86f), glm::vec3(0.86f, 0.f, -0.5f), glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.86f, 0.f, 0.5f), glm::vec3(0.5f, 0.f, 0.86f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(-0.5f, 0.f, 0.86f), glm::vec3(-0.86f, 0.f, 0.5f) };
        int health = 100;
        float rocketRot = 0.f;
    public:
        Model(const char* objFile, Material* material, Texture* texture, glm::vec3 color, std::string type, glm::vec3 position = glm::vec3(0.f));
        ~Model();            
        void draw(Shader *shader, int r = 0);
        void scale(glm::vec3 scale);
        void rotate(glm::vec3 rot, float angle);
        void translate(glm::vec3 trans);
        void addMesh(const char* objFile, Material* material, Texture* texture, glm::vec3 color, std::string type);
        void translateMesh(unsigned mesh, glm::vec3 trans);
        void rotateMesh(unsigned mesh, glm::vec3 rot, float angle);
        void scaleMesh(unsigned mesh, glm::vec3 sca);
        void changeRotation(glm::vec3 rot, glm::vec3 angle);
        void changePosition(glm::vec3 move, float dist, bool forward = false);
        void setPosition(glm::vec3 move);
        void updateFront(int index);
        void setFront(unsigned index);
        glm::vec3 getPos();
        glm::vec3 getFront();
        unsigned getHealth();
        void changeHealth(int damage);
        float getAngle();
        void setAngle(float angle);
        unsigned getFrontIndex();
        float findHeight(float x, float z);
        void setHeight(float y);
};

#endif