#include "game.h"

int main(int, char**) {
    Game game;
    if (!game.init("Minecraft in C++", 1280, 720)) {
        return -1;
    }
    game.run();
    return 0;
}
