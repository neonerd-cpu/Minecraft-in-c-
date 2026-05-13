#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <epoxy/gl.h>
#include <iostream>

#include "world.h"
#include "player.h"
#include "shader.h"
#include "selection_box.h"
#include "inventory.h"
#include "ui.h"

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void stop();

private:
    void handleEvents();
    void update(float deltaTime);
    void render();

    SDL_Window* window;
    SDL_GLContext glContext;
    bool running;
    int width, height;

    World* world;
    Player* player;
    Shader* shader;
    SelectionBox* selectionBox;
    Inventory* inventory;
    UI* ui;

    // Block interaction
    float breakingTime;
    glm::ivec3 targetedBlock;
    bool hasTarget;
    bool showInventory;

    void updateRaycast();
};

#endif
