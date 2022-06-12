#include "worms.h"

int main(){
    Game worms(1920, 1080);
    glfwSetTime(0);
    srand(time(NULL));
    while (!worms.getShouldClose()){
        
        worms.drawScene();
        
    }

    return 0;
}