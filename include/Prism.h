#ifndef PRISM_H
#define PRISM_H

class Prism {
public:
	Prism(float* vertices, int vertex_count, unsigned int* indices, int index_count);
	float* getVertices();
	int getVertexCount();
	unsigned int* getIndices();
	int getIndexCount();

private:
	float* vertices_;
	int vertex_count_;
	unsigned int* indices_;
	int index_count_;
};

#endif
