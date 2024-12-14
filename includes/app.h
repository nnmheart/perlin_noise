#pragma once
#include<SDL2/SDL.h>
#include<stdbool.h>
#include<color.h>

typedef struct SDL_app {
    bool running;
    int framerate;
    
    int screen_width;
    int screen_height;
    int cell_width;
    int cell_height;

    double amplitude;
    double frequency;
    int octave_count;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    double render_start_x;
    double render_start_y;
    
    bool resized;
    int cf_index;
    bool render;
} SDL_app;