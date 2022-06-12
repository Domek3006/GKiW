#ifndef SHADER
#define SHADER

#include "libraries.h"

class Shader{
    private:
        GLuint shaderID;
        GLuint vertexID;
        GLuint geometryID;
        GLuint fragmentID;
        char* readFile(const char* fileName);
        GLuint loadShader(GLenum shaderType, const char* fileName);
    public:
        Shader(const char* vertexFile, const char* geometryFile, const char* fragmentFile);
        ~Shader();
        void use();
        void unuse();
        void setMat4fv(glm::mat4 val, const GLchar* name);
        void setVec3f(glm::fvec3 val, const GLchar* name);
        void setFloat(GLfloat val, const GLchar* name);
};

void initShader(std::vector<Shader*> &shaders, const char* vertexFile, const char* geometryFile, const char* fragmentFile);
void freeShader(std::vector<Shader*> &shaders);

#endif