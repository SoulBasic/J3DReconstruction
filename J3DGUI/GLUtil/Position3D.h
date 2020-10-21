#pragma once
#ifndef Position3D_H
#define Position3D_H

#include<iostream>
using namespace std;
class Vector3D;
class  Position3D
{
public:
	Position3D();
	Position3D(float xpos, float ypos, float zpos, float ws = 1);
	~Position3D();
	/***************************************************************/
	float X() const { return x; }
	float Y() const { return y; }
	float Z() const { return z; }
	float W()const { return w; }

	void setX(float xp) { x = xp; }
	void setY(float yp) { y = yp; }
	void setZ(float zp) { z = zp; }
	void setW(float wp) { w = wp; }

	void getfv(float a[]);
	/****************************************************************/
	inline Position3D& operator =(const Position3D& position);

	friend inline bool operator==(const Position3D&p1, const Position3D&p2);
	friend inline bool operator!=(const Position3D&p1, const Position3D&p2);
	friend  Position3D operator +(const Position3D&p, const Vector3D&v);
	friend   Position3D operator +(const Vector3D&v, const Position3D&p);
	friend   Position3D  operator -(const Position3D&p, Vector3D&v);


protected:
	float x;
	float y;
	float z;
	float w;
};

inline Position3D &Position3D::operator =(const Position3D &position)
{
	x = position.x;
	y = position.y;
	z = position.z;
	w = position.w;
	return *this;
}

inline bool operator ==(const Position3D &p1, const Position3D &p2)
{
	return p1.x == p2.x && p1.y == p2.y &&p1.z == p2.z;
}

inline bool operator !=(const Position3D &p1, const Position3D &p2)
{
	return p1.x != p2.x || p1.y != p2.y || p1.z != p2.z;
}

ostream& operator <<(ostream &os, const Position3D &position);

#endif // Position3D_H
