#pragma once
#include "vector3.h"
#include "vertex.h"
class Vertex;
class Face{
public:
	Vertex* a;
	Vertex* b;
	Vertex* c;
	Vector3 normal;
	int modified;
	bool destroyed;
	Face(Vertex* a, Vertex* b, Vertex* c, Vector3 n);
	void destroy();
	void normalize();
	bool has(Vertex* v);
	void replace(Vertex* u, Vertex* v);
};