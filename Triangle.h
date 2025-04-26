#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <string>
#include <SDL3/SDL.h>

class Triangle {
public:
    Triangle(SDL_Vertex v1, SDL_Vertex v2, SDL_Vertex v3);
    void translate(float xoff, float yoff);
	void render(SDL_Renderer *renderer);
private:
	SDL_Vertex v1_;
	SDL_Vertex v2_;
	SDL_Vertex v3_;
};

#endif 

