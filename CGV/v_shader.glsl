#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

//uniform vec3 aColor;

out vec3 FragPos;
out vec3 Normal;
out vec2 texCoord;
out vec3 Color;

void main(){
    gl_Position = P * V * M * vec4(aPos, 1.0);
    FragPos = vec3(M * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(M))) * aNormal;
    //Color = aColor;
    Color = normalize(aColor);
    texCoord = aTexCoord;
}