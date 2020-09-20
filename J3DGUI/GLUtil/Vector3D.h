#pragma once
#ifndef Vector3D_H
#define Vector3D_H
#include"Position3D.h"
class   Vector3D:public Position3D
{
public:
    Vector3D();
    Vector3D(float xpos,float ypos,float zpos,float ws=0);
    Vector3D(Position3D &startPoint,Position3D&endPoint);
    ~Vector3D();
    /****************************************************/
    inline Vector3D& operator =(const Vector3D& vector);

    friend inline bool operator==(const Vector3D&v1,const Vector3D&v2);
    friend inline bool operator!=(const Vector3D&v1,const Vector3D&v2);
    friend inline  const Vector3D operator+(const Vector3D &v1, const Vector3D &v2);
    friend inline  const Vector3D operator-(const Vector3D &v1, const Vector3D &v2);
    friend inline  const Vector3D operator*(float factor, const Vector3D &vector);
    friend inline  const Vector3D operator*(const Vector3D &vector, float factor);
    friend inline  const Vector3D operator/(const Vector3D &vector, float divisor);
   /******************************************************/
    static float dotProduct(const Vector3D& v1, const Vector3D& v2);
    static Vector3D crossProduct(const Vector3D& v1, const Vector3D& v2);

    Vector3D normalized() const;
     void normalize();
     float length() const;

};

inline Vector3D &Vector3D::operator =(const Vector3D &vector)
{
    x=vector.x;
    y=vector.y;
    z=vector.z;
    w=vector.w;
    return  *this;
}

inline  const Vector3D operator+(const Vector3D &v1, const Vector3D &v2)
{
     return   Vector3D (v1.x+v2.x,v1.y+v2.y,v1.z+v2.z);
}

inline const  Vector3D operator-(const Vector3D &v1, const Vector3D &v2)
{
    return Vector3D (v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);
}

inline const  Vector3D operator*(float factor, const Vector3D &vector)
{
    return  Vector3D (vector.x*factor,vector.y*factor,vector.z*factor);
}

inline  const Vector3D operator*(const Vector3D &vector, float factor)
{
    return   Vector3D (vector.x*factor,vector.y*factor,vector.z*factor);
}

inline  const Vector3D operator/(const Vector3D &vector, float divisor)
{
    return  Vector3D (vector.x/divisor,vector.y/divisor,vector.z/divisor);
}

inline bool operator ==(const Vector3D &v1, const Vector3D &v2)
{
     return v1.x == v2.x &&v1.y == v2.y &&v1.z == v2.z;
}

inline bool operator !=(const Vector3D &v1, const Vector3D &v2)
{
     return v1.x != v2.x || v1.y != v2.y ||v1.z != v2.z;
}


 ostream& operator <<(ostream &os,const Vector3D &vector);
#endif // Vector3D_H
