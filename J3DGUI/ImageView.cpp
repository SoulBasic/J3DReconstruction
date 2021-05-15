#include "ImageView.h"


ImageView::ImageView(QWidget *parent) :QWidget(parent), image_index(0), right_button_menu(nullptr)
{
	this->setAttribute(Qt::WA_StyledBackground, true);
	this->setStyleSheet("background-color: rgb(230,230,250)");
	_parent = (Dialog_CoorIntersector*)parentWidget();
}

ImageView::~ImageView()
{
	delete right_button_menu;
}

void ImageView::contextMenuEvent(QContextMenuEvent *event)
{
	QPoint pos = event->pos();
	double x = static_cast<double>(pos.x() - m_XPtInterval - this->width() / 2) / m_ZoomValue + m_Image.width() / 2;
	double y = static_cast<double>(pos.y() - m_YPtInterval - this->height() / 2) / m_ZoomValue + m_Image.height() / 2;
	now_image_pos.setX(x);
	now_image_pos.setY(y);
	pos = this->mapToGlobal(pos);
	now_pos = pos;

	_parent->printMsg(u8"选中像点坐标(" + QString::number(now_image_pos.x()) + u8"," + QString::number(now_image_pos.y()) + u8")");

	if (right_button_menu != nullptr)delete right_button_menu;
	right_button_menu = new QMenu(this);
	QAction *presetAction = new QAction(u8"映射点");
	QObject::connect(presetAction, &QAction::triggered, this, &ImageView::intersectPoint);
	right_button_menu->addAction(presetAction);
	right_button_menu->exec(pos);
}
void ImageView::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter painter(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &painter, this);

	if (m_Image.isNull())
		return QWidget::paintEvent(event);
	painter.translate(this->width() / 2 + m_XPtInterval, this->height() / 2 + m_YPtInterval);
	painter.scale(m_ZoomValue, m_ZoomValue);

	QRect picRect(-m_Image.width() / 2, -m_Image.height() / 2, m_Image.width(), m_Image.height());
	painter.drawImage(picRect, m_Image);
}

void ImageView::wheelEvent(QWheelEvent *event)
{
	int value = event->delta();
	if (value > 0)
		onZoomInImage();
	else
		onZoomOutImage();

	this->update();
}

void ImageView::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_OldPos = event->pos();
		m_Pressed = true;
	}
}

void ImageView::mouseMoveEvent(QMouseEvent *event)
{
	if (!m_Pressed)
		return QWidget::mouseMoveEvent(event);

	this->setCursor(Qt::SizeAllCursor);
	QPoint pos = event->pos();
	int xPtInterval = pos.x() - m_OldPos.x();
	int yPtInterval = pos.y() - m_OldPos.y();

	m_XPtInterval += xPtInterval;
	m_YPtInterval += yPtInterval;

	m_OldPos = pos;
	this->update();
}

void ImageView::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		m_Pressed = false;
		this->setCursor(Qt::ArrowCursor);
	}
}

void ImageView::loadImage(QString fileName, int index)
{
	QFile file(fileName);
	if (!file.exists())
	{
		_parent->printMsg(u8"图像文件不存在，请检查图片名称和路径");
		return;
	}
	m_Image.load(fileName);
	image_index = index;
	// 根据窗口计算应该显示的图片缩放比例
	int width = qMin(m_Image.width(), this->width());
	int height = width * 1.0 / (m_Image.width() * 1.0 / m_Image.height());
	height = qMin(height, this->height());
	width = height * 1.0 * (m_Image.width() * 1.0 / m_Image.height());
	m_ZoomValue = static_cast<double>(height) / static_cast<double>(m_Image.height());
	m_XPtInterval = m_YPtInterval = 0;
	this->update();
}


void ImageView::onZoomInImage(void)
{
	m_ZoomValue *= 1.1;
	if (m_ZoomValue > 20)m_ZoomValue = 20;
	this->update();
}

void ImageView::onZoomOutImage(void)
{
	m_ZoomValue *= 0.9;
	if (m_ZoomValue < 0.01)m_ZoomValue = 0.01;
	this->update();
}

void ImageView::intersectPoint()
{
	if (now_image_pos.x() > 0 && now_image_pos.x() < m_Image.width() && now_image_pos.y() > 0 && now_image_pos.y() < m_Image.height())
	{
		_parent->printMsg(u8"正在计算交会点,请稍候...");
		_parent->onIntersection(now_image_pos.x(), now_image_pos.y(), image_index, now_pos);
	}
}

