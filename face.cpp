#include "vertex.h"
#include "face.h"

Face::Face(Vertex* a, Vertex* b, Vertex* c, Vector3 n){
	this->a = a;
	this->b = b;
	this->c = c;
	this->normal = n;
	//Maintain vertex association
	a->add_neighbor(b);
	a->add_neighbor(c);
	b->add_neighbor(a);
	b->add_neighbor(c);
	c->add_neighbor(a);
	c->add_neighbor(b);
	a->add_face(this);
	b->add_face(this);
	c->add_face(this);
	modified = 0;
	destroyed = false;
}

void Face::destroy(){
	a->remove_face(this);
	b->remove_face(this);
	c->remove_face(this);
	destroyed = true;
}

void Face::normalize(){
    Vector3 x = Vector3(b->pos.x - a->pos.x, b->pos.y - a->pos.y, b->pos.z - a->pos.z);
    Vector3 y = Vector3(c->pos.x - a->pos.x, c->pos.y - a->pos.y, c->pos.z - a->pos.z);
    normal = Vector3(x.y * y.z - x.z * y.y, x.z * y.x - x.x * y.z, x.x * y.y - x.y * y.x);
    float length = normal.size();
    normal = Vector3(normal.x/length, normal.y/length, normal.z/length);
}

bool Face::has(Vertex* v){
	return a==v || b==v || c==v;
}

void Face::replace(Vertex* u, Vertex* v){
	if(v->destroyed){
		fprintf(stderr, "destroyed but push back?\n");
	}
	if( a == u ){ 
		a = v;
		v->add_face(this);
		b->add_neighbor(v);
		c->add_neighbor(v);
		v->add_neighbor(b);
		v->add_neighbor(c);
		normalize();
		modified = 1;
		return;
	}
	if( b == u ){ 
		b = v;
		v->add_face(this);
		a->add_neighbor(v);
		c->add_neighbor(v);
		v->add_neighbor(a);
		v->add_neighbor(c);
		normalize();
		modified = 2;
		return;
	}
	if( c == u ){ 
		c = v;
		v->add_face(this);
		a->add_neighbor(v);
		b->add_neighbor(v);
		v->add_neighbor(a);
		v->add_neighbor(b);
		normalize();
		modified = 3;
		return;
	}
	fprintf(stderr, "Unable to replace vertex\n");
}