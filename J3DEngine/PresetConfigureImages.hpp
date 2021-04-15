#pragma once

#include <atomic>
#include <cstdlib>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <iostream>

#include "openMVG/cameras/cameras.hpp"
#include "openMVG/exif/exif_IO_EasyExif.hpp"
#include "openMVG/exif/sensor_width_database/ParseDatabase.hpp"
#include "openMVG/geodesy/geodesy.hpp"
#include "openMVG/image/image_io.hpp"
#include "openMVG/numeric/eigen_alias_definition.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/sfm/sfm_data_utils.hpp"
#include "openMVG/sfm/sfm_view.hpp"
#include "openMVG/sfm/sfm_view_priors.hpp"
#include "openMVG/types.hpp"
#include "openMVG/features/akaze/image_describer_akaze_io.hpp"
#include "openMVG/features/sift/SIFT_Anatomy_Image_Describer_io.hpp"
#include "openMVG/features/regions_factory_io.hpp"
#include "openMVG/system/timer.hpp"

#include "third_party/cmdLine/cmdLine.h"
#include "third_party/progress/progress_display.hpp"
#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <cereal/archives/json.hpp>
#include <cereal/details/helpers.hpp>

#include "nonFree/sift/SIFT_describer_io.hpp"
#include "Global.h"


#ifdef OPENMVG_USE_OPENMP
#include <omp.h>
#endif

using namespace openMVG;
using namespace openMVG::cameras;
using namespace openMVG::exif;
using namespace openMVG::geodesy;
using namespace openMVG::image;
using namespace openMVG::sfm;
using namespace openMVG::image;
using namespace openMVG::features;
using namespace std;


bool checkIntrinsicStringValidity(const std::string & Kmatrix, double & focal, double & ppx, double & ppy)
{
	std::vector<std::string> vec_str;
	stl::split(Kmatrix, ';', vec_str);
	if (vec_str.size() != 9) {
		return false; //Missing ';' character
	}

	for (size_t i = 0; i < vec_str.size(); ++i) {
		double readvalue = 0.0;
		std::stringstream ss;
		ss.str(vec_str[i]);
		if (!(ss >> readvalue)) {
			return false;
		}
		if (i == 0) focal = readvalue;
		if (i == 2) ppx = readvalue;
		if (i == 5) ppy = readvalue;
	}
	return true;
}

std::pair<bool, Vec3> checkGPS(const std::string & filename, const int & GPS_to_XYZ_method = 0)
{
	std::pair<bool, Vec3> val(false, Vec3::Zero());
	std::unique_ptr<Exif_IO> exifReader(new Exif_IO_EasyExif);
	if (exifReader)
	{

		if (exifReader->open(filename) && exifReader->doesHaveExifInfo())
		{

			double latitude, longitude, altitude;
			if (exifReader->GPSLatitude(&latitude) &&
				exifReader->GPSLongitude(&longitude) &&
				exifReader->GPSAltitude(&altitude))
			{

				val.first = true;
				switch (GPS_to_XYZ_method)
				{
				case 1:
					val.second = lla_to_utm(latitude, longitude, altitude);
					break;
				case 0:
				default:
					val.second = lla_to_ecef(latitude, longitude, altitude);
					break;
				}
			}
		}
	}
	return val;
}

std::pair<bool, Vec3> checkPriorWeightsString(const std::string &sWeights)
{
	std::pair<bool, Vec3> val(true, Vec3::Zero());
	std::vector<std::string> vec_str;
	stl::split(sWeights, ';', vec_str);
	if (vec_str.size() != 3)
	{
		std::cerr << "\n 缺少;" << std::endl;
		val.first = false;
	}

	for (size_t i = 0; i < vec_str.size(); ++i)
	{
		double readvalue = 0.0;
		std::stringstream ss;
		ss.str(vec_str[i]);
		if (!(ss >> readvalue)) {
			std::cerr << "\n 使用了无效的坐标系原点 " << std::endl;
			val.first = false;
		}
		val.second[i] = readvalue;
	}
	return val;
}

features::EDESCRIBER_PRESET stringToEnum(const std::string & sPreset)
{
	features::EDESCRIBER_PRESET preset;
	if (sPreset == "NORMAL")
		preset = features::NORMAL_PRESET;
	else
		if (sPreset == "HIGH")
			preset = features::HIGH_PRESET;
		else
			if (sPreset == "ULTRA")
				preset = features::ULTRA_PRESET;
			else
				preset = features::EDESCRIBER_PRESET(-1);
	return preset;
}

int LoadingImages(
	std::string InputImagePath,     //inputdir
	std::string OutputPath,   //outputdir
	std::string fileDatabase, //sensor width database
	std::string Kmatrix,//eigen matrix
	std::string sPriorWeights = "",
	double focal_pixels = -1.0,
	int i_User_camera_model = PINHOLE_CAMERA_RADIAL3,
	int i_GPS_XYZ_method = 0,
	bool b_Group_camera_model = true
)
{
	Global::processProject = LISTIMAGES;
	Global::saveProcess();
	std::pair<bool, Vec3> prior_w_info(false, Vec3(1.0, 1.0, 1.0));


	std::cout
		<< "输入图片路径:" << InputImagePath << std::endl
		<< "输出数据路径:" << OutputPath << std::endl
		<< "相机本征矩阵:" << Kmatrix << std::endl;//intrinsics

	if (Kmatrix.size() == 0)
	{
		std::cout << "未手动输入本征矩阵,程序自适应计算本征 " << Kmatrix << std::endl;
	}

	double width = -1, height = -1, focal = -1, ppx = -1, ppy = -1;
	const EINTRINSIC e_User_camera_model = EINTRINSIC(i_User_camera_model);

	if (!stlplus::folder_exists(InputImagePath))
	{
		std::cerr << "\n图片输入路径不存在 " << std::endl;
		return EXIT_FAILURE;
	}

	if (OutputPath.empty())
	{
		std::cerr << "\n无效的输出目录 " << std::endl;
		return EXIT_FAILURE;
	}

	if (!stlplus::folder_exists(OutputPath))
	{
		if (!stlplus::folder_create(OutputPath))
		{
			std::cerr << "\n无法创建输出目录，请检查权限 " << std::endl;
			return EXIT_FAILURE;
		}
	}

	if (Kmatrix.size() > 0 &&
		!checkIntrinsicStringValidity(Kmatrix, focal, ppx, ppy))
	{
		std::cerr << "\n无效的本征矩阵格式 " << std::endl;
		return EXIT_FAILURE;
	}

	if (Kmatrix.size() > 0 && focal_pixels != -1.0)
	{
		std::cerr << "\n无效的相机IntrPMT " << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<Datasheet> vec_database;
	if (!fileDatabase.empty())
	{
		if (!parseDatabase(fileDatabase, vec_database))
		{
			std::cerr
				<< "\n无效的SenWidDB: " << fileDatabase << std::endl;
			return EXIT_FAILURE;
		}
	}

	std::vector<std::string> vec_image = stlplus::folder_files(InputImagePath);
	std::sort(vec_image.begin(), vec_image.end());

	SfM_Data sfm_data;
	sfm_data.s_root_path = InputImagePath;
	Views & views = sfm_data.views;
	Intrinsics & intrinsics = sfm_data.intrinsics;

	C_Progress_display my_progress_bar(vec_image.size(),
		std::cout, "\n正在加载图片数据\n");
	std::ostringstream error_report_stream;
	for (std::vector<std::string>::const_iterator iter_image = vec_image.begin();
		iter_image != vec_image.end();
		++iter_image, ++my_progress_bar, Global::processState = my_progress_bar.pourcent(), Global::saveProcess())
	{
		width = height = ppx = ppy = focal = -1.0;

		const std::string sImageFilename = stlplus::create_filespec(InputImagePath, *iter_image);
		const std::string sImFilenamePart = stlplus::filename_part(sImageFilename);

		if (openMVG::image::GetFormat(sImageFilename.c_str()) == openMVG::image::Unknown)
		{
			error_report_stream
				<< sImFilenamePart << "ERROR 无法加载的图片格式 " << "\n";
			continue;
		}

		if (sImFilenamePart.find("mask.png") != std::string::npos
			|| sImFilenamePart.find("_mask.png") != std::string::npos)
		{
			error_report_stream
				<< sImFilenamePart << "找到MASK图片 " << "\n";
			continue;
		}

		ImageHeader imgHeader;
		if (!openMVG::image::ReadImageHeader(sImageFilename.c_str(), &imgHeader))
			continue;

		width = imgHeader.width;
		height = imgHeader.height;
		ppx = width / 2.0;
		ppy = height / 2.0;


		if (Kmatrix.size() > 0)
		{
			if (!checkIntrinsicStringValidity(Kmatrix, focal, ppx, ppy))
				focal = -1.0;
		}
		else
			if (focal_pixels != -1)
				focal = focal_pixels;

		if (focal == -1)
		{
			std::unique_ptr<Exif_IO> exifReader(new Exif_IO_EasyExif);
			exifReader->open(sImageFilename);

			const bool bHaveValidExifMetadata =
				exifReader->doesHaveExifInfo()
				&& !exifReader->getModel().empty();

			if (bHaveValidExifMetadata)
			{
				const std::string sCamModel = exifReader->getModel();

				if (exifReader->getFocal() == 0.0f)
				{
					error_report_stream
						<< stlplus::basename_part(sImageFilename) << ": 无法获取焦距." << "\n";
					focal = -1.0;
				}
				else
				{
					Datasheet datasheet;
					if (getInfo(sCamModel, vec_database, datasheet))
					{

						const double ccdw = datasheet.sensorSize_;
						focal = std::max(width, height) * exifReader->getFocal() / ccdw;
					}
					else
					{
						error_report_stream
							<< stlplus::basename_part(sImageFilename)
							<< "\n Warning:找不到相机传感器参数，请手动添加 \n";
					}
				}
			}
		}

		std::shared_ptr<IntrinsicBase> intrinsic;

		if (focal > 0 && ppx > 0 && ppy > 0 && width > 0 && height > 0)
		{

			switch (e_User_camera_model)
			{
			case PINHOLE_CAMERA:
				intrinsic = std::make_shared<Pinhole_Intrinsic>
					(width, height, focal, ppx, ppy);
				break;
			case PINHOLE_CAMERA_RADIAL1:
				intrinsic = std::make_shared<Pinhole_Intrinsic_Radial_K1>
					(width, height, focal, ppx, ppy, 0.0);
				break;
			case PINHOLE_CAMERA_RADIAL3:
				intrinsic = std::make_shared<Pinhole_Intrinsic_Radial_K3>
					(width, height, focal, ppx, ppy, 0.0, 0.0, 0.0);
				break;
			case PINHOLE_CAMERA_BROWN:
				intrinsic = std::make_shared<Pinhole_Intrinsic_Brown_T2>
					(width, height, focal, ppx, ppy, 0.0, 0.0, 0.0, 0.0, 0.0);
				break;
			case PINHOLE_CAMERA_FISHEYE:
				intrinsic = std::make_shared<Pinhole_Intrinsic_Fisheye>
					(width, height, focal, ppx, ppy, 0.0, 0.0, 0.0, 0.0);
				break;
			case CAMERA_SPHERICAL:
				intrinsic = std::make_shared<Intrinsic_Spherical>
					(width, height);
				break;
			default:
				std::cerr << "ERROR  未知的相机型号 " << (int)e_User_camera_model << std::endl;
				return EXIT_FAILURE;
			}
		}

		const std::pair<bool, Vec3> gps_info = checkGPS(sImageFilename, i_GPS_XYZ_method);
		if (gps_info.first && sPriorWeights != "")
		{
			ViewPriors v(*iter_image, views.size(), views.size(), views.size(), width, height);


			if (intrinsic == nullptr)
			{

				v.id_intrinsic = UndefinedIndexT;
			}
			else
			{

				intrinsics[v.id_intrinsic] = intrinsic;
			}

			v.b_use_pose_center_ = true;
			v.pose_center_ = gps_info.second;

			if (prior_w_info.first == true)
			{
				v.center_weight_ = prior_w_info.second;
			}


			views[v.id_view] = std::make_shared<ViewPriors>(v);
		}
		else
		{
			View v(*iter_image, views.size(), views.size(), views.size(), width, height);


			if (intrinsic == nullptr)
			{

				v.id_intrinsic = UndefinedIndexT;
			}
			else
			{

				intrinsics[v.id_intrinsic] = intrinsic;
			}


			views[v.id_view] = std::make_shared<View>(v);
		}
	}


	if (!error_report_stream.str().empty())
	{
		std::cerr
			<< "\n错误信息:" << std::endl
			<< error_report_stream.str() << std::endl;
	}


	if (b_Group_camera_model)
	{
		GroupSharedIntrinsics(sfm_data);
	}


	if (!Save(
		sfm_data,
		stlplus::create_filespec(OutputPath, "sfm_data.json").c_str(),
		ESfM_Data(VIEWS | INTRINSICS)))
	{
		return EXIT_FAILURE;
	}

	std::cout << std::endl
		<< "完成图片加载:\n"
		<< "成功加载的图片数量: " << vec_image.size() << std::endl;



	Global::saveProcess();
	return EXIT_SUCCESS;
}


int GetFeatures(
	std::string SFMDataFilename,
	std::string OutputPath,
	std::string ComputeMethod = "SIFT",
	std::string sFeaturePreset = "",
	bool bUpRight = false,
	bool bForce = false
)
{


#ifdef OPENMVG_USE_OPENMP
	int iNumThreads = 0;
#endif
	Global::processProject = COMPUTEFEATURES;
	Global::saveProcess();
	std::cout
		<< "\n" << "特征点算法:" << ComputeMethod << std::endl;


	SfM_Data sfm_data;
	if (!Load(sfm_data, SFMDataFilename, ESfM_Data(VIEWS | INTRINSICS))) {
		std::cerr << "输入图片无法读取 \"" << SFMDataFilename << "\" " << std::endl;
		return EXIT_FAILURE;
	}


	using namespace openMVG::features;
	std::unique_ptr<Image_describer> image_describer;

	const std::string sImage_describer = stlplus::create_filespec(OutputPath, "image_describer", "json");
	if (!bForce && stlplus::is_file(sImage_describer))
	{

		std::ifstream stream(sImage_describer.c_str());
		if (!stream.is_open())
			return EXIT_FAILURE;

		try
		{
			cereal::JSONInputArchive archive(stream);
			archive(cereal::make_nvp("image_describer", image_describer));
		}
		catch (const cereal::Exception & e)
		{
			std::cerr << e.what() << std::endl
				<< "动态分配describer内存空间失败 " << std::endl;
			return EXIT_FAILURE;
		}
	}
	else
	{

		if (ComputeMethod == "SIFT")
		{
			image_describer.reset(new SIFT_Image_describer
			(SIFT_Image_describer::Params(), !bUpRight));
		}
		else
			if (ComputeMethod == "SIFT_ANATOMY")
			{
				image_describer.reset(
					new SIFT_Anatomy_Image_describer(SIFT_Anatomy_Image_describer::Params()));
			}
			else
				if (ComputeMethod == "AKAZE_FLOAT")
				{
					image_describer = AKAZE_Image_describer::create
					(AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MSURF), !bUpRight);
				}
				else
					if (ComputeMethod == "AKAZE_MLDB")
					{
						image_describer = AKAZE_Image_describer::create
						(AKAZE_Image_describer::Params(AKAZE::Params(), AKAZE_MLDB), !bUpRight);
					}
		if (!image_describer)
		{
			std::cerr << "无法创建describer "
				<< ComputeMethod << "." << std::endl;
			return EXIT_FAILURE;
		}
		else
		{
			if (!sFeaturePreset.empty())
				if (!image_describer->Set_configuration_preset(stringToEnum(sFeaturePreset)))
				{
					std::cerr << "预设配置失败 " << std::endl;
					return EXIT_FAILURE;
				}
		}


		{
			std::ofstream stream(sImage_describer.c_str());
			if (!stream.is_open())
				return EXIT_FAILURE;

			cereal::JSONOutputArchive archive(stream);
			archive(cereal::make_nvp("image_describer", image_describer));
			auto regionsType = image_describer->Allocate();
			archive(cereal::make_nvp("regions_type", regionsType));
		}
	}


	{
		system::Timer timer;
		Image<unsigned char> imageGray;

		C_Progress_display my_progress_bar(sfm_data.GetViews().size(),
			std::cout, "- 正在获取特征点 -\n");

		std::atomic<bool> preemptive_exit(false);


#ifdef OPENMVG_USE_OPENMP
		const unsigned int nb_max_thread = omp_get_max_threads();

		if (iNumThreads > 0) {
			omp_set_num_threads(iNumThreads);
		}
		else {
			omp_set_num_threads(nb_max_thread);
		}

#pragma omp parallel for schedule(dynamic) if (iNumThreads > 0) private(imageGray)
#endif
		for (int i = 0; i < static_cast<int>(sfm_data.views.size()); ++i)
		{
			Views::const_iterator iterViews = sfm_data.views.begin();
			std::advance(iterViews, i);
			const View * view = iterViews->second.get();
			const std::string
				sView_filename = stlplus::create_filespec(sfm_data.s_root_path, view->s_Img_path),
				sFeat = stlplus::create_filespec(OutputPath, stlplus::basename_part(sView_filename), "feat"),
				sDesc = stlplus::create_filespec(OutputPath, stlplus::basename_part(sView_filename), "desc");


			if (!preemptive_exit && (bForce || !stlplus::file_exists(sFeat) || !stlplus::file_exists(sDesc)))
			{
				if (!ReadImage(sView_filename.c_str(), &imageGray))
					continue;

				Image<unsigned char> * mask = nullptr;

				const std::string
					mask_filename_local = stlplus::create_filespec(sfm_data.s_root_path, stlplus::basename_part(sView_filename) + "_mask", "png"),
					mask__filename_global = stlplus::create_filespec(sfm_data.s_root_path, "mask", "png");

				Image<unsigned char> imageMask;

				if (stlplus::file_exists(mask_filename_local))
				{
					if (!ReadImage(mask_filename_local.c_str(), &imageMask))
					{
						std::cerr << "错误MASK: " << mask_filename_local << std::endl
							<< "停止工作 " << std::endl;
						preemptive_exit = true;
						continue;
					}

					if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
						mask = &imageMask;
				}
				else
				{

					if (stlplus::file_exists(mask__filename_global))
					{
						if (!ReadImage(mask__filename_global.c_str(), &imageMask))
						{
							std::cerr << "错误MASK: " << mask__filename_global << std::endl
								<< "停止工作 " << std::endl;
							preemptive_exit = true;
							continue;
						}

						if (imageMask.Width() == imageGray.Width() && imageMask.Height() == imageGray.Height())
							mask = &imageMask;
					}
				}


				auto regions = image_describer->Describe(imageGray, mask);
				if (regions && !image_describer->Save(regions.get(), sFeat, sDesc)) {
					std::cerr << "无法保存图片信息: " << sView_filename << std::endl
						<< "停止工作 " << std::endl;
					preemptive_exit = true;
					continue;
				}
			}
			++my_progress_bar;
			Global::processState = my_progress_bar.pourcent();
			Global::saveProcess();
		}
		Global::saveProcess();
		std::cout << "任务完成，用时(秒): " << timer.elapsed() << std::endl;
	}
	return EXIT_SUCCESS;
}




