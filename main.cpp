#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

int main(int argc, char *argv[])
{
    SDL_Window *win = NULL;
    SDL_Renderer *renderer = NULL;
    int width = 800;
	int height = 600;
    bool loopShouldStop = false;

    SDL_Init(SDL_INIT_VIDEO);

    win = SDL_CreateWindow("Hello World", width, height, 0);

    renderer = SDL_CreateRenderer(win, NULL);

	SDL_FPoint a = {400.0f, 100.0f};
    SDL_FPoint b = {650.0f, 500.0f};
    SDL_FPoint c = {150.0f, 500.0f};

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

		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 

        SDL_RenderLine(renderer, a.x, a.y, b.x, b.y);
        SDL_RenderLine(renderer, b.x, b.y, c.x, c.y);
        SDL_RenderLine(renderer, c.x, c.y, a.x, a.y);


        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);

    SDL_Quit();

    return 0;
}
