#ifndef MVSVIEWER_H
#define MVSVIEWER_H
#include <QWidget>


class mvsviewer :public QWidget
{
	Q_OBJECT
public:
	mvsviewer(QWidget *parent = nullptr);
	mvsviewer(int i, QWidget *parent = nullptr);
	~mvsviewer();
private:


};

#endif // MVSVIEWER_H
