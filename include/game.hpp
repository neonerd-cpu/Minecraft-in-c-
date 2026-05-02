#ifndef game_hpp
#define game_hpp

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>
class game {
public:
  game();
  ~game();

  void init(const char *title, int xpos, int ypos, int width, int height,
            bool fullscreen);

  void handleEvents();
  void update();
  void render();
  void clean();

  bool running();

private:
    bool isRunning;
    SDL_Window *window;
    SDL_Renderer *renderer;
};
#endif