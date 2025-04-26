#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "Triangle.h"

int main(int argc, char *argv[])
{
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    int width = 800;
	int height = 600;
    bool loopShouldStop = false;
	Uint64 start_ticks = SDL_GetTicks();

    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Hello World", width, height, 0);

    renderer = SDL_CreateRenderer(win, NULL);

    SDL_Vertex vertices[3] = {
        { {400, 150}, {1.0f, 0.0f, 0.0f, 1.0f}, {0, 0} }, 
        { {200, 450}, {0.0f, 0.0f, 1.0f, 1.0f}, {0, 0} }, 
        { {600, 450}, {0.0f, 1.0f, 0.0f, 1.0f}, {0, 0} } 
    };
	 
	Triangle triangle(vertices[0], vertices[1], vertices[2]);

    while (!loopShouldStop)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_EVENT_QUIT:
                    loopShouldStop = true;
                    break;
            }
        }

		SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255); 
        SDL_RenderClear(renderer);

		triangle.render(renderer);

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 


        SDL_RenderPresent(renderer);

		triangle.translate(0.01, 0);


    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
