all:vector3.cpp vertex.cpp face.cpp decimate.cpp
	g++ vector3.cpp vertex.cpp face.cpp decimate.cpp -o poly-opt
clean:
	rm -f poly-opt