// SortVisualizer - Aplicație GUI modulară pentru algoritmi de sortare
// Folosește SDL2 pentru vizualizare grafică

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define ARRAY_SIZE 100

int array[ARRAY_SIZE];
int current_index = 0;
bool sorting = false;

void generate_random_array() {
    for (int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = rand() % WINDOW_HEIGHT;
    }
}

void render_array(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    int bar_width = WINDOW_WIDTH / ARRAY_SIZE;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        SDL_Rect bar = {
            .x = i * bar_width,
            .y = WINDOW_HEIGHT - array[i],
            .w = bar_width - 1,
            .h = array[i]
        };
        SDL_SetRenderDrawColor(renderer, (i == current_index) ? 255 : 100, 255, 100, 255);
        SDL_RenderFillRect(renderer, &bar);
    }

    SDL_RenderPresent(renderer);
}

bool bubble_sort_step() {
    static int i = 0, j = 0;
    if (i < ARRAY_SIZE - 1) {
        if (j < ARRAY_SIZE - i - 1) {
            if (array[j] > array[j + 1]) {
                int temp = array[j];
                array[j] = array[j + 1];
                array[j + 1] = temp;
            }
            current_index = j;
            j++;
            return true;
        } else {
            j = 0;
            i++;
            return true;
        }
    }
    return false;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    generate_random_array();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("SortVisualizer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_SPACE) {
                    sorting = true;
                } else if (event.key.keysym.sym == SDLK_r) {
                    generate_random_array();
                    sorting = false;
                    current_index = 0;
                }
            }
        }

        if (sorting) {
            sorting = bubble_sort_step();
        }

        render_array(renderer);
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}