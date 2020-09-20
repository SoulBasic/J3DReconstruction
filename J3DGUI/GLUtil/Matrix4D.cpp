#include<stdio.h>
#include <math.h>

#include "Matrix4D.h"
#include"Vector3D.h"
#include"Position3D.h"

#define M_PI        3.14159265358979323846

Matrix4D::Matrix4D()
{
    setToIndentity();
}

Matrix4D::Matrix4D(double *values)
{
    for (int row = 0; row < 4; ++row)
        for (int col = 0; col < 4; ++col)
            m[col][row] = values[col * 4 + row];
    flagBits = General;
}

Matrix4D::Matrix4D(const Matrix4D &matrix)
{
     for(int i=0;i!=4;++i)
         for(int j=0;j!=4;++j)
             m[i][j]=matrix.m[i][j];
     flagBits=General;
}

Matrix4D::Matrix4D(const Vector3D &column1, const Vector3D &column2, const Vector3D &column3, const Position3D &pos)
{
    m[0][0]=column1.X();            m[0][1]=column2.X();            m[0][2]=column3.X();           m[0][3]=pos.X();
    m[1][0]=column1.Y();            m[1][1]=column2.Y();             m[1][2]=column3.Y();            m[1][3]=pos.Y();
    m[2][0]=column1.Z();            m[2][1]=column2.Z();            m[2][2]=column3.Z();            m[2][3]=pos.Z();
    m[3][0]=column1.W();           m[3][1]=column2.W();           m[3][2]=column3.W();           m[3][3]=pos.W();

}

 void Matrix4D::setToIndentity()
{
    m[0][0] = 1.0f;
    m[0][1] = 0.0f;
    m[0][2] = 0.0f;
    m[0][3] = 0.0f;
    m[1][0] = 0.0f;
    m[1][1] = 1.0f;
    m[1][2] = 0.0f;
    m[1][3] = 0.0f;
    m[2][0] = 0.0f;
    m[2][1] = 0.0f;
    m[2][2] = 1.0f;
    m[2][3] = 0.0f;
    m[3][0] = 0.0f;
    m[3][1] = 0.0f;
    m[3][2] = 0.0f;
    m[3][3] = 1.0f;
    flagBits = Identity;
}

Vector3D Matrix4D::column(int index)
{
    if(index>3||index<0)    {
        cout<<"Wrong Index!";
        return Vector3D(0,0,0);
    }  else
    {
         return Vector3D(m[0][index],m[1][index],m[2][index]);
    }
}

void Matrix4D::setColumn(int index, const Vector3D &value)
{
   if(index>3||index<0) {
       cout<<"Wrong Index!";
   }  else {
        m[0][index]=value.X();
        m[1][index]=value.Y();
        m[2][index]=value.Z();
        m[3][index]=value.W();
        flagBits=General;
          }

}

Vector3D Matrix4D::row(int index)
{
    if(index>3||index<0) {
        cout<<"Wrong Index!";
        return Vector3D(0,0,0);
    } else{
        return Vector3D(m[index][0],m[index][1],m[index][2]);
    }

}

void Matrix4D::setRow(int index, const Vector3D &value)
{
    if(index>3||index<0) {
        cout<<"Wrong Index!";
    } else{
    m[index][0]=value.X();
    m[index][1]=value.Y();
    m[index][2]=value.Z();
    m[index][3]=value.W();
    flagBits=General;
    }
}

double *Matrix4D::Datas()
{
    double *a=new double[16];
    a[0]=m[0][0];      a[4]=m[0][1];      a[8]=m[0][2];      a[12]=m[0][3];
    a[1]=m[1][0];      a[5]=m[1][1];       a[9]=m[1][2];       a[13]=m[1][3];
    a[2]=m[2][0];      a[6]=m[2][1];     a[10]=m[2][2];      a[14]=m[2][3];
    a[3]=m[3][0];      a[7]=m[3][1];     a[11]=m[3][2];      a[15]=m[3][3];
    return a;
}

 Matrix4D &Matrix4D::operator+=(const Matrix4D &other)
 {
     m[0][0] += other.m[0][0];
     m[0][1] += other.m[0][1];
     m[0][2] += other.m[0][2];
     m[0][3] += other.m[0][3];
     m[1][0] += other.m[1][0];
     m[1][1] += other.m[1][1];
     m[1][2] += other.m[1][2];
     m[1][3] += other.m[1][3];
     m[2][0] += other.m[2][0];
     m[2][1] += other.m[2][1];
     m[2][2] += other.m[2][2];
     m[2][3] += other.m[2][3];
     m[3][0] += other.m[3][0];
     m[3][1] += other.m[3][1];
     m[3][2] += other.m[3][2];
     m[3][3] += other.m[3][3];
     flagBits = General;
     return *this;
 }

 Matrix4D &Matrix4D::operator-=(const Matrix4D &other)
 {
     m[0][0] -= other.m[0][0];
     m[0][1] -= other.m[0][1];
     m[0][2] -= other.m[0][2];
     m[0][3] -= other.m[0][3];
     m[1][0] -= other.m[1][0];
     m[1][1] -= other.m[1][1];
     m[1][2] -= other.m[1][2];
     m[1][3] -= other.m[1][3];
     m[2][0] -= other.m[2][0];
     m[2][1] -= other.m[2][1];
     m[2][2] -= other.m[2][2];
     m[2][3] -= other.m[2][3];
     m[3][0] -= other.m[3][0];
     m[3][1] -= other.m[3][1];
     m[3][2] -= other.m[3][2];
     m[3][3] -= other.m[3][3];
     flagBits = General;
     return *this;
 }

 Matrix4D &Matrix4D::operator*=(const Matrix4D &other)
 {
     if (flagBits == Identity) {
         *this = other;
         return *this;
     } else if (other.flagBits == Identity) {
         return *this;
     } else {
         *this = *this * other;
         return *this;
     }
 }

 Matrix4D &Matrix4D::operator*=(float factor)
 {
     m[0][0] *= factor;
     m[0][1] *= factor;
     m[0][2] *= factor;
     m[0][3] *= factor;
     m[1][0] *= factor;
     m[1][1] *= factor;
     m[1][2] *= factor;
     m[1][3] *= factor;
     m[2][0] *= factor;
     m[2][1] *= factor;
     m[2][2] *= factor;
     m[2][3] *= factor;
     m[3][0] *= factor;
     m[3][1] *= factor;
     m[3][2] *= factor;
     m[3][3] *= factor;
     flagBits = General;
     return *this;
 }

 Matrix4D &Matrix4D::operator/=(float divisor)
 {
     m[0][0] /= divisor;
     m[0][1] /= divisor;
     m[0][2] /= divisor;
     m[0][3] /= divisor;
     m[1][0] /= divisor;
     m[1][1] /= divisor;
     m[1][2] /= divisor;
     m[1][3] /= divisor;
     m[2][0] /= divisor;
     m[2][1] /= divisor;
     m[2][2] /= divisor;
     m[2][3] /= divisor;
     m[3][0] /= divisor;
     m[3][1] /= divisor;
     m[3][2] /= divisor;
     m[3][3] /= divisor;
     flagBits = General;
     return *this;
 }

 bool Matrix4D::operator==(const Matrix4D &other) const
 {
     return m[0][0] == other.m[0][0] &&
            m[0][1] == other.m[0][1] &&
            m[0][2] == other.m[0][2] &&
            m[0][3] == other.m[0][3] &&
            m[1][0] == other.m[1][0] &&
            m[1][1] == other.m[1][1] &&
            m[1][2] == other.m[1][2] &&
            m[1][3] == other.m[1][3] &&
            m[2][0] == other.m[2][0] &&
            m[2][1] == other.m[2][1] &&
            m[2][2] == other.m[2][2] &&
            m[2][3] == other.m[2][3] &&
            m[3][0] == other.m[3][0] &&
            m[3][1] == other.m[3][1] &&
            m[3][2] == other.m[3][2] &&
            m[3][3] == other.m[3][3];
 }

 bool Matrix4D::operator!=(const Matrix4D &other) const
 {
     return m[0][0] != other.m[0][0] ||
            m[0][1] != other.m[0][1] ||
            m[0][2] != other.m[0][2] ||
            m[0][3] != other.m[0][3] ||
            m[1][0] != other.m[1][0] ||
            m[1][1] != other.m[1][1] ||
            m[1][2] != other.m[1][2] ||
            m[1][3] != other.m[1][3] ||
            m[2][0] != other.m[2][0] ||
            m[2][1] != other.m[2][1] ||
            m[2][2] != other.m[2][2] ||
            m[2][3] != other.m[2][3] ||
            m[3][0] != other.m[3][0] ||
            m[3][1] != other.m[3][1] ||
            m[3][2] != other.m[3][2] ||
             m[3][3] != other.m[3][3];
 }

 Vector3D operator*(const Matrix4D &matrix, const Vector3D &vector)
 {
     float x, y, z;
     x = vector.X() * matrix.m[0][0] +
         vector.Y() * matrix.m[0][1] +
         vector.Z() * matrix.m[0][2] ;

     y = vector.X() * matrix.m[1][0] +
         vector.Y() * matrix.m[1][1] +
         vector.Z() * matrix.m[1][2] ;

     z = vector.X() * matrix.m[2][0] +
         vector.Y() * matrix.m[2][1] +
         vector.Z() * matrix.m[2][2] ;

     return Vector3D(x, y, z);
 }

 Position3D operator*(const Matrix4D &matrix, const Position3D &position)
 {
     float x, y, z;
     x = position.X() * matrix.m[0][0] +
         position.Y() * matrix.m[0][1] +
         position.Z() * matrix.m[0][2]+
         position.W()*matrix.m[0][3];

     y = position.X() * matrix.m[1][0] +
         position.Y() * matrix.m[1][1] +
         position.Z() * matrix.m[1][2]+
         position.W()*matrix.m[1][3];

     z = position.X() * matrix.m[2][0] +
         position.Y() * matrix.m[2][1] +
         position.Z() * matrix.m[2][2]+
         position.W()*matrix.m[2][3];

     return Position3D(x, y, z);
 }

 ostream &operator <<(ostream &os,const Matrix4D &matrix)
 {
     char buffer[1024];
     sprintf(buffer,"Matrix4D[\n%f  %f  %f  %f\n%f  %f  %f  %f\n%f  %f  %f  %f\n%f  %f  %f  %f\n]"\
             ,matrix.m[0][0],matrix.m[0][1],matrix.m[0][2],matrix.m[0][3]
             ,matrix.m[1][0],matrix.m[1][1],matrix.m[1][2],matrix.m[1][3]
             ,matrix.m[2][0],matrix.m[2][1],matrix.m[2][2],matrix.m[2][3]
             ,matrix.m[3][0],matrix.m[3][1],matrix.m[3][2],matrix.m[3][3]);
     os<<buffer;
     return os;
 }

  Matrix4D Matrix4D::getTranslateMatrix(float x, float y, float z)
 {
     Matrix4D matrix;
     matrix.m[0][3]=x;
     matrix.m[1][3]=y;
     matrix.m[2][3]=z;
     matrix.flagBits=Matrix4D::Translation;
     return matrix;
  }

  Matrix4D Matrix4D::getTranslateMatrix(const Vector3D &vec)
  {
      Matrix4D matrix;
      matrix.m[0][3]=vec.X();
      matrix.m[1][3]=vec.Y();
      matrix.m[2][3]=vec.Z();
      matrix.flagBits=Matrix4D::Translation;
      return matrix;
  }

  Matrix4D Matrix4D::getRotateMatrix(float angle, float x, float y, float z)
  {
      angle=angle/2*M_PI/180;
      Vector3D vec(x,y,z);
      vec.normalize();
      float cosa=cos(angle);
      float sina=sin(angle);

      double a = vec.X() * sina;
      double b = vec.Y() * sina;
      double c = vec.Z() * sina;

      Matrix4D matrix;
      matrix.m[0][0]=1.0-2.0*(b*b+c*c);
      matrix.m[1][1] = 1.0 - 2.0*( c*c + a*a );
      matrix.m[2][2] = 1.0 - 2.0*( a*a + b*b );

      matrix.m[0][1] = 2.0 * ( a*b - c*cosa );
     matrix.m[0][2] = 2.0 * ( a*c + b*cosa );

      matrix.m[1][0] = 2.0 * ( a*b + c*cosa );
      matrix.m[1][2] = 2.0 * ( b*c - a*cosa );

      matrix.m[2][0] = 2.0 * ( a*c - b*cosa );
      matrix.m[2][1] = 2.0 * ( b*c + a*cosa );

      matrix.flagBits=Matrix4D::Rotation;
      return matrix;
  }

  Matrix4D Matrix4D::getRotateMatrix(float angle, const Vector3D &vector)
  {
           angle=angle/2*M_PI/180;
           Vector3D vec=vector.normalized();
           float cosa=cos(angle);
           float sina=sin(angle);

           double a = vec.X() * sina;
           double b = vec.Y() * sina;
           double c = vec.Z() * sina;

           Matrix4D matrix;
           matrix.m[0][0]=1.0-2.0*(b*b+c*c);
           matrix.m[1][1] = 1.0 - 2.0*( c*c + a*a );
           matrix.m[2][2] = 1.0 - 2.0*( a*a + b*b );

           matrix.m[0][1] = 2.0 * ( a*b - c*cosa );
          matrix.m[0][2] = 2.0 * ( a*c + b*cosa );

           matrix.m[1][0] = 2.0 * ( a*b + c*cosa );
           matrix.m[1][2] = 2.0 * ( b*c - a*cosa );

           matrix.m[2][0] = 2.0 * ( a*c - b*cosa );
           matrix.m[2][1] = 2.0 * ( b*c + a*cosa );

           matrix.flagBits=Matrix4D::Rotation;
           return matrix;
  }

Matrix4D operator+(const Matrix4D &m1, const Matrix4D &m2)
 {
    Matrix4D m;
    m.m[0][0] = m1.m[0][0] + m2.m[0][0];
    m.m[0][1] = m1.m[0][1] + m2.m[0][1];
    m.m[0][2] = m1.m[0][2] + m2.m[0][2];
    m.m[0][3] = m1.m[0][3] + m2.m[0][3];
    m.m[1][0] = m1.m[1][0] + m2.m[1][0];
    m.m[1][1] = m1.m[1][1] + m2.m[1][1];
    m.m[1][2] = m1.m[1][2] + m2.m[1][2];
    m.m[1][3] = m1.m[1][3] + m2.m[1][3];
    m.m[2][0] = m1.m[2][0] + m2.m[2][0];
    m.m[2][1] = m1.m[2][1] + m2.m[2][1];
    m.m[2][2] = m1.m[2][2] + m2.m[2][2];
    m.m[2][3] = m1.m[2][3] + m2.m[2][3];
    m.m[3][0] = m1.m[3][0] + m2.m[3][0];
    m.m[3][1] = m1.m[3][1] + m2.m[3][1];
    m.m[3][2] = m1.m[3][2] + m2.m[3][2];
    m.m[3][3] = m1.m[3][3] + m2.m[3][3];
    return m;
 }

Matrix4D operator-(const Matrix4D &m1, const Matrix4D &m2)
 {
    Matrix4D m;
    m.m[0][0] = m1.m[0][0] - m2.m[0][0];
    m.m[0][1] = m1.m[0][1] - m2.m[0][1];
    m.m[0][2] = m1.m[0][2] - m2.m[0][2];
    m.m[0][3] = m1.m[0][3] - m2.m[0][3];
    m.m[1][0] = m1.m[1][0]- m2.m[1][0];
    m.m[1][1] = m1.m[1][1] -m2.m[1][1];
    m.m[1][2] = m1.m[1][2] - m2.m[1][2];
    m.m[1][3] = m1.m[1][3] - m2.m[1][3];
    m.m[2][0] = m1.m[2][0] - m2.m[2][0];
    m.m[2][1] = m1.m[2][1] - m2.m[2][1];
    m.m[2][2] = m1.m[2][2] - m2.m[2][2];
    m.m[2][3] = m1.m[2][3] - m2.m[2][3];
    m.m[3][0] = m1.m[3][0] - m2.m[3][0];
    m.m[3][1] = m1.m[3][1] - m2.m[3][1];
    m.m[3][2] = m1.m[3][2] - m2.m[3][2];
    m.m[3][3] = m1.m[3][3] - m2.m[3][3];
    return m;

 }

Matrix4D operator*(const Matrix4D &m1, const Matrix4D &m2)
 {
    if (m1.flagBits == Matrix4D::Identity)
        return m2;
    else if (m2.flagBits == Matrix4D::Identity)
        return m1;

     Matrix4D m;
    m.m[0][0] = m1.m[0][0] * m2.m[0][0] +
                m1.m[0][1] * m2.m[1][0] +
                m1.m[0][2] * m2.m[2][0] +
                m1.m[0][3] * m2.m[3][0];

    m.m[0][1] = m1.m[0][0] * m2.m[0][1] +
                m1.m[0][1] * m2.m[1][1] +
                m1.m[0][2] * m2.m[2][1] +
                m1.m[0][3] * m2.m[3][1];

    m.m[0][2] = m1.m[0][0] * m2.m[0][2] +
                m1.m[0][1] * m2.m[1][2] +
                m1.m[0][2] * m2.m[2][2] +
                m1.m[0][3] * m2.m[3][2];

    m.m[0][3] = m1.m[0][0] * m2.m[0][3] +
                m1.m[0][1] * m2.m[1][3] +
                m1.m[0][2] * m2.m[2][3] +
                m1.m[0][3] * m2.m[3][3];
//
    m.m[1][0] = m1.m[1][0] * m2.m[0][0] +
                m1.m[1][1] * m2.m[1][0] +
                m1.m[1][2] * m2.m[2][0] +
                m1.m[1][3] * m2.m[3][0];

    m.m[1][1] = m1.m[1][0] * m2.m[0][1] +
                m1.m[1][1] * m2.m[1][1] +
                m1.m[1][2] * m2.m[2][1] +
                m1.m[1][3] * m2.m[3][1];

    m.m[1][2] = m1.m[1][0] * m2.m[0][2] +
                m1.m[1][1] * m2.m[1][2] +
                m1.m[1][2] * m2.m[2][2] +
                m1.m[1][3] * m2.m[3][2];

    m.m[1][3] = m1.m[1][0] * m2.m[0][3] +
                m1.m[1][1] * m2.m[1][3] +
                m1.m[1][2] * m2.m[2][3] +
                m1.m[1][3] * m2.m[3][3];
    //
    m.m[2][0] = m1.m[2][0] * m2.m[0][0] +
                m1.m[2][1] * m2.m[1][0] +
                m1.m[2][2] * m2.m[2][0] +
                m1.m[2][3] * m2.m[3][0];

    m.m[2][1] = m1.m[2][0] * m2.m[0][1] +
                m1.m[2][1] * m2.m[1][1] +
                m1.m[2][2] * m2.m[2][1] +
                m1.m[2][3] * m2.m[3][1];

    m.m[2][2] = m1.m[2][0] * m2.m[0][2] +
                m1.m[2][1] * m2.m[1][2] +
                m1.m[2][2] * m2.m[2][2] +
                m1.m[2][3] * m2.m[3][2];

    m.m[2][3] = m1.m[2][0] * m2.m[0][3] +
                m1.m[2][1] * m2.m[1][3] +
                m1.m[2][2] * m2.m[2][3] +
                m1.m[2][3] * m2.m[3][3];
    //
    m.m[3][0] = m1.m[3][0] * m2.m[0][0] +
                m1.m[3][1] * m2.m[1][0] +
                m1.m[3][2] * m2.m[2][0] +
                m1.m[3][3] * m2.m[3][0];

    m.m[3][1] = m1.m[3][0] * m2.m[0][1] +
                m1.m[3][1] * m2.m[1][1] +
                m1.m[3][2] * m2.m[2][1] +
                m1.m[3][3] * m2.m[3][1];

    m.m[3][2] = m1.m[3][0] * m2.m[0][2] +
                m1.m[3][1] * m2.m[1][2] +
                m1.m[3][2] * m2.m[2][2] +
                m1.m[3][3] * m2.m[3][2];

    m.m[3][3] = m1.m[3][0] * m2.m[0][3] +
                m1.m[3][1] * m2.m[1][3] +
                m1.m[3][2] * m2.m[2][3] +
                m1.m[3][3] * m2.m[3][3];
    return m;
 }

