
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

using namespace MVS;
// WGS84 Ellipsoid
static const double WGS84_A = 6378137.0;      // major axis
static const double WGS84_B = 6356752.314245; // minor axis
static const double WGS84_E = 0.0818191908;   // first eccentricity
std::vector<std::string> filenames;
cv::Mat img;
int img_index;
double og_x, og_y, og_z;
std::string workDir;
std::map<int, std::map<int, SEACAVE::Point3f>> map;
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

void onMouse(int event, int x, int y, int flags, void* param)
{
	if (event == CV_EVENT_LBUTTONDOWN)
	{
		readImg(img_index ? img_index - 1 : filenames.size() - 1);
	}
	else if (event == CV_EVENT_RBUTTONDOWN)
	{
		readImg(((filenames.size() - 1) == img_index) ? 0 : img_index + 1);
	}
	else if (event == CV_EVENT_MOUSEWHEEL)
	{
		showPoints = !showPoints;
		readImg(img_index);
	}
	else
	{
		if (map.find(x) != map.end())
		{
			if (map[x].find(y) != map[x].end())
			{
				auto point = map[x][y];
				SEACAVE::Point3d llap = ecef_to_lla(point.x + og_x, point.y + og_y, point.z + og_z);
				printf("lon = %lf  lat = %lf  alt = %lf\n", llap.x, llap.y, llap.z);
			}
		}
	}
}

bool readImg(int idx)
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
			if (showPoints)
			{
				img.at<cv::Vec3b>(y1, x1)[0] = 255;
				img.at<cv::Vec3b>(y1, x1)[1] = 255;
				img.at<cv::Vec3b>(y1, x1)[2] = 255;
			}
		}
	}
	cv::namedWindow("image", CV_WINDOW_NORMAL);
	cv::putText(img, filenames[idx].c_str(), cv::Point(5, 20), cv::FONT_HERSHEY_SIMPLEX, 6, cv::Scalar(0, 255, 0));
	cv::imshow("image", img);
	cv::setMouseCallback("image", onMouse, nullptr);
	fclose(coor_data);
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
		MessageBoxA(nullptr, "找不到坐标映射元数据local_frame_origin.txt ", "无法打开文件 ", MB_OK);
		return -1;
	}
	fscanf(origin_coor, "%lf\n%lf\n%lf", &og_x, &og_y, &og_z);

	getFiles((workDir + "./image_coordinates").c_str(), filenames);

	for (int i = 0; i < filenames.size(); i++)
	{
		filenames[i] = filenames[i].substr(0, filenames[i].length() - 4) + "JPG";
		std::cout << i << " : " << filenames[i] << std::endl;
	}

	std::cout << "以上是可用映射照片序号，请输入需要预览的图片序号(0 - " << filenames.size() - 1 << ")" << std::endl;
	std::cout << "鼠标放置于图片中像素点，会实时显示映射坐标值" << std::endl;
	std::cout << "鼠标 左键/右键 控制 上一张/下一张 图片" << std::endl;
	std::cout << "鼠标滑轮下滑可以 显示/隐藏 可用映射点" << std::endl;
	int idx = 0;
	std::cin >> idx;
	if (idx <= 0 || idx >= filenames.size())idx = 0;
	readImg(idx);
	cvWaitKey(0);
	return 0;
}


