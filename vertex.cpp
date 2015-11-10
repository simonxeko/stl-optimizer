#include "util.h"
#include "vertex.h"
#include "face.h"

#include <float.h>

typedef list<Vertex*>::iterator VertexIter;
typedef list<Vertex*>::reverse_iterator RVertexIter;
typedef list<Face*>::iterator FaceIter;
typedef list<Face*>::reverse_iterator RFaceIter;

int vid = 0;

Vertex::Vertex(Vector3 pos){
	this->pos = pos;
	cost = DEFAULT_EDGE_COST;
	destroyed = false;
	collapse = NULL;
}
Vertex::Vertex(){
	cost = DEFAULT_EDGE_COST;
	destroyed = false;
	collapse = NULL;
}

void Vertex::add_neighbor(Vertex* u){
	if(find(neighbor.begin(),neighbor.end(),u) == neighbor.end()){
		neighbor.push_back(u);
	}
}
void Vertex::remove_neighbor(Vertex* u){
	list<Vertex*>::iterator vi = find(neighbor.begin(),neighbor.end(),u);
	if( vi != neighbor.end()){
		neighbor.erase(vi);
	}else{
		fprintf(stderr, "Unable to remove neighbor %d\n", u->destroyed);
	}
}
void Vertex::add_face(Face* u){
	if(find(face.begin(),face.end(),u) == face.end()){
		face.push_back(u);
	}
}
void Vertex::remove_face(Face* t){
	list<Face*>::iterator fi = find(face.begin(),face.end(),t);
	if(fi!=face.end()){
		face.erase(fi);
	}else{
		fprintf(stderr, "Unable to remove face %d %d\n", t->modified, t->destroyed);
	}
}
void Vertex::print(){
	fprintf(stderr, "V(%lf,%lf,%lf)C(%lf)D(%d)\n", this->pos.x, this->pos.y, this->pos.z, this->cost, destroyed);
}

void Vertex::Vertex::collapse_with(Vertex *v){
	if(!v) return;

	//make a face cache
	list<Face*> sides;
	for(FaceIter it=face.begin();it!=face.end();it++){
		sides.push_back(*it);
	}
	
	// delete triangles on edge uv, otherwise update
	for(FaceIter it=sides.begin();it!=sides.end();it++){
		if((*it)->has(v)){
			(*it)->destroy();
		}else{
			(*it)->replace(this,v);
			this->remove_face(*it);
		}
	}

	// remove the vertex itself from all neigbors
	for(list<Vertex*>::iterator it = neighbor.begin(); it!=neighbor.end();it++){
		(*it)->remove_neighbor(this);
	}

	// recompute the edge collapse costs in neighborhood
	for(VertexIter it=neighbor.begin();it!=neighbor.end();it++) {
		(*it)->compute_cost();
	}

	//mark as delete
	this->destroyed = true;
}


float Vertex::compute_cost(Vertex *v) {
	// if we collapse edge uv by moving u to v then how
	// much different will the model change, i.e. the “error”.
	float edgelength = (v->pos - pos).size();
	float curvature=0;

	// use the Face facing most away from the sides
	// to determine our curvature term
	for(FaceIter it=face.begin();it!=face.end();it++) {
		float mincurv=1;
		for(FaceIter it2=face.begin();it2!=face.end();it2++) {
			// use dot product of face normals.
			if((*it)->has(v)){
				float dotprod = (*it)->normal*(*it2)->normal;
				mincurv = min(mincurv,(1-dotprod)/2.0f);
			}
		}
		curvature = max(curvature,mincurv);
	}
	return edgelength * curvature;
}
void Vertex::compute_cost(){
	// reinsert to global tree if calculated
	if(cost!=DEFAULT_EDGE_COST) remove_vertex(this);
	// skip recalculate if inserted
	if(this->destroyed) return;
	// if it's a singular vertex
	if(neighbor.size()==0) {
		collapse=NULL;
		cost=-0.01f;
		insert_vertex(this);
		return;
	}
	// recalculate edge cost
	cost = DEFAULT_EDGE_COST;
	collapse = NULL;
	// search all neighboring edges for “least cost” edge
	for(VertexIter it = neighbor.begin(); it!=neighbor.end(); it++) {
		if((*it)->destroyed) continue;
		float c = compute_cost(*it);
		if(c < cost) {
			collapse=*it;
			cost=c;
		}
	}
	// insert to global tree again
	insert_vertex(this);
}

