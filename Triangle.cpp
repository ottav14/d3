#include "Triangle.h"
#include <iostream>
#include <array>
#include <SDL3/SDL.h>

Triangle::Triangle(SDL_Vertex v1, SDL_Vertex v2, SDL_Vertex v3)
    : v1_(v1), v2_(v2), v3_(v3) {}

void Triangle::translate(float xoff, float yoff) {
	v1_.position.x += xoff;
	v1_.position.y += yoff;

	v2_.position.x += xoff;
	v2_.position.y += yoff;

	v3_.position.x += xoff;
	v3_.position.y += yoff;
}

void Triangle::render(SDL_Renderer *renderer) {
	SDL_Vertex vertices[3] = {v1_, v2_, v3_};
	SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);
}
