#ifndef WORMS
#define WORMS

#include "libraries.h"
#include "model.h"
#include "light.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

enum material_num {CHROME = 0, RUBBER, PLASTIC};
enum shader_num {CORE = 0, LIGHTING};
enum model_num {ROCKET = 0, CROSS, TERRA, ARROW, SPHERE, VOID, LAMP, GRAVEV, GRAVET, WIND};
enum texture_num {BAZ = 0, HAND, EYE, VIKING, TOPPAT, DIRT, WATER, POST, GRAVE};

class Game{
private:
    GLFWwindow* window;
    const int WINDOW_WIDTH;
    const int WINDOW_HEIGHT;
    std::vector<Material*> materials;
    std::vector<Model*> models;
    std::vector<Model*> worms;
    std::vector<Shader*> shaders;
    std::vector<PointLight*> pointLights;
    std::vector<Texture*> textures;
    DirectionalLight* dirLight;
    Camera camera;
    glm::mat4 V;
    glm::mat4 P;
    //glm::mat4 M;

    int r = 0;
    float angle = 5.f;
    float move = 1.f;
    bool back = false;
    bool movement = false;
    unsigned rot = 0;
    unsigned change = 1;
    bool wormView = false;
    float aim = -2.f;
    float arc = -5.f;
    float arr = 5.f;
    bool arrBack = false;
    unsigned currWorm = 0;
    unsigned lastWorm = 0;
    float power = 0;
    float shoot = -1;
    bool top = false;
    bool fly = false;
    float explode = 0.f;
    int hit = -1;
    float wind = 0.04f;
    float water = 0.f;
    bool waterBack = false;
    int firstTurn = 6;
    std::vector<glm::vec3> fronts = { glm::vec3(-1.f, 0.f, 0.f), glm::vec3(-0.86f, 0.f, -0.5f), glm::vec3(-0.5f, 0.f, -0.86f),
    glm::vec3(0.f, 0.f, -1.f), glm::vec3(0.5f, 0.f, -0.86f), glm::vec3(0.86f, 0.f, -0.5f), glm::vec3(1.f, 0.f, 0.f),
    glm::vec3(0.86f, 0.f, 0.5f), glm::vec3(0.5f, 0.f, 0.86f), glm::vec3(0.f, 0.f, 1.f), glm::vec3(-0.5f, 0.f, 0.86f), glm::vec3(-0.86f, 0.f, 0.5f) };
    unsigned windIndex = rand() % 12;
    glm::vec3 windDir = this->fronts[this->windIndex];

    void initGLFW();
    void initGLEW();
    void initModels();
    void initWindow();
    void initMatrices();
    void initOpenGLProgram();
    void initShaders();
    void initMaterials();
    void initUniforms();
    void initLights();
    void initTextures();
    void freeOpenGLProgram();
    void mouseCall();
    void keyboardCall();
    void updateUniforms();
    int collisionDetection(glm::vec3 position, int ignore);
    bool terrainCollision(glm::vec3 position);
    void findHight(float x, float z);
    void terrainBlock(float x, float z);

public:
    Game(const int WINDOW_WIDTH, const int WINDOW_HEIGHT);
    virtual ~Game();

    int getShouldClose();
    void drawScene();
    void input();

    bool firstMouse = true;
    float lastX;
    float lastY;

    float dt = 0.f;
    float lastTime = 0.f;
};

void error_callback(int error, const char* description);

#endif