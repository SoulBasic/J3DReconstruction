#ifndef MATRIX4D_H
#define MATRIX4D_H
#pragma once

/*
使用说明：
	  所有矩阵的操作都针对于列向量，也就是以列为主
	  Vx  Vy  Vz  P
	  x1  x2  x3 x4
	  y1  y2   y3 y4
	  z1  z2  z3  z4
	  0   0    0    1
*/
#include<iostream>

class Vector3D;
class Position3D;
using namespace std;
class  Matrix4D
{
public:
	Matrix4D();
	explicit Matrix4D(double *values);  //一行一行输入
	Matrix4D(const Matrix4D &matrix);
	Matrix4D(const Vector3D &column1, const Vector3D &column2, const Vector3D &column3, const Position3D &pos); //3个列向量，1个位置坐标

public:
	void setToIndentity();  //设置为单位矩阵

	Vector3D column(int index);
	void setColumn(int index, const Vector3D &value);

	Vector3D row(int index);
	void setRow(int index, const Vector3D &value);

	double *Datas(); //以列为主的数据

	Matrix4D& operator+=(const Matrix4D& other);
	Matrix4D& operator-=(const Matrix4D& other);
	Matrix4D& operator*=(const Matrix4D& other);
	Matrix4D& operator*=(float factor);
	Matrix4D& operator/=(float divisor);
	bool operator==(const Matrix4D& other) const;
	bool operator!=(const Matrix4D& other) const;

	friend Matrix4D operator+(const Matrix4D& m1, const Matrix4D& m2);
	friend Matrix4D operator-(const Matrix4D& m1, const Matrix4D& m2);
	friend Matrix4D operator*(const Matrix4D& m1, const Matrix4D& m2);

	friend Vector3D operator*(const Matrix4D& matrix, const Vector3D& vector);    //因为是以列为主,所以必须matrix*vector,下面也是.
	friend Position3D operator*(const Matrix4D& matrix, const Position3D& position);

	friend ostream& operator <<(ostream &os, const Matrix4D &matrix);//重载<<

	static  Matrix4D getTranslateMatrix(float x, float y, float z);//获取平移矩阵
	static  Matrix4D getTranslateMatrix(const Vector3D &vec);

	static  Matrix4D getRotateMatrix(float angle, float x, float y, float z);//获取旋转矩阵,angle是角度不是弧度
	static  Matrix4D getRotateMatrix(float angle, const Vector3D &vector);

private:
	double m[4][4];
	int flagBits;
	enum {
		Identity = 0x0001,   // 单位矩阵
		General = 0x0002,   // 一般矩阵
		Translation = 0x0004,   // 平移矩阵
		Scale = 0x0008,   //缩放矩阵
		Rotation = 0x0010    // 旋转矩阵
	};
};

#endif // MATRIX4D_H
