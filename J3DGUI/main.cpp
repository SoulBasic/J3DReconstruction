
#include "QT3DReconstruction.h"
#include <QtWidgets/QApplication>
#include "qapplication.h"
#include "Dialog_MatchFeature.h"


int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	//Dialog_MatchFeature w;
	QT3DReconstruction w;
	w.show();
	return a.exec();
}
