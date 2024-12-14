#pragma once
#include<SDL2/SDL.h>
#include<stdbool.h>

typedef struct SDL_app {
    bool running;
    int framerate;
    int screen_width;
    int screen_height;
    int cell_width;
    int cell_height;
    SDL_Window* window;
    SDL_Renderer* renderer;
} SDL_app;