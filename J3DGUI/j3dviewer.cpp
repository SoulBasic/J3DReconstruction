#include "j3dviewer.h"



j3dviewer::j3dviewer(QWidget *parent)
	: QOpenGLWidget{ parent }
{
	this->setFocusPolicy(Qt::StrongFocus);
	manager = new RoamingScenceManager();
	ply = new PlyIO("");
	//ply->open();
}

void j3dviewer::initializeGL()
{
	manager->init();
}

void j3dviewer::paintGL()
{
	manager->render();
	if (this->ply->available == true) {
		this->ply->render();
	}

}

void j3dviewer::resizeGL(int w, int h)
{
	//glViewport(0, -(w - h) / 2, w, w);
}

void j3dviewer::mousePressEvent(QMouseEvent *e)
{
	manager->getInitPos(e->x(), e->y());
}

void j3dviewer::mouseMoveEvent(QMouseEvent *e)
{
	if (e->buttons()&Qt::LeftButton)
	{
		manager->executeRotateOperation(e->x(), e->y());
	}
	else if (e->buttons()&Qt::RightButton) 
	{
		manager->executeTranslateOperation(e->x(), e->y());
	}

	update();
}

void j3dviewer::wheelEvent(QWheelEvent *e)
{
	if (e->delta() >= 0)
	{
		manager->executeScaleOperation(1.5);
	}
	else
	{
		manager->executeScaleOperation(-1.5);
	}
	update();
}


void j3dviewer::setPly(PlyIO*ply) {
	this->ply = ply;
}