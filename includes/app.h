#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<stdbool.h>
#include<color.h>

typedef struct SDL_app {
    bool running;
    int framerate;
    
    int screen_width;
    int screen_height;
    int cell_width;
    int cell_height;

    int mouse_x;
    int mouse_y;

    double amplitude;
    double frequency;
    int octave_count;
    double zoom;
    
    SDL_Window* window;
    SDL_Renderer* renderer;
    double render_start_x;
    double render_start_y;
    
    bool resized;
    int cf_index;
    bool render;

    SDL_Texture* img_controls;
} SDL_app;