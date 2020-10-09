#pragma once
#include "Vector3D.h"
#include "Position3D.h"
#include "Matrix4D.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "GLUT.H"
#include "lpng/png.h"
#include "zlib/zlib.h"
#include <stdarg.h>
#include <math.h>
#ifndef ROAMINGSCENCEMANAGER_H
#define ROAMINGSCENCEMANAGER_H

class Vector3D;
class Position3D;
class RoamingScenceManager
{
public:
    RoamingScenceManager();
    ~RoamingScenceManager();
	GLuint read_png_file(const char* file_name);
	void initPng(const char* fileName);
    void init();
    void render();
    void executeRotateOperation(int x, int y);
    void executeScaleOperation(float factor);
    void executeTranslateOperation(int x,int y);
    void getInitPos(int x, int y);
private:
    //辅助坐标系三根轴
    Vector3D *AuxX;
    Vector3D *AuxY;
    Vector3D *AuxZ;

    //旋转后观察点方向与视线向上方向
    Vector3D*NewEye;
    Vector3D*NewUp;
    Vector3D *NewView;

    Position3D *OldMouse;
    Position3D *Mouse;

    Vector3D *TempTranslateVec;
    float TempscaleFactor;

    int ID_COORDINATY;
    void init_CoordinaryDisplayList();
};

#endif // ROAMINGSCENCEMANAGER_H
