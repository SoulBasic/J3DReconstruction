#ifndef DIALOG_COORINTERSECTOR_H
#define DIALOG_COORINTERSECTOR_H

#include <QDialog>
#include <qfiledialog.h>
#include <qimage.h>
#include <qpixmap.h>
#include <qmessagebox.h>
#include <qstring.h>
#include <vector>
#include <qstringlistmodel.h>
#include "Global.h"
#include <memory>
#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"
#include "TiffData.hpp"
#include <qlabel.h>
#include <chrono>
#include "dialog_pointinfo.h"
#include "dxflib/dl_dxf.h"
#include <qmenu.h>
#include <qevent.h>

namespace Ui {
	class Dialog_CoorIntersector;
}

class Dialog_CoorIntersector : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog_CoorIntersector(QWidget *parent = nullptr);
	~Dialog_CoorIntersector();
	void printMsg(const QString& msg);
	void intersectPoint(int x, int y, int image_index);
	void onIntersection(int x, int y, int image_index, QPoint& now_pos);
	void addPoint(double x, double y, double z);
private slots:
	void on_toolButton_imageDir_clicked();
	void on_toolButton_dsmDir_clicked();
	void on_listView_images_clicked(QModelIndex index);
	void show_picked_points_menu(const QPoint& pos);
	void on_pushButton_outputDXF_clicked();
	void on_delete_point_clicked();
private:
	void read_dsm_file(QString fileName);
	void clear_sfm_file();
	bool write_to_dxf(const std::string& fileName);

	QStringListModel* images_model;
	QStringListModel* picked_points_model;
	QString workDir;
	TiffData* tiffData;
	MVS::Scene* scene;
	Ui::Dialog_CoorIntersector *ui;
	std::vector<SEACAVE::Point3d> picked_points;
	Dialog_PointInfo dlg_point_info;
	bool have_DSM_data;
	QMenu* picked_points_menu;
};

#endif // DIALOG_COORINTERSECTOR_H
