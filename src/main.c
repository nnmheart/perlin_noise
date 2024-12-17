#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<perlin.h>
#include<app.h>
#include<color.h>

SDL_app* app;

void on_exit() {
    SDL_DestroyWindow(app->window);
    SDL_DestroyTexture(app->img_controls);
    SDL_Quit();
}

void handle_keydown(SDL_Event e) {
    SDL_Keycode key = e.key.keysym.sym;

    switch (key) {
        // Grid movements
        case SDLK_UP:
        case SDLK_w:
            app->render_start_y -= 0.1;
            app->render = true;
            break;
        case SDLK_DOWN:
        case SDLK_s:
            app->render_start_y += 0.1;
            app->render = true;
            break;
        case SDLK_LEFT:
        case SDLK_a:
            app->render_start_x -= 0.1;
            app->render = true;
            break;
        case SDLK_RIGHT:
        case SDLK_d:
            app->render_start_x += 0.1;
            app->render = true;
            break;
        case SDLK_z:
            app->cf_index = (app->cf_index + 1) % color_functions_count;
            app->render = true;
            break;
        
        // Frequency control
        case SDLK_KP_1:
            app->frequency -= 0.5;
            app->render = true;
            break;
        case SDLK_KP_2:
            app->frequency += 0.5;
            app->render = true;
            break;
        
        // Amplitude control
        case SDLK_KP_4:
            app->amplitude -= 0.5;
            app->render = true;
            break;
        case SDLK_KP_5:
            app->amplitude += 0.5;
            app->render = true;
            break;
        
        // Octave count control
        case SDLK_KP_7:
            app->octave_count -= 1;
            app->render = true;
            break;
        case SDLK_KP_8:
            app->octave_count += 1;
            app->render = true;
            break;

        default:
            break;
    }
}

void handle_events() {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        switch (e.type) {
            case SDL_QUIT:
                printf("Received SDL_QUIT, informing the mainloop to stop.\n");
                app->running = false;
                break;
            case SDL_KEYDOWN:
                handle_keydown(e);
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    app->resized = true;
                    app->render = true;
                }
                break;
            default:
                break;
        }
    }
}


void initialize_textures() {
    app->img_controls = IMG_LoadTexture(app->renderer, "controls.png");
    if (app->img_controls == NULL) {
        printf("Unable to load controls image.\n");
        printf("SDL Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

void tick() {
    if (app->resized) {
        SDL_GetWindowSize(app->window, &app->screen_width, &app->screen_height);
        SDL_DestroyTexture(app->img_controls);
        SDL_DestroyRenderer(app->renderer);
        app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);
        app->resized = false;
        initialize_textures();
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

            double total_noise = 0.0;
            double cur_freq = app->frequency;
            double cur_amp = app->amplitude;

            for (int i = 0; i < app->octave_count; i++) {
                double noise = perlin_noise2(
                    (app->render_start_x + (double)col * 0.1) * cur_freq,
                    (app->render_start_y + (double)row * 0.1) * cur_freq
                );
                total_noise += (cur_amp * noise);
                cur_amp *= 0.5;
                cur_freq *= 2.0;
            }

            color_functions[app->cf_index](renderer, total_noise);
            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void render_controls() {
    int w = 160;
    int h = 180;
    SDL_Rect area = {
        .x = app->screen_width - w,
        .y = app->screen_height - h,
        .w = w,
        .h = h
    };
    if (SDL_RenderCopy(app->renderer, app->img_controls, NULL, &area) != 0) {
        printf("Unable to render controls image.\n");
        printf("SDL Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char* args[]) {
    app = (SDL_app*) malloc(sizeof(SDL_app));
    app->running = false;
    app->framerate = 30;

    app->screen_width = 800;
    app->screen_height = 500;
    app->cell_width = 5;
    app->cell_height = 5;

    app->amplitude = 1.0;
    app->frequency = 1.0;
    app->octave_count = 1;

    app->render_start_x = 0.0;
    app->renderer = NULL;
    app->window = NULL;
    app->render_start_y = 0.0;

    app->resized = false;
    app->cf_index = 0;
    app->render = true;


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
        SDL_WINDOW_RESIZABLE
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
    initialize_textures();

    atexit(on_exit);
    app->running = true;

    printf("Entering mainloop.\n");
    while (app->running) {
        handle_events();
        tick();
        if (app->render) {
            app->render = false;
            render();
            render_controls();
            SDL_RenderPresent(app->renderer);
        }
        SDL_Delay(1000 / app->framerate);
    }

    return 0;
}