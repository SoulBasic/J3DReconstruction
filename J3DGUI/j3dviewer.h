#ifndef J3DVIEWER_H
#define J3DVIEWER_H
#pragma once
#include "PlyIO.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <GLFW/glfw3.h>

#include <QMouseEvent>
#include <QWheelEvent>
#include "Global.h"
#include "GLUtil/Vector3D.h"
#include "GLUtil/Position3D.h"
#include "GLUtil/Matrix4D.h"
#include "lpng/png.h"
#include "zlib/zlib.h"
#include <stdarg.h>
#include <math.h>

//class RoamingScenceManager;
class j3dviewer : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
	j3dviewer(QWidget *parent = nullptr);
	~j3dviewer();
	PlyIO* ply;
	void setPly();


	void executeRotateOperation(int x, int y);
	void executeScaleOperation(float factor);
	void executeTranslateOperation(int x, int y);
	void getInitPos(int x, int y);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
	void mousePressEvent(QMouseEvent*e);
	void mouseMoveEvent(QMouseEvent*e);
	void wheelEvent(QWheelEvent *e);

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


#endif // J3DVIEWER_H
