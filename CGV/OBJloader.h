#ifndef LOADER
#define LOADER

#include "libraries.h"
#include "vertex.h"

static std::vector<vertex> loadOBJ(const char* file_name, glm::vec3 color){
    std::vector<glm::fvec3> vertex_positions;
    std::vector<glm::fvec2> vertex_texCoords;
    std::vector<glm::fvec3> vertex_normals;
    std::vector<GLint> vertex_position_indicies;
    std::vector<GLint> vertex_texcoord_indicies;
	std::vector<GLint> vertex_normal_indicies;
    std::vector<vertex> vertices;
    std::stringstream ss;
    std::ifstream in_file(file_name);
    std::string line = "";
    std::string prefix = "";
    glm::vec3 tmp_vec3;
    glm::vec2 tmp_vec2;
    GLint tmp_glint = 0;

    while (std::getline(in_file, line)){
        ss.clear();
        ss.str(line);
        ss >> prefix;

        if (prefix == "v"){
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_positions.push_back(tmp_vec3);
        }
        else if (prefix == "vt"){
            ss >> tmp_vec2.x >> tmp_vec2.y;
            tmp_vec2.y *= -1;
            vertex_texCoords.push_back(tmp_vec2);
        }
        else if (prefix == "vn"){
            ss >> tmp_vec3.x >> tmp_vec3.y >> tmp_vec3.z;
            vertex_normals.push_back(tmp_vec3);
        }
        else if (prefix == "f"){
            int counter = 0;
            while (ss >> tmp_glint){
                if (counter == 0){
                    vertex_position_indicies.push_back(tmp_glint);
                }
                else if (counter == 1){
                    vertex_texcoord_indicies.push_back(tmp_glint);
                }
                else if (counter == 2){
                    vertex_normal_indicies.push_back(tmp_glint);
                }
                if (ss.peek() == '/'){
                    counter++;
                    ss.ignore(1,'/');
                }
                else if (ss.peek() == ' '){
                    counter++;
                    ss.ignore(1,' ');
                }
                if (counter > 2){
                    counter = 0;
                }
            }
        }
        else{}
    }
    vertices.resize(vertex_position_indicies.size(), vertex());
    for (unsigned int i = 0; i < vertices.size(); i++){
        vertices[i].position = vertex_positions[vertex_position_indicies[i]-1];
        vertices[i].texCoord = vertex_texCoords[vertex_texcoord_indicies[i]-1];
        vertices[i].normal = vertex_normals[vertex_normal_indicies[i]-1];
        vertices[i].color = color;
    }
    return vertices;
}

#endif