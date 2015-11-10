#pragma once
#include <math.h>
class Vector3{	
public:
	float x,y,z;
	Vector3(float x, float y, float z);
	Vector3();
	float size() const;
};

Vector3 operator-(const Vector3 m1, const Vector3 m2);
bool operator<(const Vector3 m1, const Vector3 m2);
bool operator>(const Vector3 m1, const Vector3 m2);
float operator*(const Vector3 m1, const Vector3 m2);
bool operator==(const Vector3 m1, const Vector3 m2);