#ifndef VERTEX
#define VERTEX

#include <glm/glm.hpp>

struct vertex
{
    glm::vec3 position;
    glm::vec3 color;
    glm::vec2 texCoord;
    glm::vec3 normal;
};


#endif