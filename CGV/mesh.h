#ifndef MESH
#define MESH

#include "libraries.h"
#include "vertex.h"
#include "shader.h"
#include "texture.h"
#include "material.h"

class Mesh{
    private:
        vertex* vertexArray;
        unsigned int vertexCount;
        glm::mat4 modelMat;
        GLuint VAO;
        GLuint VBO;
        Material* material;
        Texture* texture;
        std::string type;

        void initVAO();
    public:
        Mesh(vertex* vertexArray, const unsigned int& vertexCount, Material* material, Texture* texture, std::string type, glm::mat4 M = glm::mat4(1.f));
        ~Mesh();
        void draw(Shader *shader, glm::vec3 trans, glm::vec3 angle, int r);
        void scale(glm::vec3 scale);
        void rotate(glm::vec3 rot, float angle);
        void translate(glm::vec3 trans);
        float findHeight(float x, float z);
        glm::vec3 getVertex();
};

#endif