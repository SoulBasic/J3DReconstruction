#include <iostream>
#include <string>
#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"
#include "opencv2/highgui.hpp"
#include "opencv2/opencv.hpp"
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <Windows.h>
#include <direct.h>
#include <cstdio>
#include "opencv2\opencv.hpp"
#include <cmath>
#include <list>
#include "dxflib/dl_dxf.h"
#define max_count_of_opened_file 8
using namespace MVS;
typedef std::map<int, std::map<int, SEACAVE::Point3f>> point_map;
struct opened_map
{
	std::string fname;
	point_map* f_ptr;
	opened_map(std::string fn, point_map* map) :fname(fn), f_ptr(map) {}
};

// WGS84 Ellipsoid
static const double WGS84_A = 6378137.0;      // major axis
static const double WGS84_B = 6356752.314245; // minor axis
static const double WGS84_E = 0.0818191908;   // first eccentricity
std::vector<std::string> filenames;
cv::Mat img;
int img_index;
double og_x, og_y, og_z;
std::string workDir;

point_map* now_map_p;
std::vector<SEACAVE::Point3d> picked_points;
std::list<opened_map> opened_coor_files;
bool readImg(int idx);
bool showPoints;



SEACAVE::Point3d ecef_to_lla(double x, double y, double z)
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

int getFiles(const char* path, std::vector<std::string>& arr)
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

bool getPoint(int x, int y, SEACAVE::Point3d& point)
{
	point_map& now_map = *now_map_p;
	if (now_map.find(x) != now_map.end())
	{
		if (now_map[x].find(y) != now_map[x].end())
		{
			auto p = now_map[x][y];
			point = ecef_to_lla(p.x + og_x, p.y + og_y, p.z + og_z);
			return true;
		}
	}
	return false;
}

void onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONUP)
	{
		SEACAVE::Point3d point;
		if (
			getPoint(x, y, point) ||
			getPoint(x - 1, y - 1, point) ||
			getPoint(x - 1, y, point) ||
			getPoint(x - 1, y + 1, point) ||
			getPoint(x, y - 1, point) ||
			getPoint(x, y + 1, point) ||
			getPoint(x + 1, y - 1, point) ||
			getPoint(x + 1, y, point) ||
			getPoint(x + 1, y + 1, point)
			)
		{
			picked_points.push_back(point);
			printf("添加成功,目前总点数为%d\t(lon = %10lf\tlat = %10lf\talt = %10lf)\n", picked_points.size(), point.x, point.y, point.z);
		}
		else
		{
			printf("鼠标处无有效点,请重试\n");
			return;
		}
	}
	else if (event == CV_EVENT_RBUTTONUP)
	{
		if (picked_points.size() > 0)
		{
			picked_points.pop_back();
			printf("删除成功,目前总点数为%d\n", picked_points.size());
		}
		else
		{
			printf("无法删除,目前点集为空\n");
		}

	}
	else if (event == CV_EVENT_MOUSEWHEEL)
	{

	}
	else
	{

	}
}

opened_map* findMap(const std::string& fname)
{
	for (opened_map f : opened_coor_files)
	{
		if (f.fname == fname)
		{
			return &f;
		}
	}
	return nullptr;
}

void show_points()
{
	point_map& now_map = *now_map_p;
	for (auto x : now_map)
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
}

bool readImg(int idx)
{
	img = cv::imread(workDir + "/undistorted_images/" + filenames[idx]);
	if (img.empty())return false;
	cv::destroyWindow(filenames[img_index].c_str());
	img_index = idx;
	std::string fname = workDir + "/image_coordinates/" + std::string(Util::getFileName(filenames[idx]).c_str()) + ".coor";

	opened_map* mp = findMap(fname);
	if (mp == nullptr)
	{
		FILE* coor_data = fopen(fname.c_str(), "rb");
		if (coor_data == nullptr)
		{
			printf("无法打开coor文件\n");
			return false;
		}
		if (opened_coor_files.size() >= max_count_of_opened_file)
		{
			opened_map temp = opened_coor_files.front();
			delete temp.f_ptr;
			opened_coor_files.pop_front();
		}
		point_map* newmap = new point_map();
		now_map_p = newmap;
		point_map& now_map = *now_map_p;
		opened_coor_files.push_back(opened_map(fname, newmap));
		now_map.clear();
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
			now_map[x][y] = point;
		}
		fclose(coor_data);
	}
	else
	{
		now_map_p = mp->f_ptr;
	}
	if (showPoints)show_points();
	cv::namedWindow(filenames[idx].c_str(), CV_WINDOW_NORMAL);
	cv::imshow(filenames[idx].c_str(), img);
	cv::setMouseCallback(filenames[idx].c_str(), onMouse, nullptr);
	return true;
}


bool write_to_dxf(const std::string& fileName)
{
	DL_Dxf* dxf = new DL_Dxf();
	DL_Codes::version exportVersion = DL_Codes::AC1015;
	DL_WriterA* dw = dxf->out(fileName.c_str(), exportVersion);
	if (dw == NULL) {
		printf("无法输出到目标路径，请检查路径或使用管理员权限重试\n");
		return false;
	}
	dxf->writeHeader(*dw);
	dw->sectionEnd();
	dw->sectionTables();
	dxf->writeVPort(*dw);

	dw->tableLinetypes(3);
	dxf->writeLinetype(*dw, DL_LinetypeData("BYBLOCK", "BYBLOCK", 0, 0, 0.0));
	dxf->writeLinetype(*dw, DL_LinetypeData("BYLAYER", "BYLAYER", 0, 0, 0.0));
	dxf->writeLinetype(*dw, DL_LinetypeData("CONTINUOUS", "Continuous", 0, 0, 0.0));
	dw->tableEnd();

	int numberOfLayers = 3;
	dw->tableLayers(numberOfLayers);

	dxf->writeLayer(*dw,
		DL_LayerData("0", 0),
		DL_Attributes(
			std::string(""),      // leave empty
			DL_Codes::black,        // default color
			100,                  // default width
			"CONTINUOUS", 1.0));       // default line style

	dxf->writeLayer(*dw,
		DL_LayerData("mainlayer", 0),
		DL_Attributes(
			std::string(""),
			DL_Codes::white,
			100,
			"CONTINUOUS", 1.0));

	dxf->writeLayer(*dw,
		DL_LayerData("textlayer", 0),
		DL_Attributes(
			std::string(""),
			DL_Codes::white,
			100,
			"CONTINUOUS", 1.0));

	dw->tableEnd();

	dw->tableStyle(1);
	dxf->writeStyle(*dw, DL_StyleData("standard", 0, 2.5, 1.0, 0.0, 0, 2.5, "txt", ""));
	dw->tableEnd();

	dxf->writeView(*dw);
	dxf->writeUcs(*dw);

	dw->tableAppid(1);
	dxf->writeAppid(*dw, "ACAD");
	dw->tableEnd();

	dxf->writeDimStyle(*dw, 1, 1, 1, 1, 1);

	dxf->writeBlockRecord(*dw);
	dxf->writeBlockRecord(*dw, "myblock1");
	dxf->writeBlockRecord(*dw, "myblock2");
	dw->tableEnd();

	dw->sectionEnd();

	dw->sectionBlocks();
	dxf->writeBlock(*dw, DL_BlockData("*Model_Space", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Model_Space");
	dxf->writeBlock(*dw, DL_BlockData("*Paper_Space", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Paper_Space");
	dxf->writeBlock(*dw, DL_BlockData("*Paper_Space0", 0, 0.0, 0.0, 0.0));
	dxf->writeEndBlock(*dw, "*Paper_Space0");

	dxf->writeBlock(*dw, DL_BlockData("myblock1", 0, 0.0, 0.0, 0.0));
	// ...
	// write block entities e.g. with dxf->writeLine(), ..
	// ...
	dxf->writeEndBlock(*dw, "myblock1");

	dxf->writeBlock(*dw, DL_BlockData("myblock2", 0, 0.0, 0.0, 0.0));
	// ...
	// write block entities e.g. with dxf->writeLine(), ..
	// ...
	dxf->writeEndBlock(*dw, "myblock2");

	dw->sectionEnd();
	dw->sectionEntities();


	double sum = 0;
	int len = picked_points.size();
	for (int i = 0; i < len; i++)
		sum += picked_points[i].y;
	double avg = sum / len;
	sum = 0;
	for (int i = 0; i < len; i++)
		sum += pow(picked_points[i].y - avg, 2);
	double gap = sqrt(sum / len) / 20;
	for (int i = 0; i < picked_points.size(); i++)
	{
		auto& point = picked_points[i];
		dxf->writePoint(
			*dw,
			DL_PointData(point.x,
				point.y,
				point.z),
			DL_Attributes("mainlayer", 255, -1, "BYLAYER", 1.0));

		dxf->writeText(*dw, DL_TextData(point.x, point.y + gap, point.z,
			0.0, 0.0, 0.0,
			gap, 0,
			0, 0, 0,
			std::to_string(i),
			"Standard",
			0),
			DL_Attributes("textlayer", 255, -1, "BYLAYER", 1.0)
		);
	}
	dw->sectionEnd();

	dxf->writeObjects(*dw);
	dxf->writeObjectsEnd(*dw);

	dw->dxfEOF();
	dw->close();
	delete dw;
	delete dxf;
	return true;
}

int main(int argc, char* argv[])
{
	workDir = argv[1];
	showPoints = true;
	if (workDir.size() == 0)
	{
		MessageBoxA(nullptr, "重建路径错误或重建模型无效 ", "无法打开文件 ", MB_OK);
		return -1;
	}
	FILE* origin_coor = fopen((workDir + "/local_frame_origin.txt").c_str(), "rt");
	if (origin_coor == nullptr)
	{
		std::cout << "找不到坐标映射元数据local_frame_origin.txt, 默认使用原点作为计算点" << std::endl;
		og_x = og_y = og_z = 0;
	}
	else
	{
		fscanf(origin_coor, "%lf\n%lf\n%lf", &og_x, &og_y, &og_z);
	}

	getFiles((workDir + "./image_coordinates").c_str(), filenames);

	for (int i = 0; i < filenames.size(); i++)
	{
		filenames[i] = filenames[i].substr(0, filenames[i].length() - 4) + "JPG";
		std::cout << i << " : " << filenames[i] << std::endl;
	}

	std::cout << "以上是可用映射照片序号，请输入需要预览的图片序号(0 - " << filenames.size() - 1 << ")" << std::endl;
	std::cout << "鼠标左键选择目标点，并将坐标值添加到点集中" << std::endl;
	std::cout << "鼠标右键可以删除上一个被添加到点集中的点" << std::endl;
	std::cout << "键盘 -/+切换图片; s显示/隐藏可用点; p显示已选中的点集信息; o输出到dxf文件" << std::endl;
	int idx = 0;
	std::cin >> idx;
	if (idx <= 0 || idx >= filenames.size())idx = 0;
	readImg(idx);
	while (true)
	{
		auto key = cv::waitKey(10);
		switch (key)
		{
		case '-':
		{
			readImg(img_index ? img_index - 1 : filenames.size() - 1);
			break;
		}
		case '=':
		case '+':
		{
			readImg(((filenames.size() - 1) == img_index) ? 0 : img_index + 1);
			break;
		}
		case 's':
		{
			showPoints = !showPoints;
			readImg(img_index);
			break;
		}
		case 'p':
		{
			printf("目前已选中的点集如下:\n--------------------------------------------------\n");
			for (int i = 0; i < picked_points.size(); i++)
			{
				SEACAVE::Point3d& point = picked_points[i];
				printf("点号:%d\tlon = %lf\tlat = %lf\talt = %lf\n", i, point.x, point.y, point.z);
			}
			printf("--------------------------------------------------\n");
			break;
		}
		case 'o':
		{
			if (picked_points.size() == 0)
			{
				printf("没有点可以输出！\n");
				break;
			}
			if (write_to_dxf(workDir + "/picked_points.dxf"))
			{
				printf("已成功保存到文件picked_points.dxf");
			}
			break;
		}
		default:
			break;
		}
		if (key == 27)
		{
			break;
		}

	}
	return 0;
}


