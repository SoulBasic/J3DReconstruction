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
#define THREAD_NUM 10
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
	bool read(const std::string& fileName);
	SEACAVE::Point3d find(MVS::Image* img, int x, int y);
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

	static void find_thread(TiffData* tiffData, MVS::Image* img, int low, int high, int x, int y);
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
