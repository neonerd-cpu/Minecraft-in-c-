#include <SDL2/SDL.h>
#include <iostream>

int main() {
    SDL_Init(SDL_INIT_VIDEO
    SDL_Window* window = SDL_CreateWindow(
        "Minecraft",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );


    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

