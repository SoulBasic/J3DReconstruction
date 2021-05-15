#ifndef IMAGE_VIEW_H
#define IMAGE_VIEW_H
#include <QWidget>
#include <QImage>
#include <qmessagebox.h>
#include <QMenu>
#include <QContextMenuEvent>
#include <QStyleOption>
#include <QPainter>
#include <QFileDialog>
#include "dialog_coorintersector.h"

class ImageView : public QWidget
{
	Q_OBJECT
public:
	ImageView(QWidget* parent);
	~ImageView();
	void loadImage(QString fileName, int index);
protected:
	void contextMenuEvent(QContextMenuEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;

private:
	QImage m_Image;
	qreal m_ZoomValue = 1.0;
	int m_XPtInterval = 0;
	int m_YPtInterval = 0;
	QPoint m_OldPos;
	QPoint now_pos;
	QPoint now_image_pos;
	bool m_Pressed = false;
	Dialog_CoorIntersector* _parent;
	SEACAVE::Point3d intersectResult;
	int image_index;
	QMenu* right_button_menu;
private slots:
	void onZoomInImage(void);
	void onZoomOutImage(void);
	void intersectPoint();

};
#endif

