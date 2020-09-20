#pragma once

#include "openMVG/cameras/Camera_Common.hpp"
#include "openMVG/features/feature.hpp"
#include "openMVG/features/svg_features.hpp"
#include "openMVG/geometry/frustum.hpp"
#include "openMVG/matching/indMatch.hpp"
#include "openMVG/matching/indMatch_utils.hpp"
#include "openMVG/matching_image_collection/Pair_Builder.hpp"
#include "openMVG/sfm/pipelines/structure_from_known_poses/structure_estimator.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider_cache.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_BA.hpp"
#include "openMVG/sfm/sfm_data_BA_ceres.hpp"
#include "openMVG/sfm/sfm_data_filters.hpp"
#include "openMVG/sfm/sfm_data_filters_frustum.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/sfm/sfm_report.hpp"
#include "openMVG/sfm/sfm_data_triangulation.hpp"
#include "openMVG/tracks/tracks.hpp"
#include "openMVG/system/timer.hpp"
#include "openMVG/types.hpp"

#include "third_party/cmdLine/cmdLine.h"
#include "third_party/progress/progress_display.hpp"

#include <iostream>
#include <memory>
#include <string>

using namespace openMVG;
using namespace openMVG::matching;
using namespace openMVG::sfm;
using namespace std;

Pair_Set BuildPairsFromFrustumsIntersections(
	const SfM_Data & sfm_data,
	const double z_near = -1., // default near plane
	const double z_far = -1.)  // default far plane
{
	const Frustum_Filter frustum_filter(sfm_data, z_near, z_far);
	return frustum_filter.getFrustumIntersectionPairs();
}


int StructureFromPoses(
	std::string sSfM_Data_Filename,
	std::string sMatchesDir,
	std::string sOutFile,
	std::string sMatchFile,
	std::string sPairFile = "",
	double dMax_reprojection_error = 4.0,
	unsigned int ui_max_cache_size = 0,
	int triangulation_method = static_cast<int>(ETriangulationMethod::DEFAULT),
	bool bundle_adjustment = false,
	bool direct_triangulation = false
)
{
	Global::processProject = SFP;
	Global::saveProcess();
	std::cout << "计算SFP" << std::endl;


	if (!isValid(static_cast<ETriangulationMethod>(triangulation_method))) {
		std::cerr << "\n 无效的空三方法" << std::endl;
		return EXIT_FAILURE;
	}


	SfM_Data sfm_data;
	if (!Load(sfm_data, sSfM_Data_Filename, ESfM_Data(VIEWS | INTRINSICS | EXTRINSICS))) {
		std::cerr << std::endl
			<< "输入的SfM_Data文件 \"" << sSfM_Data_Filename << "\" 无法读取." << std::endl;
		return EXIT_FAILURE;
	}


	using namespace openMVG::features;
	const std::string sImage_describer = stlplus::create_filespec(sMatchesDir, "image_describer", "json");
	std::unique_ptr<Regions> regions_type = Init_region_type_from_file(sImage_describer);
	if (!regions_type)
	{
		std::cerr << "无效的: "
			<< sImage_describer << " regions 文件." << std::endl;
		return EXIT_FAILURE;
	}


	std::shared_ptr<Regions_Provider> regions_provider;
	if (ui_max_cache_size == 0)
	{

		regions_provider = std::make_shared<Regions_Provider>();
	}
	else
	{

		regions_provider = std::make_shared<Regions_Provider_Cache>(ui_max_cache_size);
	}

	C_Progress_display progress;
	Global::processState = 0;
	Global::saveProcess();
	if (!regions_provider->load(sfm_data, sMatchesDir, regions_type, &progress)) {
		std::cerr << std::endl
			<< "无效的regions." << std::endl;
		return EXIT_FAILURE;
	}

	/*std::cout
		<< "读取sfm_data:\n"
		<< " #views: " << sfm_data.GetViews().size() << "\n"
		<< " #poses: " << sfm_data.GetPoses().size() << "\n"
		<< " #intrinsics: " << sfm_data.GetIntrinsics().size() << "\n"
		<< " #tracks: " << sfm_data.GetLandmarks().size()
		<< std::endl;*/

	const bool bDirect_triangulation = direct_triangulation;

	if (bDirect_triangulation)
	{

		if (sMatchFile.empty() || !sPairFile.empty())
		{
			std::cerr << "匹配文件无法读取" << std::endl;
			return EXIT_FAILURE;
		}

		PairWiseMatches matches;
		if (!Load(matches, sMatchFile))
		{
			std::cerr << "匹配文件无法读取" << std::endl;
			return EXIT_FAILURE;
		}

		const int min_track_length = 2;
		openMVG::tracks::STLMAPTracks tracks;
		{

			std::cout << "\n" << "建立跟踪函数" << std::endl;
			tracks::TracksBuilder tracks_builder;
			tracks_builder.Build(matches);
			std::cout << "过滤函数" << std::endl;
			tracks_builder.Filter(min_track_length);

			tracks_builder.ExportToSTL(tracks);


			{
				std::ostringstream track_stream;

				std::set<uint32_t> images_id;
				tracks::TracksUtilsMap::ImageIdInTracks(tracks, images_id);
				track_stream
					<< "跟踪特征数: " << tracks_builder.NbTracks() << "\n";
				std::copy(images_id.begin(), images_id.end(),
					std::ostream_iterator<uint32_t>(track_stream, ", "));

				std::map<uint32_t, uint32_t> track_length_histogram;
				tracks::TracksUtilsMap::TracksLength(tracks, track_length_histogram);
				for (const auto & it : track_length_histogram) {
					track_stream << "\t" << it.first << "\t" << it.second << "\n";
				}
				track_stream << "\n";
				std::cout << track_stream.str();
			}
		}

		Landmarks & structure = sfm_data.structure;
		IndexT idx(0);
		for (const auto & tracks_it : tracks)
		{
			structure[idx] = {};
			Observations & obs = structure.at(idx).obs;
			for (const auto & track_it : tracks_it.second)
			{
				const auto imaIndex = track_it.first;
				const auto featIndex = track_it.second;
				const Vec2 & pt = regions_provider->get(imaIndex)->GetRegionPosition(featIndex);
				obs[imaIndex] = { pt, featIndex };
			}
			++idx;
		}

		{
			openMVG::system::Timer timer;
			const double max_reprojection_error = 4.0;
			bool console_verbose = true;
			SfM_Data_Structure_Computation_Robust structure_estimator(
				max_reprojection_error,
				min_track_length,
				min_track_length,
				static_cast<ETriangulationMethod>(triangulation_method),
				console_verbose);
			structure_estimator.triangulate(sfm_data);
			std::cout << "\n@空中三角测量用时: " << timer.elapsedMs() << std::endl;
		}
	}
	else
	{

		Pair_Set pairs;
		if (sMatchFile.empty() && sPairFile.empty())
		{
			pairs = BuildPairsFromFrustumsIntersections(sfm_data);
		}
		else
		{
			if (!sPairFile.empty() && sMatchFile.empty())
			{
				if (!loadPairs(sfm_data.GetViews().size(), sPairFile, pairs))
				{
					std::cerr << "无法读取匹配文件" << std::endl;
					return EXIT_FAILURE;
				}
			}
			else if (!sMatchFile.empty() && sPairFile.empty())
			{
				PairWiseMatches matches;
				if (!Load(matches, sMatchFile))
				{
					std::cerr << "无法读取匹配文件" << std::endl;
					return EXIT_FAILURE;
				}
				pairs = getPairs(matches);

				const std::set<IndexT> valid_viewIdx = Get_Valid_Views(sfm_data);
				pairs = Pair_filter(pairs, valid_viewIdx);
			}
			else
			{
				std::cerr << "无法读取匹配文件" << std::endl;
			}
		}

		openMVG::system::Timer timer;

		SfM_Data_Structure_Estimation_From_Known_Poses structure_estimator(dMax_reprojection_error);
		structure_estimator.run(sfm_data, pairs, regions_provider,
			static_cast<ETriangulationMethod>(triangulation_method));
		std::cout << "\n重建预估时间(s): " << timer.elapsed() << "." << std::endl;

	}
	regions_provider.reset(); 
	RemoveOutliers_AngleError(sfm_data, 2.0);

	std::cout
		<< "\n#找到landmark: " << sfm_data.GetLandmarks().size() << std::endl;

	std::cout << "输出报告" << std::endl;
	Generate_SfM_Report(sfm_data,
		stlplus::create_filespec(stlplus::folder_part(sOutFile), "SfMRep.tml"));

	if (bundle_adjustment)
	{

		const IndexT minPointPerPose = 12; // 6 min
		const IndexT minTrackLength = 3; // 2 min
		if (eraseUnstablePosesAndObservations(sfm_data, minPointPerPose, minTrackLength))
		{
			KeepLargestViewCCTracks(sfm_data);
			eraseUnstablePosesAndObservations(sfm_data, minPointPerPose, minTrackLength);

			const size_t pointcount_cleaning = sfm_data.structure.size();
			std::cout << "点云清理:\n"
				<< "\t #3DPoints: " << pointcount_cleaning << "\n";
		}

		std::cout << "Bundle调整..." << std::endl;
		Bundle_Adjustment_Ceres bundle_adjustment_obj;
		bundle_adjustment_obj.Adjust
		(
			sfm_data,
			Optimize_Options(
				cameras::Intrinsic_Parameter_Type::ADJUST_ALL,
				Extrinsic_Parameter_Type::ADJUST_ALL,
				Structure_Parameter_Type::ADJUST_ALL)
		);
	}

	//std::cout
	//	<< "找到sfm_data scene:\n"
	//	<< " #views: " << sfm_data.GetViews().size() << "\n"
	//	<< " #poses: " << sfm_data.GetPoses().size() << "\n"
	//	<< " #intrinsics: " << sfm_data.GetIntrinsics().size() << "\n"
	//	<< " #tracks: " << sfm_data.GetLandmarks().size()
	//	<< std::endl;

	if (stlplus::extension_part(sOutFile) != "ply") {
		Save(sfm_data,
			stlplus::create_filespec(
				stlplus::folder_part(sOutFile),
				stlplus::basename_part(sOutFile), "ply"),
			ESfM_Data(ALL));
	}

	if (Save(sfm_data, sOutFile, ESfM_Data(ALL)))
	{
		Global::processState = 100;
		Global::saveProcess();
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
