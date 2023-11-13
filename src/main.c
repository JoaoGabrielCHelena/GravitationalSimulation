#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

#include "functions.h"
#include "definitions.h"
#include "body.h"

bool running = true;
bool pause = false;

Uint64 frames_elapsed = 0;

void update() {
    update_bodies();
}

void draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, GRID_COLOR);
    draw_background_grid(renderer);

    draw_bodies(renderer);
}

int main() {
    // Creating window and renderer
    SDL_Window* window;
    SDL_Renderer* renderer;

    initialize(&window, &renderer); 

    // Variables used within the game loop
    Uint64 frame_start=0, frame_end=0, current_frame_time=0;
    Uint64 frame_time_sum=0;
    Uint64 frequency = SDL_GetPerformanceFrequency();

    ///// Main Loop /////
    while (running) {
        frame_start = SDL_GetPerformanceCounter();

        handle_events();

        if (pause) { continue; }

        ///////////////////////////////////
        ///////////////////////////////////
        ///////////////////////////////////

        update();
        draw(renderer);

        // Updating window
        SDL_RenderPresent(renderer);

        // Clearing window
        SDL_SetRenderDrawColor(renderer, BACKGROUND_COLOR);
        SDL_RenderClear(renderer);

        ///////////////////////////////////
        ///////////////////////////////////
        ///////////////////////////////////

        frames_elapsed++;
        frame_end = SDL_GetPerformanceCounter();

        current_frame_time = frame_end - frame_start;
        current_frame_time += limit_fps(current_frame_time, frequency);

        frame_time_sum += current_frame_time;

        // Printing the average FPS over a period of PRINT_FPS_DELAY_FRAMES frames
        if (frames_elapsed % PRINT_FPS_DELAY_FRAMES == 0) {
            printf("%f\n", 1 / ((double) frame_time_sum / PRINT_FPS_DELAY_FRAMES / frequency));

            frame_time_sum = 0;
        }
    }

    // Quitting
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return EXIT_SUCCESS;
}