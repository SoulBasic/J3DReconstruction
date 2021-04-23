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
private:
	typedef std::map<int, std::map<int, SEACAVE::Point3f>> image_map;
	bool readImg(int idx);
	static void onMouse(int event, int x, int y, int flags, void * param);
    Ui::dialog_coor *ui;
	MVS::Scene scene;
	int img_index;
	cv::Mat img;
	std::string img_name;
	std::unordered_map<std::string, image_map> maps;

};

#endif // DIALOG_COOR_H
