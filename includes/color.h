#pragma once
#include<SDL2/SDL.h>

typedef void (*ColorFunction)(SDL_Renderer* renderer, double noise);

void CF_uncoherent_monochrome(SDL_Renderer* renderer, double noise) {
    SDL_SetRenderDrawColor(
        renderer,
        (int)(noise * 0xFF),
        (int)(noise * 0xFF),
        (int)(noise * 0xFF),
        0xFF
    );
}
void CF_coherent_monochrome(SDL_Renderer* renderer, double noise) {
    noise = (noise + 1) / 2;
    SDL_SetRenderDrawColor(
        renderer,
        (int)(noise * 0xFF),
        (int)(noise * 0xFF),
        (int)(noise * 0xFF),
        0xFF
    );
}

void CF_height_color(SDL_Renderer* renderer, double noise) {
    noise = (noise + 1) / 2;
    noise = noise * 3000;
    uint8_t r = 0x00, g = 0x00, b = 0x00;

    if ((0 <= noise) && (noise < 200)) {
        r = 81;
        g = 202;
        b = 39;
    } else if ((200 <= noise) && (noise < 500)) {
        r = 161;
        g = 241;
        b = 130;
    } else if ((500 <= noise) && (noise < 1000)) {
        r = 238;
        g = 243;
        b = 101;
    } else if ((1000 <= noise) && (noise < 1500)) {
        r = 240;
        g = 172;
        b = 25;
    } else if ((1500 <= noise) && (noise < 2000)) {
        r = 203;
        g = 180;
        b = 130;
    } else if ((noise <= 2000) && (noise < 2500)) {
        r = 151;
        g = 121;
        b = 57;
    } else {
        r = 255;
        g = 255;
        b = 255;
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
}

void CF_only_red(SDL_Renderer* renderer, double noise) {
    noise = (noise + 1) / 2;
    SDL_SetRenderDrawColor(renderer, (uint8_t)(0xFF * noise), 0x00, 0x00, 0xFF);
}
void CF_only_green(SDL_Renderer* renderer, double noise) {
    noise = (noise + 1) / 2;
    SDL_SetRenderDrawColor(renderer, 0x00, (uint8_t)(0xFF * noise), 0x00, 0xFF);
}
void CF_only_blue(SDL_Renderer* renderer, double noise) {
    noise = (noise + 1) / 2;
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, (uint8_t)(0xFF * noise), 0xFF);
}

ColorFunction color_functions[] = {
    CF_coherent_monochrome,
    CF_uncoherent_monochrome,
    CF_height_color,
    CF_only_red,
    CF_only_green,
    CF_only_blue,
};
const int color_functions_count = sizeof(color_functions) / sizeof(ColorFunction);