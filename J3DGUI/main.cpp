
#include "QT3DReconstruction.h"
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
	
	QApplication a(argc, argv);
	QT3DReconstruction w;
	w.show();
	return a.exec();
}
