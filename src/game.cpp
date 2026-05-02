#include "/home/krish/Desktop/Minecraft-in-c-/include/game.hpp"
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

game::game() {}
game::~game() {}
void game::init(const char *title, int xpos, int ypos, int width, int height,
                bool fullscreen) {
  int flags = 0;
  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }
  if (SDL_Init(SDL_INIT_VIDEO) == 0) {
    std::cout << "Subsystem initialised..." << std::endl;
    window = SDL_CreateWindow("Minecraft", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 800, 600, flags);
    if (window) {
      std::cout << "Window Created" << std::endl;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
        SDL_SetRenderDrawColor(renderer, 255,255,255,255);
      std::cout << "Renderer created" << std::endl;
    }
    isRunning = true;
  
  }else{
    isRunning= false;
  }
}
void game::handleEvents() {
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT:
            isRunning = false;
            break;
        default:
            break;
    
    }
}
void game::render() {
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}
void game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    std::cout<<"Game Closed"<<std::endl;
}
void game::update() {}
bool game::running() { return isRunning; }