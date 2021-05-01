#include "dialog_coor.h"
#include "ui_dialog_coor.h"
// WGS84 Ellipsoid
#define WGS84_A 6378137.0      // major axis
#define WGS84_B 6356752.314245 // minor axis
#define WGS84_E 0.0818191908   // first eccentricity
dialog_coor::dialog_coor(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::dialog_coor)
{
	ui->setupUi(this);
}

dialog_coor::~dialog_coor()
{
	WinExec("taskkill /f /im CoorIntersector.exe", SW_HIDE);
	delete ui;
}
void dialog_coor::on_pushButton_browseOutputDir_clicked()
{
	QString dir = QFileDialog::getExistingDirectory(this, u8"浏览重建数据文件夹 ", "", NULL);
	ui->lineEdit_filename->setText(dir);
}
void dialog_coor::setCoordinate(SEACAVE::Point3d p)
{
	//sprintf(buf, "lon = %lf lat = %lf alt = %lf", p.x, p.y, p.z);
	//ui->label_coor->setText(buf);
}
int dialog_coor::getFiles(const char* path, std::vector<std::string>& arr)
{
	int num_of_img = 0;
	intptr_t hFile = 0;
	struct _finddata_t fileinfo;
	char p[700] = { 0 };
	strcpy(p, path);
	strcat(p, "\\*");
	char buf[256];
	if ((hFile = _findfirst(p, &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					continue;
			}
			else
			{

				strcpy(buf, path);
				strcat(buf, "\\");
				strcat(buf, fileinfo.name);
				struct stat st1;
				stat(buf, &st1);
				arr.push_back(fileinfo.name);
			}
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
	return num_of_img;
}

SEACAVE::Point3d dialog_coor::ecef_to_lla(double x, double y, double z)
{
	const double b = sqrt(WGS84_A*WGS84_A*(1 - WGS84_E * WGS84_E));
	const double ep = sqrt((WGS84_A*WGS84_A - b * b) / (b*b));
	const double p = hypot(x, y);
	const double th = atan2(WGS84_A*z, b*p);
	const double lon = atan2(y, x);
	const double lat = atan2((z + ep * ep*b* pow(sin(th), 3)), (p - WGS84_E * WGS84_E*WGS84_A*pow(cos(th), 3)));
	const double N = WGS84_A / sqrt(1 - WGS84_E * WGS84_E*sin(lat)*sin(lat));
	const double alt = p / cos(lat) - N;

	return SEACAVE::Point3d{ R2D(lat), R2D(lon), alt };
}

bool dialog_coor::readImg(int idx)
{
	cv::Mat img = cv::imread(workDir + "/undistorted_images/" + filenames[idx]);
	if (img.empty())return false;
	img_index = idx;
	std::string fname = workDir + "/image_coordinates/" + std::string(Util::getFileName(filenames[idx]).c_str()) + ".coor";
	FILE* coor_data = fopen(fname.c_str(), "rb");
	//cv::resize(m_img, img, cv::Size(m_img.cols * 0.5, m_img.rows*0.5));
	cv::destroyWindow("image");
	map.clear();
	while (!feof(coor_data))
	{
		char buf[20];
		fread(buf, 20, 1, coor_data);
		int* p = reinterpret_cast<int*>(buf);
		int x = *p;
		p++;
		int y = *p;
		p++;
		float* k = reinterpret_cast<float*>(p);
		Point3f point;
		point.x = *k;
		k++;
		point.y = *k;
		k++;
		point.z = *k;
		map[x][y] = point;
	}
	for (auto x : map)
	{
		for (auto y : x.second)
		{
			int x1 = x.first;
			int y1 = y.first;
			if (x1 < 0 || y1 < 0 || x1 >= img.cols || y1 >= img.rows)continue;
			img.at<cv::Vec3b>(y1, x1)[0] = 255;
			img.at<cv::Vec3b>(y1, x1)[1] = 255;
			img.at<cv::Vec3b>(y1, x1)[2] = 255;
		}
	}
	cv::namedWindow("image", CV_WINDOW_NORMAL);
	cv::putText(img, filenames[idx].c_str(), cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, 10, cv::Scalar(0, 255, 0));
	cv::imshow("image", img);
	cv::setMouseCallback("image", onMouse, nullptr);
	Global::dlg = this->ui;
	fclose(coor_data);
	return true;
}

void dialog_coor::onMouse(int event, int x, int y, int flags, void* param)
{
	dialog_coor* dlg = reinterpret_cast<dialog_coor*>(Global::dlg);
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		dlg->readImg(dlg->img_index ? dlg->img_index - 1 : dlg->filenames.size() - 1);
	}
	else if (event == CV_EVENT_RBUTTONDOWN)
	{
		dlg->readImg(((dlg->filenames.size() - 1) == dlg->img_index) ? 0 : dlg->img_index + 1);
	}
	else
	{
		if (dlg->map.find(x) != dlg->map.end())
		{
			if (dlg->map[x].find(y) != dlg->map[x].end())
			{
				auto point = dlg->map[x][y];
				dlg->setCoordinate(ecef_to_lla(point.x + dlg->og_x, point.y + dlg->og_y, point.z + dlg->og_z));
			}
		}
	}
}

void dialog_coor::on_button_ok_clicked()
{
	QString workDir = ui->lineEdit_filename->text();
	QString cmd = "CoorIntersector.exe " + workDir;
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = true;

	if (!CreateProcess(
		NULL,
		(LPSTR)cmd.toStdString().c_str(),
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi))
	{
		QMessageBox::information(NULL, u8"失败", u8"打开文件失败，CoorIntersector程序文件不存在或不完整 ", QMessageBox::Ok, QMessageBox::Ok);
		return;
	}



	//FILE* origin_coor = fopen((workDir + "/local_frame_origin.txt").c_str(), "rt");
	//if (origin_coor == nullptr)
	//{
	//	QMessageBox::information(NULL, u8"错误 ", u8"找不到local_frame_origin文件,请检查目录是否正确", QMessageBox::Ok, QMessageBox::Ok);
	//	return;
	//}
	//fscanf(origin_coor, "%lf\n%lf\n%lf", &og_x, &og_y, &og_z);

	//getFiles((workDir + "/image_coordinates").c_str(), filenames);
	//for (int i = 0; i < filenames.size(); i++)
	//{
	//	filenames[i] = filenames[i].substr(0, filenames[i].length() - 4) + "JPG";
	//	std::cout << i << " : " << filenames[i] << std::endl;
	//}
	//readImg(0);
}
