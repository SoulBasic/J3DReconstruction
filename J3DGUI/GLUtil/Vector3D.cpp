#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#include "Vector3D.h"

Vector3D::Vector3D()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vector3D::Vector3D(float xpos, float ypos, float zpos, float ws) :Position3D(xpos, ypos, zpos)
{
	w = 0;
}

Vector3D::Vector3D(Position3D &startPoint, Position3D &endPoint)
{
	x = endPoint.X() - startPoint.X();
	y = endPoint.Y() - startPoint.Y();
	z = endPoint.Z() - startPoint.Z();
	w = 0;
}

Vector3D::~Vector3D()
{

}

float Vector3D::dotProduct(const Vector3D &v1, const Vector3D &v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3D Vector3D::crossProduct(const Vector3D &v1, const Vector3D &v2)
{
	Vector3D v(v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x);
	return v;
}

Vector3D Vector3D::normalized() const
{
	double len = double(x) * double(x) +
		double(y) * double(y) +
		double(z) * double(z);

	if (abs(len - 1.0) <= 0.00001f) return *this;
	else if (abs(len) > 0.00001f)  return *this / sqrt(len);
	else return Vector3D();
}

void Vector3D::normalize()
{
	double len = double(x) * double(x) +
		double(y) * double(y) +
		double(z) * double(z);

	if (abs(len - 1.0) <= 0.00001f || abs(len) <= 0.00001f) return;
	len = sqrt(len);

	x /= len;
	y /= len;
	z /= len;

}

float Vector3D::length() const
{
	return sqrt(x * x + y * y + z * z);
}

ostream &operator <<(ostream &os, const Vector3D &vector)
{
	char buffer[100];
	sprintf(buffer, "Vector3D(%f, %f, %f)", vector.X(), vector.Y(), vector.Z());
	os << buffer;
	return os;
}

