#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
#include<perlin.h>
#include<app.h>

SDL_app* app;

void on_exit() {
    SDL_DestroyWindow(app->window);
    SDL_Quit();
}

void handle_events() {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                printf("Received SDL_QUIT, informing the mainloop to stop.\n");
                app->running = false;
                break;
            default:
                break;
        }
    }
}

void render() {
    SDL_Renderer* renderer = app->renderer;
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);

    int row_count = app->screen_height / app->cell_height;
    int col_count = app->screen_width / app->cell_width;

    for (int row = 0; row < row_count; row++) {
        for (int col = 0; col < col_count; col++) {
            SDL_Rect rect = {
                .x = col * app->cell_width,
                .y = row * app->cell_height,
                .w = app->cell_width,
                .h = app->cell_height
            };

            double noise = perlin_noise2(
                (double)row * 0.1,
                (double)col * 0.1
            );

            SDL_SetRenderDrawColor(
                renderer,
                (int)(noise * 0xFF),
                (int)(noise * 0xFF),
                (int)(noise * 0xFF),
                0xFF
            );
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* args[]) {
    app = (SDL_app*) malloc(sizeof(SDL_app));
    app->screen_width = 800;
    app->screen_height = 500;
    app->cell_width = 5;
    app->cell_height = 5;
    app->framerate = 30;
    app->running = false;
    app->renderer = NULL;
    app->window = NULL;

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        printf("SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    printf("Initialized SDL.\n");

    SDL_Window* window = SDL_CreateWindow(
        "Perlin Noise",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        app->screen_width,
        app->screen_height,
        0
    );
    app->window = window;
    if (window == NULL) {
        printf("Failed to create window.\n");
        printf("SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    printf("Initialized window.\n");

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    app->renderer = renderer;
    if (renderer == NULL) {
        printf("Failed to create renderer.\n");
        printf("SDL Error: %s\n", SDL_GetError());
        return 1;
    }
    printf("Created renderer.\n");

    atexit(on_exit);
    app->running = true;

    printf("Entering mainloop.\n");
    while (app->running) {
        handle_events();
        render();
        SDL_Delay(1000 / app->framerate);
    }

    return 0;
}