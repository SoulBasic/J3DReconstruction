#pragma once
#include "openMVG/cameras/Camera_Common.hpp"
#include "openMVG/cameras/Cameras_Common_command_line_helper.hpp"
#include "openMVG/sfm/pipelines/sequential/sequential_SfM.hpp"
#include "openMVG/sfm/pipelines/sfm_features_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_matches_provider.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/sfm/sfm_report.hpp"
#include "openMVG/sfm/sfm_view.hpp"
#include "openMVG/sfm/sfm_data_colorization.hpp"
#include "openMVG/system/timer.hpp"
#include "openMVG/types.hpp"

#include "software/SfM/SfMPlyHelper.hpp"

#include "third_party/cmdLine/cmdLine.h"
#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <cstdlib>
#include <memory>
#include <string>
#include <utility>


using namespace openMVG;
using namespace openMVG::sfm;
using namespace openMVG::cameras;



bool computeIndexFromImageNames(
	const SfM_Data & sfm_data,
	const std::pair<std::string, std::string>& initialPairName,
	Pair& initialPairIndex)
{
	if (initialPairName.first == initialPairName.second)
	{
		std::cerr << "\n无效的图像名称" << std::endl;
		return false;
	}

	initialPairIndex = { UndefinedIndexT, UndefinedIndexT };

	for (Views::const_iterator it = sfm_data.GetViews().begin();
		it != sfm_data.GetViews().end(); ++it)
	{
		const View * v = it->second.get();
		const std::string filename = stlplus::filename_part(v->s_Img_path);
		if (filename == initialPairName.first)
		{
			initialPairIndex.first = v->id_view;
		}
		else {
			if (filename == initialPairName.second)
			{
				initialPairIndex.second = v->id_view;
			}
		}
	}
	return (initialPairIndex.first != UndefinedIndexT &&
		initialPairIndex.second != UndefinedIndexT);
}



void GetCameraPositions(const SfM_Data & sfm_data, std::vector<Vec3> & vec_camPosition)
{
	for (const auto & view : sfm_data.GetViews())
	{
		if (sfm_data.IsPoseAndIntrinsicDefined(view.second.get()))
		{
			const geometry::Pose3 pose = sfm_data.GetPoseOrDie(view.second.get());
			vec_camPosition.push_back(pose.center());
		}
	}
}



int StructureFromMotion(
	std::string SFMDataFilename,
	std::string MatchesDataPath,
	std::string sfmOutputName,
	std::string OutputDataPath = "",
	std::string initialPairString_first = "",
	std::string initialPairString_second = "",
	std::string sIntrinsic_refinement_options = "ADJUST_ALL",
	int i_User_camera_model = PINHOLE_CAMERA_RADIAL3,
	bool b_use_motion_priors = false,
	bool prior_usage = false,
	int triangulation_method = static_cast<int>(ETriangulationMethod::DEFAULT),
	int resection_method = static_cast<int>(resection::SolverType::DEFAULT)
)
{
	Global::processProject = SFM;
	Global::saveProcess();
	std::cout << "进行增量SFM" << std::endl;


	std::pair<std::string, std::string> initialPairString(initialPairString_first, initialPairString_second);

	

	if (!isValid(static_cast<ETriangulationMethod>(triangulation_method))) {
		std::cerr << "\n 无效的空中三角测量方法" << std::endl;
		return EXIT_FAILURE;
	}

	if (!isValid(openMVG::cameras::EINTRINSIC(i_User_camera_model))) {
		std::cerr << "\n 无效的相机参数" << std::endl;
		return EXIT_FAILURE;
	}

	const cameras::Intrinsic_Parameter_Type intrinsic_refinement_options =
		cameras::StringTo_Intrinsic_Parameter_Type(sIntrinsic_refinement_options);
	if (intrinsic_refinement_options == static_cast<cameras::Intrinsic_Parameter_Type>(0))
	{
		std::cerr << "相机参数优化错误" << std::endl;
		return EXIT_FAILURE;
	}


	SfM_Data sfm_data;
	if (!Load(sfm_data, SFMDataFilename, ESfM_Data(VIEWS | INTRINSICS))) {
		std::cerr << std::endl
			<< "SFMData" << SFMDataFilename << "无法读取" << std::endl;
		return EXIT_FAILURE;
	}

	using namespace openMVG::features;
	const std::string sImage_describer = stlplus::create_filespec(MatchesDataPath, "image_describer", "json");
	std::unique_ptr<Regions> regions_type = Init_region_type_from_file(sImage_describer);
	if (!regions_type)
	{
		std::cerr << "无效的: "
			<< sImage_describer << "文件" << std::endl;
		return EXIT_FAILURE;
	}

	std::shared_ptr<Features_Provider> feats_provider = std::make_shared<Features_Provider>();
	if (!feats_provider->load(sfm_data, MatchesDataPath, regions_type)) {
		std::cerr << std::endl
			<< "无效的特征数据" << std::endl;
		return EXIT_FAILURE;
	}

	std::shared_ptr<Matches_Provider> matches_provider = std::make_shared<Matches_Provider>();
	if 
		(
			!(matches_provider->load(sfm_data, sfmOutputName) ||
				matches_provider->load(sfm_data, stlplus::create_filespec(MatchesDataPath, "matches.f.txt")) ||
				matches_provider->load(sfm_data, stlplus::create_filespec(MatchesDataPath, "matches.f.bin")))
			)
	{
		std::cerr << std::endl
			<< "无效的匹配数据" << std::endl;
		return EXIT_FAILURE;
	}

	if (OutputDataPath.empty()) {
		std::cerr << "\n无效的输出路径" << std::endl;
		return EXIT_FAILURE;
	}

	if (!stlplus::folder_exists(OutputDataPath))
	{
		if (!stlplus::folder_create(OutputDataPath))
		{
			std::cerr << "\n无法创建输出目录" << std::endl;
		}
	}

	openMVG::system::Timer timer;
	Global::processState = 0;
	SequentialSfMReconstructionEngine sfmEngine(
		sfm_data,
		OutputDataPath,
		stlplus::create_filespec(OutputDataPath, "Rep.tml"));

	sfmEngine.SetFeaturesProvider(feats_provider.get());
	sfmEngine.SetMatchesProvider(matches_provider.get());

	sfmEngine.Set_Intrinsics_Refinement_Type(intrinsic_refinement_options);
	sfmEngine.SetUnknownCameraType(EINTRINSIC(i_User_camera_model));
	b_use_motion_priors = prior_usage;
	sfmEngine.Set_Use_Motion_Prior(b_use_motion_priors);
	sfmEngine.SetTriangulationMethod(static_cast<ETriangulationMethod>(triangulation_method));
	sfmEngine.SetResectionMethod(static_cast<resection::SolverType>(resection_method));

	if (!initialPairString.first.empty() && !initialPairString.second.empty())
	{
		Pair initialPairIndex;
		if (!computeIndexFromImageNames(sfm_data, initialPairString, initialPairIndex))
		{
			std::cerr << "无法进行匹配初始化 <" << initialPairString.first
				<< ", " << initialPairString.second << ">!\n";
			return EXIT_FAILURE;
		}
		sfmEngine.setInitialPair(initialPairIndex);
	}

	if (sfmEngine.Process())
	{
		std::cout << std::endl << "SFM总耗时(秒): " << timer.elapsed() << std::endl;

		//std::cout << "正在生成结果输出文件" << std::endl;
		//Generate_SfM_Report(sfmEngine.Get_SfM_Data(),
		//	stlplus::create_filespec(sfmOutputDir, "SfMReconstruction_Report.html"));

		std::cout << "输出结果数据到目录" << std::endl;
		Save(sfmEngine.Get_SfM_Data(),
			stlplus::create_filespec(OutputDataPath, "sfm_data", ".bin"),
			ESfM_Data(ALL));

		Save(sfmEngine.Get_SfM_Data(),
			stlplus::create_filespec(OutputDataPath, "cloud_and_poses", ".ply"),
			ESfM_Data(ALL));
		Global::processState = 100;
		Global::saveProcess();
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}



int PrintPointColors(
	std::string InputFile,  
	std::string OutputFile   
)
{
	Global::processProject = COLORED;
	Global::processState = 0;
	Global::saveProcess();
	if (OutputFile.empty())
	{
		std::cerr << std::endl
			<< "没有指定输出文件名." << std::endl;
		return EXIT_FAILURE;
	}

	SfM_Data sfm_data;
	if (!Load(sfm_data, InputFile, ESfM_Data(ALL)))
	{
		std::cerr << std::endl
			<< "输入的SFMData文件 \"" << InputFile << "\" 无法读取" << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<Vec3> vec_3dPoints, vec_tracksColor, vec_camPosition;
	if (ColorizeTracks(sfm_data, vec_3dPoints, vec_tracksColor))
	{
		GetCameraPositions(sfm_data, vec_camPosition);

		if (plyHelper::exportToPly(vec_3dPoints, vec_camPosition, OutputFile, &vec_tracksColor))
		{
			Global::processState = 100;
			Global::saveProcess();
			return EXIT_SUCCESS;
		}
	}

	return EXIT_FAILURE;
}
