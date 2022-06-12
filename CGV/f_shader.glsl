#version 330

//uniform vec3 objectColor;
//uniform vec3 lightColor;
//uniform vec3 lightPos;
uniform vec3 viewPos;

in vec3 Normal;
in vec3 FragPos;
in vec3 Color;
in vec2 texCoord;

struct Material{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shine;
};

uniform Material material;

struct Light{
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
};

uniform Light light;

struct pointLight{
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 color;
    float constant;
    float linear;
    float quadratic;
};

#define LIGHTS 4

uniform pointLight point[LIGHTS];

uniform sampler2D tex;

out vec4 FragColor;

vec3 directionalLighting(){
    // ambient
    vec3 ambient = light.ambient * material.ambient;
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * (diff * material.diffuse);
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shine);
    vec3 specular = light.specular * (spec * material.specular);
    return (ambient + diffuse + specular);
}

vec3 pointLighting(pointLight pointL){
    // ambient
    vec3 ambient = pointL.ambient * material.ambient;
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(pointL.position - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = pointL.diffuse * (diff * material.diffuse);
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = pointL.specular * (spec * material.specular);

    float dist = length(pointL.position - FragPos);
    float attenuation = 1.0 / (pointL.constant + pointL.linear*dist + pointL.quadratic*dist*dist);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}

void main(){
    
    
    vec3 result = directionalLighting();
    //vec3 result = vec3(0.f);
    for (int i = 0; i < LIGHTS; i++){
        result += pointLighting(point[i]);
    }
    

    result = result * light.color * Color;

    //FragColor = vec4(result, 1.0);

    FragColor = texture(tex, texCoord) * vec4(result, 1.0);

    //FragColor = vec4(Color, 1.0);
    //FragColor = vec4(150.0, 20.0, 0.0, 1.0);
    //FragColor = texture(tex, texCoord) * vec4(Color, 0.9f);
}