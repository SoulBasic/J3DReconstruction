#include "dialog_coor.h"
#include "ui_dialog_coor.h"

dialog_coor::dialog_coor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialog_coor)
{
    ui->setupUi(this);
}

dialog_coor::~dialog_coor()
{
	scene.Release();
    delete ui;
}

bool dialog_coor::readImg(int idx)
{
	std::string m_imgname = scene.images[idx].name.c_str();
	cv::Mat m_img = cv::imread(m_imgname);
	if (m_img.empty())
	{
		QMessageBox::critical(this, u8"错误 ", u8"找不到对应的图片: " + QString(img_name.c_str()), QMessageBox::Ok, QMessageBox::Ok);
		return false;
	}
	cv::destroyWindow(img_name);
	img_name = m_imgname;
	img = m_img;
	img_index = idx;
	auto map = maps[img_name];
	for (auto x : map)
	{
		for (auto y : x.second)
		{
			int x1 = x.first;
			int y1 = y.first;
			img.at<cv::Vec3b>(y1, x1)[0] = 255;
			img.at<cv::Vec3b>(y1, x1)[1] = 255;
			img.at<cv::Vec3b>(y1, x1)[2] = 255;
		}
	}
	cv::imshow(img_name, img);
	cv::setMouseCallback(img_name, onMouse, this);
	return true;
}
void dialog_coor::onMouse(int event, int x, int y, int flags, void* param)
{
	auto dlg = reinterpret_cast<dialog_coor*>(param);
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		dlg->readImg(dlg->img_index ? dlg->img_index - 1 : dlg->scene.images.size() - 1);
		//cv::Vec3b p = img2.at<cv::Vec3b>(y, x);
		//printf("R=%d, G=%d, B=%d\n", p[2], p[1], p[0]);
	}
	else if (event == CV_EVENT_RBUTTONDOWN)
	{
		int sz = dlg->scene.images.size();
		dlg->readImg((dlg->img_index == (sz - 1)) ? 0 : dlg->img_index + 1);
		//cvtColor(img, img3, CV_BGR2HSV);
		//cv::Vec3b p = img3.at<cv::Vec3b>(y, x);
		//printf("H=%d, S=%d, V=%d\n", p[0], p[1], p[2]);
	}
	else
	{
		auto map = dlg->maps[dlg->img_name];
		if (map.find(x) != map.end())
		{
			if (map[x].find(y) != map[x].end())
			{
				auto point = map[x][y];
				printf("x = %.2f  y = %.2f  z = %.2f\n", point.x, point.y, point.z);
			}
		}
	}
}
void dialog_coor::on_button_ok_clicked()
{
	QString filename = ui->lineEdit_filename->text();
	if (filename == "")return;
	scene = MVS::Scene(0);

	if (!scene.Load(filename.toStdString(), false))
	{
		QMessageBox::critical(this, u8"错误 ", u8"无法打开J3D文件", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}
	const auto points = scene.pointcloud.points;
	const auto views = scene.pointcloud.pointViews;


	for (int i = 0; i < points.size(); i++)
	{
		auto point = points[i];
		auto view = views[i];
		for (MVS::PointCloud::View idxImage : view) {
			const MVS::Image& imageData = scene.images[idxImage];
			const Point2 x(imageData.camera.TransformPointW2I(Cast<REAL>(points[i])));
			int x1 = x.x * 2;
			int y1 = x.y * 2;
			maps[imageData.name.c_str()][x1][y1] = point;
		}
	}
	readImg(0);
}
