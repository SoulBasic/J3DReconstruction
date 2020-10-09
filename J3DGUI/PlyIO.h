#pragma once
#include <windows.h>
#include <math.h>		
#pragma once
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include "GLUtil/RoamingScenceManager.h"
#include "qdebug.h"

#define M_PI 3.14159265358979323846
#define TYPE_PLY_RB 0
#define TYPE_PLY_RT 1
#define TYPE_TXT 2

class PlyIO
{
public:
	PlyIO(char* fileName);
	~PlyIO();
	struct Point3D//点坐标
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
		GLfloat  r;
		GLfloat  g;
		GLfloat  b;
		GLfloat  alpha;
		GLfloat u;
		GLfloat v;
	};
	struct Face3D
	{
		int v1;
		int v2;
		int v3;
		GLfloat normal[3];
	};
	struct  Line3D//储存线(两个端点)
	{
		Point3D pointone;
		Point3D pointtwo;
	};
	struct  Triangle3D//储存三角形(三个端点)
	{
		Point3D pointone;
		Point3D pointtwo;
		Point3D pointthree;
		GLfloat normal[3]; //点的法向量
	};
	char* fileName;
	bool available;
	Point3D * vertex;
	Face3D * faces;
	std::vector<Line3D> Lines;
	std::vector<Triangle3D> Triangles;

	GLfloat x_max, x_min, y_max, y_min, z_max, z_min;//存储数据的左下、左上、右下、右上坐标

	int vertex_N, face_N;   //点数及面数

	std::string textureFileName;
	std::string workDir;
	bool open();
	void calculateNormal(Face3D &face);
	GLvoid render();
	GLuint CreateTextureFromPng(const char* filename);

private:
	std::vector<int> vertex_types, face_types;
	int type;
	FILE *file;
	int getPlyFileType();
	std::string vectostring(std::vector<char> vec);
	std::string readValueRB();
	int getTypeBytesLength(std::string type);
	bool readHeaderRB();


};

