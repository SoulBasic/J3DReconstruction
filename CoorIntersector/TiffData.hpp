#pragma once
#include <vector>
#include "libs/MVS/Common.h"
#include "libs/MVS/Scene.h"
#include "gdal_priv.h"
#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>


#define TIFFBYTE float
using namespace std;
using namespace MVS;


#define PI				3.1415926535897932384626433832795
#define D2R(d)			((d)*(PI/180.0)) // degree to radian
#define R2D(r)			((r)*(180.0/PI)) // radian to degree
#define WGS84_A 6378137.0
#define WGS84_B  6356752.314245
#define WGS84_E 0.0818191908
#define THREAD_NUM 20
class TiffData
{
public:
	TiffData() :width(-1), height(-1), og_x(-1), og_y(-1), og_z(-1), alt_data(nullptr) {}
	~TiffData()
	{
		delete alt_data;
	}
	inline int Width() { return width; }
	inline int Height() { return height; }
	inline TIFFBYTE* Alt_data() { return alt_data; }
	inline double* Trans() { return trans; }
	bool read(const std::string& fileName)
	{
		GDALDataset *poDataset;
		GDALAllRegister();
		poDataset = (GDALDataset *)GDALOpen(fileName.c_str(), GA_ReadOnly);
		if (poDataset == NULL)
		{
			cout << "打开tif文件失败!" << endl;
			return false;
		}
		GDALRasterBand *poBand1;
		poBand1 = poDataset->GetRasterBand(1);

		width = poDataset->GetRasterXSize();
		height = poDataset->GetRasterYSize();

		CPLErr aaa = poDataset->GetGeoTransform(trans);

		alt_data = (TIFFBYTE *)malloc(sizeof(TIFFBYTE)*(width)*(height));
		if (alt_data == NULL)
		{
			printf("内存申请失败，tif文件过大\n");
			return false;
		}
		poBand1->RasterIO(GF_Read, 0, 0, width, height, alt_data, width, height, poBand1->GetRasterDataType(), 0, 0);
		//for (int i = 0; i < height; i++)
		//{
		//	for (int j = 0; j < width; j++)
		//	{
		//		auto elevation = *alt_data++;
		//		if (elevation + FLT_MAX < 1e-5)continue;
		//		auto Xgeo = trans[0] + j * trans[1];
		//		auto Ygeo = trans[3] + i * trans[5];
		//		std::setiosflags(std::ios::fixed);
		//		std::cout << std::setprecision(15)
		//			<< Xgeo << "\t" << Ygeo << "\t" << elevation << std::endl;
		//	}
		//}
		delete poDataset;
		return true;

	}

	SEACAVE::Point3d find(MVS::Image& img, int x, int y)
	{
		min_distance = DBL_MAX;
		ret_point.x = ret_point.y = ret_point.z = NAN;
		int thread_task = height / THREAD_NUM;
		running_thread = THREAD_NUM;
		for (int i = 0; i < THREAD_NUM; i++)
		{
			int low = i * thread_task;
			int high = low + thread_task;
			if (high > height)high = height;
			std::thread(&TiffData::find_thread, this, img, low, high, x, y).detach();
		}
		while (running_thread > 0)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
		return ret_point;
	}

	inline void setOrigin(double x, double y, double z) { og_x = x; og_y = y; og_z = z; }

	static SEACAVE::Point3d lla_to_ecef(double lat, double lon, double alt)
	{
		const double clat = cos(D2R(lat));
		const double slat = sin(D2R(lat));
		const double clon = cos(D2R(lon));
		const double slon = sin(D2R(lon));

		const double a2 = WGS84_A * WGS84_A;
		const double b2 = WGS84_B * WGS84_B;

		const double L = 1.0 / sqrt(a2 * clat * clat + b2 * slat * slat);
		const double x = (a2 * L + alt) * clat * clon;
		const double y = (a2 * L + alt) * clat * slon;
		const double z = (b2 * L + alt) * slat;

		return SEACAVE::Point3d(x, y, z);
	}

	static SEACAVE::Point3d ecef_to_lla(double x, double y, double z)
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

	static void find_thread(TiffData* tiffData, MVS::Image& img, int low, int high, int x, int y)
	{
		auto& cam = img.camera;
		double lon, lat;
		TIFFBYTE* altData = tiffData->alt_data + tiffData->width * low;
		SEACAVE::Point3d point;
		SEACAVE::Point2d img_point;
		TIFFBYTE elevation;
		std::unique_lock<std::mutex> lock(tiffData->_mtx);
		lock.unlock();
		for (int i = low; i < high; i++)
		{
			for (int j = 0; j < tiffData->width; j++)
			{
				elevation = *altData++;
				if (elevation + FLT_MAX < 1e-5)continue;
				lon = tiffData->trans[0] + j * tiffData->trans[1];
				lat = tiffData->trans[3] + i * tiffData->trans[5];
				point = SEACAVE::Point3d{ lon ,lat ,elevation };
				img_point = tiffData->get_image_point(point, cam);
				if (std::abs(static_cast<int>(img_point.x) - x) < 2 && std::abs(static_cast<int>(img_point.y) - y) < 2)
				{
					auto distance = tiffData->get_point_distance(point, cam);
					lock.lock();
					if (distance < tiffData->min_distance)
					{
						tiffData->ret_point = point;
						tiffData->min_distance = distance;
					}
					lock.unlock();
				}
			}
		}
		tiffData->running_thread--;
	}
private:
	int width;
	int height;
	TIFFBYTE* alt_data;
	double trans[6];
	double og_x;
	double og_y;
	double og_z;
	double min_distance;
	SEACAVE::Point3d ret_point;
	std::mutex _mtx;
	std::atomic_int running_thread;

	inline void localize(SEACAVE::Point3d& p) { p.x -= og_x; p.y -= og_y; p.z -= og_z; }

	SEACAVE::Point2d get_image_point(SEACAVE::Point3d& point, Camera& cam)
	{
		auto real_point = lla_to_ecef(point.y, point.x, point.z);
		localize(real_point);
		return cam.TransformPointW2I(real_point);
	}

	double get_point_distance(SEACAVE::Point3d& point, Camera& cam)
	{
		auto real_point = lla_to_ecef(point.y, point.x, point.z);
		localize(real_point);
		return cam.Distance(real_point);
	}

};
