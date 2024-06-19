#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <stdio.h>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int running = 1;
int time_since_start = 0;
int blue_intensity = 0;

void main_loop() {
	time_since_start++;
    SDL_Event event;
	const Uint8* keyboard_state = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = 0;
            #ifdef __EMSCRIPTEN__
            emscripten_cancel_main_loop();
            #endif
        }
    }

	if (keyboard_state[SDL_SCANCODE_W]) {
		blue_intensity++;
	}

    SDL_SetRenderDrawColor(renderer, time_since_start % 255, 0, blue_intensity % 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Hello SDL", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    #ifdef __EMSCRIPTEN__
    emscripten_set_main_loop(main_loop, 0, 1);
    #else
    while (running) {
        main_loop();
        SDL_Delay(16); // Simulate ~60 FPS
    }
    #endif

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
