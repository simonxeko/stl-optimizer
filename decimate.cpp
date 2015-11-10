#include <cstdio>
#include <cmath>
#include <list>
#include <vector>
#include <set>
#include <map>
#include "util.h"
#include "rbtree.h"

using namespace std;

// global vars
set<Face*> triangles;
set<Vertex*> vertices;
map<Vector3,Vertex*> vmap;

// keep vertex sorting in edge-cost with rbtree (good performance)
RBTree<Vertex*> vrbt;

// read binary stl
bool read_stl(char* fname){
	FILE* f = fopen(fname,"rb");

	char header_info[80] = "";
	unsigned long nTriLong;

	if (!f){
		fprintf(stderr, "File not loaded");
		return false;
	}

    // read 80 byte header
	fread(header_info, 1, 80, f);
    // read 4-byte ulong
	fread(&nTriLong, 4, 1, f);
	fprintf(stderr, "Faces: %ld\n", nTriLong);

    // read triangles
	for(int i = 0; i < nTriLong; i++){

		Vector3 vs[4];
		short abc;
		// read normal, vertex * 3
		fread(vs, 12, 4, f);
		// read attribute bytes
		fread(&abc, 2, 1, f);
		// vptr point to vertex pointers
		Vertex* vptr[3];

		for(int j = 1; j <= 3; j++){
			map<Vector3,Vertex*>::iterator it = vmap.find(vs[j]);
			Vertex* nv;
			if(it != vmap.end()){
				// found the vertex that is already exisited
				nv = it->second;
			}else{
				// create new vertex that is not exisited yet
				nv = new Vertex(vs[j]);
				vmap.insert(std::pair<Vector3,Vertex*>(vs[j], nv));
				vertices.insert(nv);
			}
			vptr[j-1] = nv;
		}
		triangles.insert(new Face(vptr[0],vptr[1],vptr[2],vs[0]));
	}

	fprintf(stderr, "Vertices: %ld\n", vertices.size());
	return true;
}

// global rbtree control
void insert_vertex(Vertex* u){
	vrbt.insert(u);
}

// global rbtree control
void remove_vertex(Vertex* u){
	vrbt.remove(u);
}

// sort by cost and position, since rbtree doesn't allow duplicate value in comparison
bool compare(const Vertex* a, const Vertex* b){
	if(a->cost < b->cost){
		return true;
	}else if(a->cost > b->cost){
		return false;
	}
	if(a->pos.x < b->pos.x){
		return true;
	}else if(a->pos.x > b->pos.x){
		return false;
	}
	if(a->pos.y > b->pos.y){
		return true;
	}else if(a->pos.y < b->pos.y){
		return false;
	}
	if(a->pos.z < b->pos.z){
		return true;
	}else if(a->pos.z > b->pos.z){
		return false;
	}
	return false;
}

int main(int argc, char* argv[]){
	int target_vertices = 300000;
	int resolution = 1;
	if( argc > 1 ){
		
		if(argc > 2){
			target_vertices = atoi(argv[2]);
			fprintf(stderr, "Target vertices %d\n", target_vertices);
		}

		//Load stl
		read_stl(argv[1]);

		//Reducing STL
		fprintf(stderr, "Building Edge Cost\n");
		for(set<Vertex*>::iterator it = vertices.begin(); it!=vertices.end(); it++){
			(*it)->compute_cost();
		}
		fprintf(stderr, "Reducing\n");
		int vertex_count = vertices.size();
		while(vertex_count-->target_vertices){
			Vertex* least_cost_vertex = vrbt.minimum();
			float least_cost = least_cost_vertex->cost;
			vrbt.remove(least_cost_vertex);
			least_cost_vertex->collapse_with(least_cost_vertex->collapse);
			if(vertex_count%1000 == 0){
				fprintf(stderr, "Vertices Count %ld, least_cost %f\n", vertex_count, least_cost);
			}
		}

		//Output selection
		set<Face*> output;
		for(set<Face*>::iterator it = triangles.begin(); it!=triangles.end(); it++){
			Vector3 a = (*it)->a->pos;
			Vector3 b = (*it)->b->pos;
			Vector3 c = (*it)->c->pos;
			if(a==b || b==c || c==a) continue;
			if((*it)->destroyed) continue;
			output.insert(*it);
		}

		fprintf(stderr,"Optimized faces: %ld, vertices: %ld\n", output.size(), vertex_count);

		//Output STL
		unsigned char header[80] = {0};
		unsigned short empty = 0;
		unsigned int len = output.size();
		fprintf(stderr, "Start output %u\n", len);
		FILE* fout = stdout;
		if(!fout){
			fprintf(stderr, "Unable to write file??\n");
		}
		fwrite(header, 1, 80, fout);
		fwrite(&len, 4, 1, fout);
		fprintf(stderr, "Iterating triangles..\n");
		for(set<Face*>::iterator it = output.begin(); it!=output.end(); it++){
			Face* tri = *it;
			Vector3 n = tri->normal;
			Vector3 a = tri->a->pos;
			Vector3 b = tri->b->pos;
			Vector3 c = tri->c->pos;
			fwrite(&n, 12, 1, fout);
			fwrite(&a, 12, 1, fout);
			fwrite(&b, 12, 1, fout);
			fwrite(&c, 12, 1, fout);
			fwrite(&empty, 2, 1, fout);
		}
		fprintf(stderr, "Output ended\n");
	}
	return 0;
}