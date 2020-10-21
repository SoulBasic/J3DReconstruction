#include<stdio.h>

#include "Position3D.h"
#include"Vector3D.h"

Position3D::Position3D() :x(0), y(0), z(0), w(1)
{

}

Position3D::Position3D(float xpos, float ypos, float zpos, float ws) : x(xpos), y(ypos), z(zpos), w(ws)
{

}

Position3D::~Position3D()
{

}

void Position3D::getfv(float a[])
{
	a[0] = x;
	a[1] = y;
	a[2] = z;
	a[3] = w;
}

Position3D operator+(const Position3D&p, const Vector3D&v)
{
	return Position3D(p.X() + v.X(), p.Y() + v.Y(), p.Z() + v.Z());
}
Position3D operator+(const Vector3D&v, const Position3D&p)
{
	return Position3D(p.x + v.X(), p.y + v.Y(), p.z + v.Z());
}

Position3D  operator-(const Position3D&p, Vector3D&v)
{
	return Position3D(p.x - v.X(), p.y - v.Y(), p.z - v.Z());
}

ostream &operator <<(ostream &os, const Position3D &position)
{
	char buffer[100];
	sprintf(buffer, "Position3D(%f, %f, %f, %f)", position.X(), position.Y(), position.Z(), position.W());
	os << buffer;
	return os;
}
