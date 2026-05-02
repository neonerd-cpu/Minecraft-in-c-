#include </home/krish/Desktop/Minecraft-in-c-/include/game.hpp>
#include <SDL2/SDL_video.h>

game *g = nullptr;
int main() {
    g = new game();
    g->init("Minecraft",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800,600, false);
    while (g->running()){
        g->handleEvents();
        g->update();
        g->render();

    }
    g->clean();
    return 0;
}

