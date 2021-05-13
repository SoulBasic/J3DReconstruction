#include "TiffData.h"


bool TiffData::read(const std::string & fileName)
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

void TiffData::find_thread(TiffData* tiffData, MVS::Image* img, int low, int high, int x, int y)
{
	auto& cam = img->camera;
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

SEACAVE::Point3d TiffData::find(MVS::Image* img, int x, int y)
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



