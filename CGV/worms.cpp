#include "Worms.h"

void error_callback(int error, const char* description){
    fputs(description, stderr);
}

void Game::keyboardCall(){
    if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(this->window, true);

    if (glfwGetKey(this->window, GLFW_KEY_W) == GLFW_PRESS) {
        if (!wormView) this->camera.processKeyboard(FORWARD, this->dt);
        else {
            this->aim++;
            if (this->aim > 4.f) this->aim = 4.f;
        }
    }
    else if (glfwGetKey(this->window, GLFW_KEY_S) == GLFW_PRESS) {
        if (!wormView) this->camera.processKeyboard(BACKWARD, this->dt);
        else {
            this->aim--;
            if (this->aim < -12.f) this->aim = -12.f;
        }
    }
    else if (glfwGetKey(this->window, GLFW_KEY_A) == GLFW_PRESS)
        this->camera.processKeyboard(LEFT, this->dt);
    else if (glfwGetKey(this->window, GLFW_KEY_D) == GLFW_PRESS)
        this->camera.processKeyboard(RIGHT, this->dt);
    else if (glfwGetKey(this->window, GLFW_KEY_O) == GLFW_PRESS)
        this->camera.processZoom(1.f);
    else if (glfwGetKey(this->window, GLFW_KEY_P) == GLFW_PRESS)
        this->camera.processZoom(-1.f);
    else if (glfwGetKey(this->window, GLFW_KEY_M) == GLFW_PRESS) {
        this->camera.changeView(-1);
        this->wormView = true;
    }
    else if (glfwGetKey(this->window, GLFW_KEY_N) == GLFW_PRESS) {
        this->camera.changeView(1);
        this->wormView = false;
    }
    else if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_PRESS) {
        if (this->rot == 0) {
            this->r--;
            this->movement = true;
            this->arr = 0.f;
            this->models[ARROW]->scale(glm::vec3(0.f));
        }
    }
    else if (glfwGetKey(this->window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        if (this->rot == 0) this->rot = 1;       
    }
    else if (glfwGetKey(this->window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        if (this->rot == 0) this->rot = 2;
    }
    else if (glfwGetKey(this->window, GLFW_KEY_V) == GLFW_RELEASE && this->fly) {
        this->lastWorm = this->currWorm;
        this->shoot = 1000.f;
        this->currWorm++;
        if (this->currWorm > std::size(this->worms) - 1) {
            this->currWorm = 0;
        }
        this->fly = false;
        this->arr = 5.f;
        this->models[ROCKET]->changePosition(glm::vec3(0.f, 1.5f, 0.f), NULL);
        this->models[CROSS]->setAngle(this->worms[this->currWorm]->getAngle());
    }
    else if (glfwGetKey(this->window, GLFW_KEY_V) == GLFW_PRESS) {
        this->power += 10.f;
        if (this->power > 1000.f) this->power = 1000.f;
        this->fly = true;
    }
    else if (glfwGetKey(this->window, GLFW_KEY_UP) == GLFW_RELEASE) {
        if (this->rot == 0 && !this->movement) this->r++;        
    }
}



void Game::initGLFW(){
    if (!glfwInit()) {
		fprintf(stderr, "Can't initialize GLFW.\n");
		exit(EXIT_FAILURE); 
	}
}

void Game::initGLEW(){
    GLenum err;
    if ((err=glewInit()) != GLEW_OK) {
		fprintf(stderr, "Can't initialize GLEW.\n");
		exit(EXIT_FAILURE);
	}
}

void Game::initWindow(){
    this->window = glfwCreateWindow(this->WINDOW_WIDTH, this->WINDOW_HEIGHT, "Worms", NULL, NULL);
    if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
    glfwMakeContextCurrent(this->window);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Game::initOpenGLProgram(){
    glEnable(GL_DEPTH_TEST);
    //glEnable(GL_CULL_FACE);
}

void Game::initMatrices(){
    this->V = glm::lookAt(glm::vec3(0.0f,0.0f,-5.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));
    this->P = glm::perspective(glm::radians(this->camera.getZoom()), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
    //this->M = glm::mat4(1.f);
}

void Game::initShaders(){
    initShader(this->shaders, "v_shader.glsl", NULL, "f_shader.glsl");
    //initShader(this->shaders, "light_v_shader.glsl", NULL, "light_f_shader.glsl");
}

Game::Game(const int WINDOW_WIDTH, const int WINDOW_HEIGHT):
    WINDOW_WIDTH(WINDOW_WIDTH),
    WINDOW_HEIGHT(WINDOW_HEIGHT){
    this->window = nullptr;
    glfwSetErrorCallback(error_callback);
    this->initGLFW();
    this->initWindow();
    this->initGLEW();
    this->initMatrices();
    this->initMaterials();
    this->initTextures();
    this->initModels();
    this->initShaders();
    this->initLights();
    this->initUniforms();
    this->initOpenGLProgram();
}

Game::~Game(){
    glfwDestroyWindow(this->window);
    glfwTerminate();
    freeOpenGLProgram();
}

void Game::drawScene(){
    glClearColor(58.0 / 255.0, 168.0 / 255.0, 254.0 / 255.0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    this->input();
    //this->shaders[CORE]->setMat4fv(this->M, "M");
    
    //this->models[BAZOOKA]->translate(glm::vec3(0.f, -1.5f, 2.f));
    /*float dt = glfwGetTime();
    this->models[0]->rotateMesh(1, glm::vec3(0.f, 0.f, 1.f), glm::radians(r) * dt);
    this->models[0]->translateMesh(1, glm::vec3(0.f, r/5.f, 0.f));
    this->models[0]->rotateMesh(2, glm::vec3(0.f, 0.f, 1.f), glm::radians(r) * dt);
    this->models[0]->translateMesh(2, glm::vec3(0.f, r/5.f, 0.f));
    this->models[0]->rotateMesh(3, glm::vec3(0.f, 0.f, 1.f), glm::radians(r) * dt);
    this->models[0]->translateMesh(3, glm::vec3(0.f, r/5.f, 0.f));*/

    
    //this->models[ROCKET]->scale(glm::vec3(0.1f));

    while (this->worms[this->currWorm]->getHealth() <= 0) {
        this->currWorm++;
        this->arr = 5.f;
        if (this->currWorm > std::size(this->worms) - 1) {
            this->currWorm = 0;
        }
    }
    
    if (this->arr > 0.f && !this->arrBack) {
        //this->models[ARROW]->scale(glm::vec3(0.1f));
        this->models[ARROW]->setPosition(this->worms[this->currWorm]->getPos() + glm::vec3(0.f, arr, 0.f));
        //this->models[ARROW]->scale(glm::vec3(7.f));
        this->arr += 0.1f;
        if (this->arr > 15.f) this->arrBack = true;
        this->models[ARROW]->draw(this->shaders[CORE]);
    }
    else if (this->arrBack) {
            this->arr -= 0.1f;
            //this->models[ARROW]->scale(glm::vec3(0.1f));
            this->models[ARROW]->setPosition(this->worms[this->currWorm]->getPos() + glm::vec3(0.f, arr, 0.f));
            //this->models[ARROW]->scale(glm::vec3(7.f));
            if (this->arr < 5.f) {
                this->arrBack = false;
            }
            
            this->models[ARROW]->draw(this->shaders[CORE]);
    }

    

    if (r < 0) {
        r = 0;
    }
    if (r > 3) {
        r = 3;
    }
    
    if (this->movement) {
        this->worms[this->currWorm]->translateMesh(1, glm::vec3(move * 0.001f, -move / 1000.f, 0.f));
        this->worms[this->currWorm]->translateMesh(2, glm::vec3(0.f, -move / 1000.f, 0.f));
        this->worms[this->currWorm]->translateMesh(3, glm::vec3(0.f, -move / 1000.f, 0.f));
        /*this->worms[this->currWorm]->rotateMesh(1, glm::vec3(0.f, 0.f, 1.f), glm::radians(-move) / 50.f);
        this->worms[this->currWorm]->rotateMesh(2, glm::vec3(0.f, 0.f, 1.f), glm::radians(-move) / 50.f);
        this->worms[this->currWorm]->rotateMesh(3, glm::vec3(0.f, 0.f, 1.f), glm::radians(-move) / 50.f);*/
        if (this->move < 51.f && !this->back) {
            this->move += 1.f;
            this->worms[this->currWorm]->changePosition(glm::vec3(0.f), 0.01f, true);
            if (move == 51.f) this->back = true;
        }
        else {
            this->move -= 1.f;
            if (this->move == 1.f) {
                this->back = false;
                this->terrainBlock(this->worms[this->currWorm]->getPos().x, this->worms[this->currWorm]->getPos().z);
                this->findHight(this->worms[this->currWorm]->getPos().x, this->worms[this->currWorm]->getPos().z);
                this->movement = false;
            }
        }
    }

    //this->worms[this->currWorm]->translate(glm::vec3(0.f, this->models[TERRA]->findHeight(this->worms[this->currWorm]->getPos().x, this->worms[this->currWorm]->getPos().z), 0.f));
    this->findHight(this->worms[this->currWorm]->getPos().x, this->worms[this->currWorm]->getPos().z);
    //this->terrain->draw(this->shaders[CORE]);

    if (this->rot == 1) {
        this->change++;
        this->worms[this->currWorm]->changeRotation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        this->models[CROSS]->changeRotation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        this->models[ROCKET]->changeRotation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, 1.f, 0.f));
        if (this->change == 30) {
            this->rot = 0;
            this->change = 1;
            this->worms[this->currWorm]->updateFront(1);
            this->models[ROCKET]->updateFront(1);
        }
    }
    else if (this->rot == 2) {
        this->change++;
        this->worms[this->currWorm]->changeRotation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
        this->models[CROSS]->changeRotation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
        this->models[ROCKET]->changeRotation(glm::vec3(0.f, 1.f, 0.f), glm::vec3(0.f, -1.f, 0.f));
        if (this->change == 30) {
            this->rot = 0.f;
            this->change = 1;
            this->worms[this->currWorm]->updateFront(-1);
            this->models[ROCKET]->updateFront(-1);
        }
    }

    if (this->shoot > 0) {

        if (this->arc < this->aim && !this->top) {
            this->arc += 0.08f;
            this->models[ROCKET]->changePosition(glm::vec3(0.f, 0.08f, 0.f), NULL);
            this->models[ROCKET]->changePosition(windDir * wind, NULL);
        }
        if (this->arc >= this->aim) {
            this->top = true;
        }
        this->models[ROCKET]->changePosition(glm::vec3(0.f), this->power * 0.0008f, true);
        if (this->top) {
            this->models[ROCKET]->changePosition(glm::vec3(0.f, -0.08f, 0.f), NULL);
            this->models[ROCKET]->changePosition(windDir * wind, NULL);
            this->arc = 1.5f;
        }
        if (collisionDetection(this->models[ROCKET]->getPos(), this->lastWorm) != -1 || abs(this->models[ROCKET]->getPos().x) >= 50.0f || abs(this->models[ROCKET]->getPos().z) >= 50.0f || this->terrainCollision(this->models[ROCKET]->getPos())) {
            this->hit = collisionDetection(this->models[ROCKET]->getPos(), this->lastWorm);
            this->explode = 5.f;
            //this->models[ROCKET]->changePosition(glm::vec3(0.f, -50.f, 0.f), NULL);
            this->models[ROCKET]->setAngle(this->worms[this->currWorm]->getAngle());
            this->models[ROCKET]->setFront(this->worms[this->currWorm]->getFrontIndex());
            this->aim = 1.5f;
            this->shoot = 0.f;
            this->top = false;
            this->hit = collisionDetection(this->models[ROCKET]->getPos(), -1);
            this->windIndex = rand() % 12;
            this->windDir = this->fronts[this->windIndex];
            if (this->hit > -1) {
                this->worms[this->hit]->changeHealth(-20);
            }
        }
        else {
            this->shoot -= 1.0f;
            this->models[ROCKET]->draw(this->shaders[CORE], glm::radians(-this->shoot+920.f));
            this->camera.setFront(this->models[ROCKET]->getFront() - glm::vec3(0.f, 0.3f, 0.f));
            this->camera.setPosition(this->models[ROCKET]->getPos() - (this->models[ROCKET]->getFront() * 3.f) + glm::vec3(0.f, 1.2f, 0.f));
        }
    }
    else {
        this->models[ROCKET]->setPosition(this->worms[this->currWorm]->getPos());
    }

    if (this->explode > 0.f) {
        if (this->explode > 4.9f) this->models[SPHERE]->setPosition(this->models[ROCKET]->getPos());
        if ((int)this->explode % 2 == 1) this->pointLights[2]->setPos(this->models[SPHERE]->getPos());
        else this->pointLights[2]->setPos(glm::vec3(100.f));
        this->models[SPHERE]->draw(this->shaders[CORE]);
        //this->camera.setFront(glm::vec3(0.f, 2.f, 0.f));
        //this->camera.setPosition(this->models[ROCKET]->getPos() - this->models[ROCKET]->getFront() * 2.f);
        this->explode -= 0.1f;
        this->power = 0.f;
    }

    for (unsigned i = 0; i < std::size(this->worms); i++) {
        int j = 0;
        if (this->currWorm == i) j = this->r;
        if (this->worms[i]->getHealth() > 0) this->worms[i]->draw(this->shaders[CORE], j);
        else if (i % 2 == 0) {
            this->models[GRAVEV]->translate(this->worms[i]->getPos());
            this->models[GRAVEV]->draw(this->shaders[CORE]);
        }
        else {
            this->models[GRAVET]->translate(this->worms[i]->getPos());
            this->models[GRAVET]->draw(this->shaders[CORE]);
        }
    }

    this->models[WIND]->translate(glm::vec3(0.f, 15.f, 0.f));
    if (this->wormView && this->explode <= 0.f && this->power == 0.f) {
        this->pointLights[2]->setPos(glm::vec3(100.f));
        this->camera.setFront(this->worms[this->currWorm]->getFront());
        this->camera.setPosition(this->worms[this->currWorm]->getPos() + this->worms[this->currWorm]->getFront() + glm::vec3(0.f, 1.5f, 0.f));
        this->models[CROSS]->setPosition(this->worms[this->currWorm]->getPos() + 30.f * this->worms[this->currWorm]->getFront());
        this->models[CROSS]->changePosition(glm::vec3(0.f, aim, 0.f), NULL);
        this->models[CROSS]->draw(this->shaders[CORE]);
    }
    
    if (this->waterBack) {
        this->water -= 0.1;
        if (this->water <= -10.f) this->waterBack = false;
    }
    else {
        this->water += 0.1;
        if (this->water >= 10.f) {
            this->waterBack = true;
        }
    }
    this->models[TERRA]->draw(this->shaders[CORE]);
    this->models[VOID]->draw(this->shaders[CORE], this->water);
    this->models[LAMP]->draw(this->shaders[CORE], 0);
    this->models[LAMP]->draw(this->shaders[CORE], 1);
    this->models[LAMP]->draw(this->shaders[CORE], 2);
    this->models[WIND]->rotate(glm::vec3(0.f, 0.f, 1.f), glm::radians(-90.f));
    this->models[WIND]->rotate(glm::vec3(1.f, 0.f, 0.f), glm::radians(30.f * this->windIndex));
    this->models[WIND]->draw(this->shaders[CORE]);

    glfwSwapBuffers(this->window);
    glfwPollEvents();
    this->updateUniforms();    

    if (this->firstTurn >= 0) {
        this->firstTurn--;
        this->currWorm++;
        if (this->currWorm > 5) this->currWorm = 0;
    }

}

int Game::getShouldClose(){
    return glfwWindowShouldClose(this->window);
}

void Game::freeOpenGLProgram(){
    freeShader(this->shaders);
    freeMaterial(this->materials);
}

void Game::initModels(){
    this->worms.push_back(new Model("models/worm_down.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm", glm::vec3(-7.f, 5.f, -5.f)));
    this->worms[0]->addMesh("models/worm_up.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm");
    this->worms[0]->addMesh("models/eyes.obj", this->materials[RUBBER], this->textures[EYE], glm::vec3(255.f), "worm");
    this->worms[0]->addMesh("models/viking.obj", this->materials[CHROME], this->textures[VIKING], glm::vec3(255.f), "worm");
    this->worms[0]->addMesh("models/bazooka.obj", this->materials[CHROME], this->textures[BAZ], glm::vec3(255.f), "baz");
    this->worms[0]->addMesh("models/hand.obj", this->materials[RUBBER], this->textures[HAND], glm::vec3(1.f), "baz");
    this->models.push_back(new Model("models/rocket.obj", this->materials[RUBBER], this->textures[TOPPAT], glm::vec3(255.f), "rocket"));
    this->worms.push_back(new Model("models/worm_down.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm", glm::vec3(16.f, 0.f, -2.f)));
    this->worms[1]->addMesh("models/worm_up.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm");
    this->worms[1]->addMesh("models/eyes.obj", this->materials[RUBBER], this->textures[EYE], glm::vec3(255.f), "worm");
    this->worms[1]->addMesh("models/toppat.obj", this->materials[CHROME], this->textures[TOPPAT], glm::vec3(255.f), "worm");
    this->worms[1]->addMesh("models/bazooka.obj", this->materials[CHROME], this->textures[BAZ], glm::vec3(255.f), "baz");
    this->worms[1]->addMesh("models/hand.obj", this->materials[RUBBER], this->textures[HAND], glm::vec3(1.f), "baz");
    this->worms.push_back(new Model("models/worm_down.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm", glm::vec3(-45.f, 5.f, -31.f)));
    this->worms[2]->addMesh("models/worm_up.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm");
    this->worms[2]->addMesh("models/eyes.obj", this->materials[RUBBER], this->textures[EYE], glm::vec3(255.f), "worm");
    this->worms[2]->addMesh("models/viking.obj", this->materials[CHROME], this->textures[VIKING], glm::vec3(255.f), "worm");
    this->worms[2]->addMesh("models/bazooka.obj", this->materials[CHROME], this->textures[BAZ], glm::vec3(255.f), "baz");
    this->worms[2]->addMesh("models/hand.obj", this->materials[RUBBER], this->textures[HAND], glm::vec3(1.f), "baz");
    this->worms.push_back(new Model("models/worm_down.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm", glm::vec3(42.f, 0.f, 15.f)));
    this->worms[3]->addMesh("models/worm_up.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm");
    this->worms[3]->addMesh("models/eyes.obj", this->materials[RUBBER], this->textures[EYE], glm::vec3(255.f), "worm");
    this->worms[3]->addMesh("models/toppat.obj", this->materials[CHROME], this->textures[TOPPAT], glm::vec3(255.f), "worm");
    this->worms[3]->addMesh("models/bazooka.obj", this->materials[CHROME], this->textures[BAZ], glm::vec3(255.f), "baz");
    this->worms[3]->addMesh("models/hand.obj", this->materials[RUBBER], this->textures[HAND], glm::vec3(1.f), "baz");
    this->worms.push_back(new Model("models/worm_down.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm", glm::vec3(2.f, 5.f, 38.f)));
    this->worms[4]->addMesh("models/worm_up.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm");
    this->worms[4]->addMesh("models/eyes.obj", this->materials[RUBBER], this->textures[EYE], glm::vec3(255.f), "worm");
    this->worms[4]->addMesh("models/viking.obj", this->materials[CHROME], this->textures[VIKING], glm::vec3(255.f), "worm");
    this->worms[4]->addMesh("models/bazooka.obj", this->materials[CHROME], this->textures[BAZ], glm::vec3(255.f), "baz");
    this->worms[4]->addMesh("models/hand.obj", this->materials[RUBBER], this->textures[HAND], glm::vec3(1.f), "baz");
    this->worms.push_back(new Model("models/worm_down.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm", glm::vec3(-30.f, 0.f, 15.f)));
    this->worms[5]->addMesh("models/worm_up.obj", this->materials[PLASTIC], this->textures[HAND], glm::vec3(255.f, 175.f, 175.f), "worm");
    this->worms[5]->addMesh("models/eyes.obj", this->materials[RUBBER], this->textures[EYE], glm::vec3(255.f), "worm");
    this->worms[5]->addMesh("models/toppat.obj", this->materials[CHROME], this->textures[TOPPAT], glm::vec3(255.f), "worm");
    this->worms[5]->addMesh("models/bazooka.obj", this->materials[CHROME], this->textures[BAZ], glm::vec3(255.f), "baz");
    this->worms[5]->addMesh("models/hand.obj", this->materials[RUBBER], this->textures[HAND], glm::vec3(1.f), "baz");
    this->models.push_back(new Model("models/crosshair.obj", this->materials[CHROME], this->textures[HAND], glm::vec3(158.f, 0.f, 0.f), "cross"));
    this->models.push_back(new Model("models/terrain.obj", this->materials[CHROME], this->textures[DIRT], glm::vec3(255.f), "terra"));
    this->models.push_back(new Model("models/arrow.obj", this->materials[RUBBER], this->textures[HAND], glm::vec3(158.f, 0.f, 0.f), "arrow"));
    this->models.push_back(new Model("models/sphere.obj", this->materials[CHROME], this->textures[DIRT], glm::vec3(212.f, 221.f, 0.f), "sphere"));
    this->models.push_back(new Model("models/water.obj", this->materials[RUBBER], this->textures[WATER], glm::vec3(47.f, 91.f, 181.f), "water"));
    this->models.push_back(new Model("models/lamp.obj", this->materials[CHROME], this->textures[POST], glm::vec3(255.f), "lamp"));
    this->models.push_back(new Model("models/grave.obj", this->materials[CHROME], this->textures[GRAVE], glm::vec3(255.f), "grave"));
    this->models[GRAVEV]->addMesh("models/graveViking.obj", this->materials[CHROME], this->textures[VIKING], glm::vec3(255.f), "grave");
    this->models.push_back(new Model("models/grave.obj", this->materials[CHROME], this->textures[GRAVE], glm::vec3(255.f), "grave"));
    this->models[GRAVET]->addMesh("models/graveToppat.obj", this->materials[CHROME], this->textures[TOPPAT], glm::vec3(255.f), "grave");
    this->models.push_back(new Model("models/arrow.obj", this->materials[RUBBER], this->textures[HAND], glm::vec3(255.f, 195.f, 0.f), "wind"));
}

void Game::initMaterials(){
    this->materials.push_back(new Material(glm::vec3(0.25f), glm::vec3(0.4f), glm::vec3(0.774597f), 0.6f));
    this->materials.push_back(new Material(glm::vec3(0.05f), glm::vec3(0.5f), glm::vec3(0.7f), 0.078125f));
    this->materials.push_back(new Material(glm::vec3(0.0f), glm::vec3(0.55f), glm::vec3(0.7f), 0.25f));
}

void Game::initLights(){
    this->dirLight = new DirectionalLight(glm::vec3(-0.2f, -1.f, -0.3f), glm::vec3(1.f, 1.f, 1.f), glm::vec3(0.9f), glm::vec3(2.f), glm::vec3(1.f));
    this->pointLights.push_back(new PointLight(glm::vec3(-30.f, 13.f, -30.5f), 1.f, 0.014f, 0.07f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(10.f), glm::vec3(25.f), glm::vec3(1.f)));
    this->pointLights.push_back(new PointLight(glm::vec3(-10.f, 11.f, 15.f), 1.f, 0.014f, 0.07f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(10.f), glm::vec3(25.f), glm::vec3(1.f)));
    this->pointLights.push_back(new PointLight(glm::vec3(-100.f, 100.f, 5.f), 1.f, 0.05f, 0.0075f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(10.f), glm::vec3(25.f), glm::vec3(1.f)));
    this->pointLights.push_back(new PointLight(glm::vec3(28.f, 2.f, -34.f), 1.f, 0.014f, 0.07f, glm::vec3(1.f, 1.f, 1.f), glm::vec3(10.f), glm::vec3(25.f), glm::vec3(1.f)));
}

void Game::initUniforms(){
    this->shaders[CORE]->setMat4fv(this->V, "V");
    this->shaders[CORE]->setVec3f(this->camera.getPos(), "viewPos");
    this->shaders[CORE]->setMat4fv(this->P, "P");
    this->shaders[CORE]->setVec3f(this->dirLight->getDir(), "light.direction");
    this->shaders[CORE]->setVec3f(this->dirLight->getColor(), "light.color");
    this->shaders[CORE]->setVec3f(this->dirLight->getAmb(), "light.ambient");
    this->shaders[CORE]->setVec3f(this->dirLight->getSpec(), "light.specular");
    this->shaders[CORE]->setVec3f(this->dirLight->getDiff(), "light.diffuse");
    this->shaders[CORE]->setVec3f(this->pointLights[0]->getPos(), "point[0].position");
    this->shaders[CORE]->setVec3f(this->pointLights[0]->getColor(), "point[0].color");
    this->shaders[CORE]->setVec3f(this->pointLights[0]->getAmb(), "point[0].ambient");
    this->shaders[CORE]->setVec3f(this->pointLights[0]->getSpec(), "point[0].specular");
    this->shaders[CORE]->setVec3f(this->pointLights[0]->getDiff(), "point[0].diffuse");
    this->shaders[CORE]->setFloat(this->pointLights[0]->getConst(), "point[0].constant");
    this->shaders[CORE]->setFloat(this->pointLights[0]->getLin(), "point[0].linear");
    this->shaders[CORE]->setFloat(this->pointLights[0]->getQuad(), "point[0].quadratic");
    this->shaders[CORE]->setVec3f(this->pointLights[1]->getPos(), "point[1].position");
    this->shaders[CORE]->setVec3f(this->pointLights[1]->getColor(), "point[1].color");
    this->shaders[CORE]->setVec3f(this->pointLights[1]->getAmb(), "point[1].ambient");
    this->shaders[CORE]->setVec3f(this->pointLights[1]->getSpec(), "point[1].specular");
    this->shaders[CORE]->setVec3f(this->pointLights[1]->getDiff(), "point[1].diffuse");
    this->shaders[CORE]->setFloat(this->pointLights[1]->getConst(), "point[1].constant");
    this->shaders[CORE]->setFloat(this->pointLights[1]->getLin(), "point[1].linear");
    this->shaders[CORE]->setFloat(this->pointLights[1]->getQuad(), "point[1].quadratic");
    this->shaders[CORE]->setVec3f(this->pointLights[2]->getPos(), "point[2].position");
    this->shaders[CORE]->setVec3f(this->pointLights[2]->getColor(), "point[2].color");
    this->shaders[CORE]->setVec3f(this->pointLights[2]->getAmb(), "point[2].ambient");
    this->shaders[CORE]->setVec3f(this->pointLights[2]->getSpec(), "point[2].specular");
    this->shaders[CORE]->setVec3f(this->pointLights[2]->getDiff(), "point[2].diffuse");
    this->shaders[CORE]->setFloat(this->pointLights[2]->getConst(), "point[2].constant");
    this->shaders[CORE]->setFloat(this->pointLights[2]->getLin(), "point[2].linear");
    this->shaders[CORE]->setFloat(this->pointLights[2]->getQuad(), "point[2].quadratic");
    this->shaders[CORE]->setVec3f(this->pointLights[3]->getPos(), "point[3].position");
    this->shaders[CORE]->setVec3f(this->pointLights[3]->getColor(), "point[3].color");
    this->shaders[CORE]->setVec3f(this->pointLights[3]->getAmb(), "point[3].ambient");
    this->shaders[CORE]->setVec3f(this->pointLights[3]->getSpec(), "point[3].specular");
    this->shaders[CORE]->setVec3f(this->pointLights[3]->getDiff(), "point[3].diffuse");
    this->shaders[CORE]->setFloat(this->pointLights[3]->getConst(), "point[3].constant");
    this->shaders[CORE]->setFloat(this->pointLights[3]->getLin(), "point[3].linear");
    this->shaders[CORE]->setFloat(this->pointLights[3]->getQuad(), "point[3].quadratic");
}


void Game::initTextures() {
    this->textures.push_back(new Texture("textures/baz.png"));
    this->textures.push_back(new Texture("textures/hand.png"));
    this->textures.push_back(new Texture("textures/eye.png"));
    this->textures.push_back(new Texture("textures/viking.png"));
    this->textures.push_back(new Texture("textures/toppat.png"));
    this->textures.push_back(new Texture("textures/dirt.png"));
    this->textures.push_back(new Texture("textures/water.png"));
    this->textures.push_back(new Texture("textures/lamp_post.png"));
    this->textures.push_back(new Texture("textures/grave.png"));
}

void Game::input(){
    float currTime = glfwGetTime();
    this->dt = currTime - this->lastTime;
    this->lastTime = currTime;
    this->keyboardCall();
    if (!this->wormView) this->mouseCall();
    //this->scrollBack();
}

void Game::mouseCall(){
    double xpos, ypos;
    glfwGetCursorPos(this->window, &xpos, &ypos);
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;
    this->camera.processMouse(xoffset, yoffset);
}

void Game::updateUniforms() {
    this->shaders[CORE]->setMat4fv(this->camera.getView(), "V");
    this->shaders[CORE]->setVec3f(this->camera.getPos(), "viewPos");
    this->shaders[CORE]->setMat4fv(glm::perspective(glm::radians(this->camera.getZoom()), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f), "P");
    this->shaders[CORE]->setVec3f(this->pointLights[2]->getPos(), "point[2].position");
}

int Game::collisionDetection(glm::vec3 position, int ignore) {
    for (int i = 0; i < std::size(worms); i++) {
        if (float j = pow((pow(position.x-this->worms[i]->getPos().x,2.f) + pow(position.y - this->worms[i]->getPos().y, 2.f) + pow(position.z - this->worms[i]->getPos().z, 2.f)), 0.5) < 2.f) {
            if (i != ignore) {
                return i;
            }
        }
    }
    return -1;
}

void Game::findHight(float x, float z) {
    if (x >= -50.f && x <= -25.f && z >= -50.f && z <= -25.f) {
        this->worms[this->currWorm]->setHeight(11.15f);
        return;
    }
    if (x >= 12.5f && x <= 50.f && z >= -50.f && z <= -25.f) {
        this->worms[this->currWorm]->setHeight(0.f);
        return;
    }
    if (x >= -50.f && x <= -12.5f && z >= 37.5f && z <= 50.f) {
        this->worms[this->currWorm]->setHeight(0.f);
        return;
    }
    if (x >= -25.f && x <= -12.5f && z >= 25.f && z <= 37.5f) {
        this->worms[this->currWorm]->setHeight(0.f);
        return;
    }
    if (x >= 37.5f && x <= 50.f && z >= 37.5f && z <= 50.f) {
        this->worms[this->currWorm]->setHeight(0.f);
        return;
    }
    if (x >= -50.f && x <= -25.f && z >= -12.5f && z <= 0.f) {
        this->worms[this->currWorm]->setHeight(6.f);
        return;
    }
    if (x >= -12.5f && x <= 0.f && z >= -50.f && z <= -25.f) {
        this->worms[this->currWorm]->setHeight(6.f);
        return;
    }
    if (x >= 12.5f && x <= 25.f && z >= 0.f && z <= 25.f) {
        this->worms[this->currWorm]->setHeight(5.3f);
        return;
    }
    if (x >= -25.f && x <= 0.f && z >= 0.f && z <= 25.f) {
        this->worms[this->currWorm]->setHeight(9.5f);
        return;
    }
    if (x >= -50.f && x <= -25.f && z >= 12.5f && z <= 25.f) {
        this->worms[this->currWorm]->setHeight(2.45f);
        return;
    }
    if (x >= -12.5f && x <= 25.f && z >= -12.5f && z <= 0.f) {
        this->worms[this->currWorm]->setHeight(2.2f);
        return;
    }
    if (x >= 0.f && x <= 25.f && z >= -25.f && z <= -12.5f) {
        this->worms[this->currWorm]->setHeight(2.2f);
        return;
    }
    if (x >= 0.f && x <= 25.f && z >= 25.f && z <= 50.f) {
        this->worms[this->currWorm]->setHeight(1.8f);
        return;
    }
    if (x >= 37.5f && x <= 50.f && z >= 0.f && z <= 25.f) {
        this->worms[this->currWorm]->setHeight(1.8f);
        return;
    }
    if (x > 50.f || x < -50.f || z > 50.f || z < -50.f) {
        this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.1f, 0.f), NULL);
        this->worms[this->currWorm]->changeHealth(-1.f);
        return;
    }
    if (x >= -50.f && x <= 0.f && z >= -25.f && z <= -12.5f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().z < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.00205f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.00205f, 0.f), NULL);
        }
        return;
    }
    if (x >= -50.f && x <= -25.f && z >= 0.f && z <= 12.5f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().z < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0014f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0014f, 0.f), NULL);
        }
        return;
    }
    if (x >= -50.f && x <= -25.f && z >= 25.f && z <= 37.5f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().z < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0011f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0011f, 0.f), NULL);
        }
        return;
    }
    if (x >= -25.f && x <= -12.5f && z >= -50.f && z <= -25.f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().x < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.00195f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.00195f, 0.f), NULL);
        }
        return;
    }
    if (x >= -25.f && x <= -12.5f && z >= -25.f && z <= -12.5f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().x < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.00195f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.00195f, 0.f), NULL);
        }
        return;
    }
    if (x >= -25.f && x <= -12.5f && z >= -12.5f && z <= 0.f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().x < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0014f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0014f, 0.f), NULL);
        }
        return;
    }
    if (x >= -12.5f && x <= 0.f && z >= 25.f && z <= 50.f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().x > 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0008f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0008f, 0.f), NULL);
        }
        return;
    }
    if (x >= 0.f && x <= 12.5f && z >= -50.f && z <= -25.f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().x < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0023f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0023f, 0.f), NULL);
        }
        return;
    }
    if (x >= 25.f && x <= 37.5f && z >= 37.5 && z <= 50.f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().x < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0007f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0007f, 0.f), NULL);
        }
        return;
    }
    if (x >= 25.f && x <= 37.5f && z >= 0.f && z <= 25.f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().x < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.00143f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.00143f, 0.f), NULL);
        }
        return;
    }
    if (x >= 0.f && x <= 12.5f && z >= 0.f && z <= 25.f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().x < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0016f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0016f, 0.f), NULL);
        }
        return;
    }
    if (x >= 37.5f && x <= 50.f && z >= -12.5f && z <= 0.f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().z > 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0007f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0007f, 0.f), NULL);
        }
        return;
    }
    if (x >= 37.5f && x <= 50.f && z >= 25.f && z <= 37.5f) {
        if (this->movement) {
            if (this->worms[this->currWorm]->getFront().z < 0.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.0007f, 0.f), NULL);
            else this->worms[this->currWorm]->changePosition(glm::vec3(0.f, -0.0007f, 0.f), NULL);
        }
        return;
    }
}

bool Game::terrainCollision(glm::vec3 position) {
    if (position.x >= -50.f && position.x <= -18.75f && position.z >= -50.f && position.z <= -18.75f && position.y <= 11.f) {
        return true;
    }
    if (position.x >= 6.25f && position.x <= 50.f && position.z >= -50.f && position.z <= -18.75f && position.y <= 0.f) {
        return true;
    }
    if (position.x >= -50.f && position.x <= -6.25f && position.z >= 31.25f && position.z <= 50.f && position.y <= 0.f) {
        return true;
    }
    if (position.x >= -31.25f && position.x <= -6.25f && position.z >= 18.75f && position.z <= 31.25f && position.y <= 0.f) {
        return true;
    }
    if (position.x >= 31.25f && position.x <= 50.f && position.z >= 31.25f && position.z <= 50.f && position.y <= 0.f) {
        return true;
    }
    if (position.x >= -50.f && position.x <= -18.25f && position.z >= -18.75f && position.z <= 6.25f && position.y <= 6.f) {
        return true;
    }
    if (position.x >= -18.75f && position.x <= 6.25f && position.z >= -50.f && position.z <= -18.75f && position.y <= 6.f) {
        return true;
    }
    if (position.x >= 6.25f && position.x <= 31.75f && position.z >= 0.f && position.z <= 25.f && position.y <= 5.5f) {
        return true;
    }
    if (position.x >= -25.f && position.x <= 6.25f && position.z >= 0.0f && position.z <= 25.f && position.y <= 9.f) {
        return true;
    }
    if (position.x >= -50.f && position.x <= -25.f && position.z >= -6.25f && position.z <= 31.25f && position.y <= 2.2f) {
        return true;
    }
    if (position.x >= -18.75f && position.x <= 31.25f && position.z >= -18.75f && position.z <= 0.f && position.y <= 2.2f) {
        return true;
    }
    if (position.x >= -6.25f && position.x <= 31.25f && position.z >= -25.f && position.z <= -12.5f && position.y <= 2.2f) {
        return true;
    }
    if (position.x >= -6.25f && position.x <= 31.25f && position.z >= 12.5f && position.z <= 50.f && position.y <= 2.2f) {
        return true;
    }
    if (position.x >= 31.25f && position.x <= 50.f && position.z >= -6.25f && position.z <= 31.25f && position.y <= 2.2f) {
        return true;
    }
    return false;
}

void Game::terrainBlock(float x, float z) {
    if (x >= -25.f && x <= 37.5f && z >= -12.5f) {
        if (z <= 0.f && z >= -1.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.f, -0.5f), NULL);
        return;
    }
    if (x <= -25.f && z >= 0.f && z <= 25.f) {
        if (x <= -25.f && x >= -26.f) this->worms[this->currWorm]->changePosition(glm::vec3(-0.5f, 0.f, 0.f), NULL);
        return;
    }
    if (x >= -25.f && x <= 37.5f && z >= 25.f) {
        if (z >= 25.f && z <= 26.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.f, 0.5f), NULL);
        return;
    }
    if (x >= -12.5f && x <= 12.5f && z >= -25.f) {
        if (z >= -25.f && z <= -24.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.f, 0.5f), NULL);
        return;
    }
    if (x >= 12.5f && x <= 37.5f && z <= -25.f) {
        if (z <= -25.f && z >= -26.f) this->worms[this->currWorm]->changePosition(glm::vec3(0.f, 0.f, -0.5f), NULL);
        return;
    }
}