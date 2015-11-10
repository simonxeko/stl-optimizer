#pragma once
#include "vector3.h"
#include "face.h"
#include <list>
#include <float.h>

#define DEFAULT_EDGE_COST FLT_MAX
using namespace std;
class Vertex{
public:
	Vertex* collapse;
	Vector3 pos;
	list<Face *> face;
	list<Vertex *> neighbor;
	float cost;
	bool destroyed;
	Vertex(Vector3 pos);
	Vertex();
	void add_neighbor(Vertex* u);
	void remove_neighbor(Vertex* u);
	void add_face(Face* t);
	void remove_face(Face* t);
	void collapse_with(Vertex *v);
	void compute_cost();
	float compute_cost(Vertex *v);
	
	void print();
};