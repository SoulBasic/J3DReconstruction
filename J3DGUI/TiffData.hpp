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

constexpr double conepr_p = 180.0 / M_PI * 3600;
#define PI				3.1415926535897932384626433832795
#define D2R(d)			((d)*(PI/180.0)) // degree to radian
#define R2D(r)			((r)*(180.0/PI)) // radian to degree
#define WGS84_A 6378137.0
#define WGS84_B  6356752.314245
#define WGS84_E 0.0818191908
class TiffData
{
public:
	TiffData() :width(-1), height(-1), og_x(-1), og_y(-1), og_z(-1), alt_data(nullptr), thread_num(1) {}
	~TiffData()
	{
		delete alt_data;
	}
	inline int Width() { return width; }
	inline int Height() { return height; }
	inline TIFFBYTE* Alt_data() { return alt_data; }
	inline double* Trans() { return trans; }
	inline bool isEmpty() { return alt_data == nullptr; }
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
		thread_num = (width + height) / 3000;
		if (thread_num < 3) thread_num = 3;
		if (thread_num > 25)thread_num = 25;
		return true;
	}

	SEACAVE::Point3d find(MVS::Image& img, int x, int y)
	{
		min_distance = DBL_MAX;
		ret_point.x = ret_point.y = ret_point.z = NAN;
		int thread_task = height / thread_num;
		running_thread = thread_num;
		double x1 = static_cast<double>(x);
		double y1 = static_cast<double>(y);
		for (int i = 0; i < thread_num; i++)
		{
			int low = i * thread_task;
			int high = low + thread_task;
			if (high > height)high = height;
			std::thread(&TiffData::find_thread, this, img, low, high, x1, y1).detach();
		}
		while (running_thread > 0)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(120));
		}
		return ret_point;
	}

	void set_origin_from_file(const std::string& fileName)
	{
		FILE* origin_coor = fopen(fileName.c_str(), "rt");
		if (origin_coor == nullptr)
		{
			std::cout << "找不到坐标映射元数据local_frame_origin.txt, 默认使用原点作为计算点" << std::endl;
			og_x = og_y = og_z = 0;
		}
		else
		{
			fscanf(origin_coor, "%lf\n%lf\n%lf", &og_x, &og_y, &og_z);
		}
	}

	static SEACAVE::Point3d lla_to_ecef(const SEACAVE::Point3d& p) { return lla_to_ecef(p.x, p.y, p.z); }
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
	static SEACAVE::Point3d ecef_to_lla(const SEACAVE::Point3d& p) { return ecef_to_lla(p.x, p.y, p.z); }
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

	static SEACAVE::Point3d rad2dms(double rad)
	{
		SEACAVE::Point3d a;
		double dms = rad * conepr_p;
		a.x = dms / 3600.0;
		a.y = (dms - a.x * 3600) / 60.0;
		a.z = static_cast<int>((dms - a.x * 3600)) - a.y * 60;
		return a;
	}

	static inline double dms2dec(SEACAVE::Point3d dms)
	{
		return dms.x + dms.y / 60.0 + dms.z / 3600.0;
	}

	static SEACAVE::Point3d cgcs2000_117E_to_lla(double x, double y, double z)
	{
		constexpr double a = 6378137.0; //major semi axis
		constexpr double efang = 0.0066943799901413; //square of e
		constexpr double e2fang = 0.0067394967422764; //suqre of e2
		constexpr double a0 = 6367449.14576629083603620529;
		constexpr double a2 = 32077.01723072680033510551;
		constexpr double a4 = 67.33039860364382889202;
		constexpr double a6 = 0.13188597743733651568;
		constexpr double a8 = 0.00024462948002847807;

		double FBf = 0;
		double Bf0 = x / a0, Bf1 = 0;
		y -= 500000;
		//计算Bf的值，直到满足条件
		while ((Bf0 - Bf1) >= 0.0001) {
			Bf1 = Bf0;
			FBf = -a2 * sin(2 * Bf0) / 2 + a4 * sin(4 * Bf0) / 4 - a6 * sin(6 * Bf0) / 6 + a8 * sin(8 * Bf0) / 8;
			Bf0 = (x - FBf) / a0;
		}
		double Bf = Bf0;
		//计算公式中参数
		double Wf = sqrt(1 - efang * sin(Bf) * sin(Bf));
		double Nf = a / Wf;
		double Mf = a * (1 - efang) / pow(Wf, 3);
		double nffang = e2fang * cos(Bf) * cos(Bf);
		double tf = tan(Bf);
		double B = Bf - tf * y * y / (2 * Mf * Nf) + tf * (5 + 3 * tf * tf + nffang - 9 * nffang * tf * tf) * pow(y, 4) / (24 * Mf * pow(Nf, 3)) - tf * (61 + 90 * tf * tf + 45 * pow(tf, 4)) * pow(y, 6) / (720 * Mf * pow(Nf, 5));
		double l = y / (Nf*cos(Bf)) - (1 + 2 * tf*tf + nffang)*pow(y, 3) / (6 * pow(Nf, 3)*cos(Bf)) + (5 + 28 * tf*tf + 24 * pow(tf, 4))*pow(y, 5) / (120 * pow(Nf, 5)*cos(Bf));
		//转化成为十进制经纬度格式
		auto array_B = rad2dms(B);
		auto array_L = rad2dms(l);
		double Bdec = dms2dec(array_B);
		double Ldec = dms2dec(array_L) + 117;
		return SEACAVE::Point3d{ Bdec, Ldec,z };
	}



	static void find_thread(TiffData* tiffData, MVS::Image& img, int low, int high, double x, double y)
	{
		auto& cam = img.camera;
		double lon, lat;
		TIFFBYTE* altData = tiffData->alt_data + tiffData->width * low;
		SEACAVE::Point3d point;
		SEACAVE::Point3d localized_point;
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
				localized_point = tiffData->localize(point);
				img_point = tiffData->get_image_point(localized_point, cam);
				if (std::abs(img_point.x - x) < 1 && std::abs(img_point.y - y) < 1)
				{
					auto distance = tiffData->get_point_distance(localized_point, cam);
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
	int thread_num;
	inline SEACAVE::Point3d localize(const SEACAVE::Point3d& p) { return SEACAVE::Point3d{ p.x - og_x, p.y - og_y, p.z - og_z }; }
	inline SEACAVE::Point2d get_image_point(const SEACAVE::Point3d& point, Camera& cam) { return cam.TransformPointW2I(point); }
	inline double get_point_distance(const SEACAVE::Point3d& point, Camera& cam) { return cam.Distance(point); }

};
