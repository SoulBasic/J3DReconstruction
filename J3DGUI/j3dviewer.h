#ifndef J3DVIEWER_H
#define J3DVIEWER_H
#pragma once
#include "PlyIO.h"
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include "GLUtil/RoamingScenceManager.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include "Global.h"
//class RoamingScenceManager;
class j3dviewer : public QOpenGLWidget, public QOpenGLFunctions
{
    Q_OBJECT
public:
	j3dviewer(QWidget *parent = nullptr);
	PlyIO*ply;
	void setPly(PlyIO*ply);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
	void mousePressEvent(QMouseEvent*e);
	void mouseMoveEvent(QMouseEvent*e);
	void wheelEvent(QWheelEvent *e);

private:
	RoamingScenceManager*manager;
	
};


#endif // J3DVIEWER_H
