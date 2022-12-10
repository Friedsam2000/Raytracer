#define GRID_WIDTH 700
#define GRID_HEIGTH 450
#define FPS 60
#define GRID_CELL_SIZE 2
#define INITIAL_CAMERA_X 100
#define INITIAL_CAMERA_Y 100

//more settings (definitions) in src/Grid.cpp


#include "../Dependencies/SDL2/include/SDL.h"
#include "../Dependencies/SDL2/include/SDL_ttf.h"
#include "Grid.h"

static Uint32 next_time;

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}



int main()
{
    //Create a grid
    Grid grid(GRID_WIDTH, GRID_HEIGTH);

    // calculate window size from grid size
    int window_width = (grid.width * GRID_CELL_SIZE);
    int window_height = (grid.height * GRID_CELL_SIZE);

    //set up initial grid
    grid.setCamera(Coordinate(INITIAL_CAMERA_X, INITIAL_CAMERA_Y));

    //draw a circle with radius 30 in the middle of the grid
    for (int i = 0; i < grid.height; i++){
        for (int j = 0; j < grid.width; j++){
            if (sqrt(pow(i - grid.height/2, 2) + pow(j - grid.width/2, 2)) < 80){
                grid.setObject(Coordinate(j, i));
            }
        }
    }

    // create sdl window
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_CreateWindowAndRenderer(window_width, window_height, 0, &window,&renderer);
    SDL_SetWindowTitle(window, "SDL Grid");
    TTF_Init();

    // SDL Event loop.
    SDL_bool quit = SDL_FALSE;
    next_time = SDL_GetTicks() + 1000 / FPS;
    while (!quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            //check if the window is closed
            if(event.type == SDL_QUIT) {
                    quit = SDL_TRUE;
            }
            //check if a key is pressed
            if(event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                //process the key press
                grid.handleSDLEvent(renderer, GRID_CELL_SIZE, event);
            }
        }

        //draw the grid in the SDL window
        grid.drawPixelsInSDL(renderer, GRID_CELL_SIZE);
        //draw the screens output in the SDL window
        grid.drawScreensInSDL(renderer, GRID_CELL_SIZE);
        //overlay text
        grid.overlayTextInSDL(renderer, GRID_CELL_SIZE);
        SDL_RenderPresent(renderer);
        //if the next time is reached, update the grid
        SDL_Delay(time_left());
        next_time += 1000 / FPS;
    }

    //Clean up.
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    return EXIT_SUCCESS;
}
