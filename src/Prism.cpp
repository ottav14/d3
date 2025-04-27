#include "Prism.h"

Prism::Prism(float* vertices, int vertex_count, unsigned int* indices, int index_count)
	: vertices_(vertices), vertex_count_(vertex_count), indices_(indices), index_count_(index_count) {}

float* Prism::getVertices() {
	return vertices_;
}

int Prism::getVertexCount() {
	return vertex_count_;
}

unsigned int* Prism::getIndices() {
	return indices_;
}

int Prism::getIndexCount() {
	return index_count_;
}

