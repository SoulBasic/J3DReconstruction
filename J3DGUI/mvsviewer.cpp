#include "mvsviewer.h"
#include <qstring>
#include "Windows.h"
#include "qwindow.h"
mvsviewer::mvsviewer(QWidget *parent)
	: QWidget(parent)
{
}

mvsviewer::mvsviewer(int i, QWidget *parent)
	: QWidget(parent)
{
	i++;
	WId wid = (WId)FindWindow("GLFW30", "J3D Viewer");
	QWindow*m_window;
	m_window = QWindow::fromWinId(wid);
	m_window->setFlags(m_window->flags() | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	QWidget *m_widget;
	m_widget = QWidget::createWindowContainer(m_window, this);
	m_widget->setMinimumSize(1361, 661);
}
mvsviewer::~mvsviewer() {

}



