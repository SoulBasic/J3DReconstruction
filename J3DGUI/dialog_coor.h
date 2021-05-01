#ifndef DIALOG_COOR_H
#define DIALOG_COOR_H

#include <QDialog>
#include <string>
#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <qmessagebox.h>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <qstring.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include "Global.h"
namespace Ui {
class dialog_coor;
}

class dialog_coor : public QDialog
{
    Q_OBJECT

public:
    explicit dialog_coor(QWidget *parent = nullptr);
    ~dialog_coor();

private slots:
	void on_button_ok_clicked();
	void on_pushButton_browseOutputDir_clicked();
private:
	void setCoordinate(SEACAVE::Point3d p);
	char buf[128];
	std::map<int, std::map<int, SEACAVE::Point3f>> map;
	std::string workDir;
	std::vector<std::string> filenames;
	bool readImg(int idx);
	static int getFiles(const char* path, std::vector<std::string>& arr);
	static void onMouse(int event, int x, int y, int flags, void * param);
	static SEACAVE::Point3d ecef_to_lla(double x, double y, double z);
    Ui::dialog_coor *ui;
	cv::Mat img;
	int img_index;
	double og_x, og_y, og_z;
};

#endif // DIALOG_COOR_H
