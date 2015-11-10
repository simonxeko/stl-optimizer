#include "vector3.h"
#include <math.h>
Vector3::Vector3(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}
Vector3::Vector3(){
	x = y = z = 0;
}
float Vector3::size() const{
	return sqrt(x * x + y * y + z * z);
}

Vector3 operator-(const Vector3 m1, const Vector3 m2){
	return Vector3(m1.x-m2.x, m1.y-m2.y, m1.z-m2.z);
}

bool operator<(const Vector3 m1, const Vector3 m2){
	return (double)(m1.x) + (double)(m1.y)*10000.0 + (double)(m1.z)*10000000.0 < (double)(m2.x) + (double)(m2.y)*10000.0 + (double)(m2.z)*10000000.0;
}

bool operator>(const Vector3 m1, const Vector3 m2){
	return (double)(m1.x) + (double)(m1.y)*10000.0 + (double)(m1.z)*10000000.0 > (double)(m2.x) + (double)(m2.y)*10000.0 + (double)(m2.z)*10000000.0;
}

float operator*(const Vector3 m1, const Vector3 m2){
	return m1.x*m2.x+m1.y*m2.y+m1.z*m2.z;
}

bool operator==(const Vector3 m1, const Vector3 m2){
	return m1.x == m2.x && m1.y == m2.y && m1.z == m2.z;
}