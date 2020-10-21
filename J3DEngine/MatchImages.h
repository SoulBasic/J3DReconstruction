#pragma once
#include "openMVG/graph/graph.hpp"
#include "openMVG/graph/graph_stats.hpp"
#include "openMVG/features/akaze/image_describer_akaze.hpp"
#include "openMVG/features/descriptor.hpp"
#include "openMVG/features/feature.hpp"
#include "openMVG/matching/indMatch.hpp"
#include "openMVG/matching/indMatch_utils.hpp"
#include "openMVG/matching_image_collection/Matcher_Regions.hpp"
#include "openMVG/matching_image_collection/Cascade_Hashing_Matcher_Regions.hpp"
#include "openMVG/matching_image_collection/GeometricFilter.hpp"
#include "openMVG/sfm/pipelines/sfm_features_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider.hpp"
#include "openMVG/sfm/pipelines/sfm_regions_provider_cache.hpp"
#include "openMVG/matching_image_collection/F_ACRobust.hpp"
#include "openMVG/matching_image_collection/E_ACRobust.hpp"
#include "openMVG/matching_image_collection/E_ACRobust_Angular.hpp"
#include "openMVG/matching_image_collection/Eo_Robust.hpp"
#include "openMVG/matching_image_collection/H_ACRobust.hpp"
#include "openMVG/matching_image_collection/Pair_Builder.hpp"
#include "openMVG/matching/pairwiseAdjacencyDisplay.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/stl/stl.hpp"
#include "openMVG/system/timer.hpp"

#include "third_party/cmdLine/cmdLine.h"
#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <string>
#include "Global.h"
using namespace openMVG;
using namespace openMVG::matching;
using namespace openMVG::robust;
using namespace openMVG::sfm;
using namespace openMVG::matching_image_collection;
using namespace std;

enum EGeometricModel
{
	FUNDAMENTAL_MATRIX = 0,
	ESSENTIAL_MATRIX = 1,
	HOMOGRAPHY_MATRIX = 2,
	ESSENTIAL_MATRIX_ANGULAR = 3,
	ESSENTIAL_MATRIX_ORTHO = 4,
	ESSENTIAL_MATRIX_UPRIGHT = 5
};

enum EPairMode
{
	PAIR_EXHAUSTIVE = 0,
	PAIR_CONTIGUOUS = 1,
	PAIR_FROM_FILE = 2
};
int GetMatches(
	std::string SFMDataFilename,
	std::string OutputPath,
	std::string sGeometricModel = "f",
	std::string sPredefinedPairList = "",
	std::string sNearestMatchingMethod = "AUTO",
	int iMatchingVideoMode = -1,
	int imax_iteration = 2048,
	unsigned int ui_max_cache_size = 0,
	float fDistRatio = 0.8f,
	bool bForce = false,
	bool bGuided_matching = false
)
{
	Global::processProject = MATCHFEATURES;
	Global::saveProcess();
	std::cout << "开始匹配特征点 " << "\n"
		<< "输出目录 " << OutputPath << "\n" << std::endl;

	EPairMode ePairmode = (iMatchingVideoMode == -1) ? PAIR_EXHAUSTIVE : PAIR_CONTIGUOUS;

	if (sPredefinedPairList.length()) {
		ePairmode = PAIR_FROM_FILE;
		if (iMatchingVideoMode > 0) {
			std::cerr << "\n不兼容的选择" << std::endl;
			return EXIT_FAILURE;
		}
	}

	if (OutputPath.empty() || !stlplus::is_folder(OutputPath)) {
		std::cerr << "\n无效的输出目录" << std::endl;
		return EXIT_FAILURE;
	}

	EGeometricModel eGeometricModelToCompute = FUNDAMENTAL_MATRIX;
	std::string sGeometricMatchesFilename = "";
	switch (sGeometricModel[0])
	{
	case 'f': case 'F':
		eGeometricModelToCompute = FUNDAMENTAL_MATRIX;
		sGeometricMatchesFilename = "matches.f.bin";
		break;
	case 'e': case 'E':
		eGeometricModelToCompute = ESSENTIAL_MATRIX;
		sGeometricMatchesFilename = "matches.e.bin";
		break;
	case 'h': case 'H':
		eGeometricModelToCompute = HOMOGRAPHY_MATRIX;
		sGeometricMatchesFilename = "matches.h.bin";
		break;
	case 'a': case 'A':
		eGeometricModelToCompute = ESSENTIAL_MATRIX_ANGULAR;
		sGeometricMatchesFilename = "matches.f.bin";
		break;
	case 'o': case 'O':
		eGeometricModelToCompute = ESSENTIAL_MATRIX_ORTHO;
		sGeometricMatchesFilename = "matches.o.bin";
		break;
	case 'u': case 'U':
		eGeometricModelToCompute = ESSENTIAL_MATRIX_UPRIGHT;
		sGeometricMatchesFilename = "matches.f.bin";
		break;
	default:
		std::cerr << "未知几何模型" << std::endl;
		return EXIT_FAILURE;
	}

	SfM_Data sfm_data;
	if (!Load(sfm_data, SFMDataFilename, ESfM_Data(VIEWS | INTRINSICS))) {
		std::cerr << std::endl
			<< "输入的SfM_Data文件 \"" << SFMDataFilename << "\" 无法读取." << std::endl;
		return EXIT_FAILURE;
	}

	using namespace openMVG::features;
	const std::string sImage_describer = stlplus::create_filespec(OutputPath, "image_describer", "json");
	std::unique_ptr<Regions> regions_type = Init_region_type_from_file(sImage_describer);
	if (!regions_type)
	{
		std::cerr << "无效的: "
			<< sImage_describer << " regions文件." << std::endl;
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
	if (!regions_provider->load(sfm_data, OutputPath, regions_type, &progress)) {
		std::cerr << std::endl << "Invalid regions." << std::endl;
		return EXIT_FAILURE;
	}

	PairWiseMatches map_PutativesMatches;


	std::vector<std::string> vec_fileNames;
	std::vector<std::pair<size_t, size_t>> vec_imagesSize;
	{
		vec_fileNames.reserve(sfm_data.GetViews().size());
		vec_imagesSize.reserve(sfm_data.GetViews().size());
		for (Views::const_iterator iter = sfm_data.GetViews().begin();
			iter != sfm_data.GetViews().end();
			++iter)
		{
			const View * v = iter->second.get();
			vec_fileNames.push_back(stlplus::create_filespec(sfm_data.s_root_path,
				v->s_Img_path));
			vec_imagesSize.push_back(std::make_pair(v->ui_width, v->ui_height));
		}
	}

	std::cout << std::endl << " - PUTATIVE匹配中 - " << std::endl;
	if (!bForce
		&& (stlplus::file_exists(OutputPath + "/matches.putative.txt")
			|| stlplus::file_exists(OutputPath + "/matches.putative.bin"))
		)
	{
		if (!(Load(map_PutativesMatches, OutputPath + "/matches.putative.bin") ||
			Load(map_PutativesMatches, OutputPath + "/matches.putative.txt")))
		{
			std::cerr << "无法加载输入匹配文件";
			return EXIT_FAILURE;
		}
		std::cout << "\t 加载已有的结果;"
			<< " #pair: " << map_PutativesMatches.size() << std::endl;
	}
	else
	{
		std::cout << "Use: ";
		switch (ePairmode)
		{
		case PAIR_EXHAUSTIVE: std::cout << "穷举匹配" << std::endl; break;
		case PAIR_CONTIGUOUS: std::cout << "序列匹配" << std::endl; break;
		case PAIR_FROM_FILE:  std::cout << "用户自定义的匹配" << std::endl; break;
		}


		std::unique_ptr<Matcher> collectionMatcher;
		if (sNearestMatchingMethod == "AUTO")
		{
			if (regions_type->IsScalar())
			{
				std::cout << "使用FAST_CASCADE_HASHING_L2匹配器" << std::endl;
				collectionMatcher.reset(new Cascade_Hashing_Matcher_Regions(fDistRatio));
			}
			else
				if (regions_type->IsBinary())
				{
					std::cout << "使用BRUTE_FORCE_HAMMING匹配器" << std::endl;
					collectionMatcher.reset(new Matcher_Regions(fDistRatio, BRUTE_FORCE_HAMMING));
				}
		}
		else
			if (sNearestMatchingMethod == "BRUTEFORCEL2")
			{
				std::cout << "使用 BRUTE_FORCE_L2匹配器" << std::endl;
				collectionMatcher.reset(new Matcher_Regions(fDistRatio, BRUTE_FORCE_L2));
			}
			else
				if (sNearestMatchingMethod == "BRUTEFORCEHAMMING")
				{
					std::cout << "使用 BRUTE_FORCE_HAMMING 匹配器" << std::endl;
					collectionMatcher.reset(new Matcher_Regions(fDistRatio, BRUTE_FORCE_HAMMING));
				}
				else
					if (sNearestMatchingMethod == "HNSWL2")
					{
						std::cout << "使用 HNSWL2 匹配器" << std::endl;
						collectionMatcher.reset(new Matcher_Regions(fDistRatio, HNSW_L2));
					}
					else
						if (sNearestMatchingMethod == "ANNL2")
						{
							std::cout << "使用 ANN_L2 匹配器" << std::endl;
							collectionMatcher.reset(new Matcher_Regions(fDistRatio, ANN_L2));
						}
						else
							if (sNearestMatchingMethod == "CASCADEHASHINGL2")
							{
								std::cout << "使用 CASCADE_HASHING_L2 匹配器" << std::endl;
								collectionMatcher.reset(new Matcher_Regions(fDistRatio, CASCADE_HASHING_L2));
							}
							else
								if (sNearestMatchingMethod == "FASTCASCADEHASHINGL2")
								{
									std::cout << "使用 FAST_CASCADE_HASHING_L2 匹配器" << std::endl;
									collectionMatcher.reset(new Cascade_Hashing_Matcher_Regions(fDistRatio));
								}
		if (!collectionMatcher)
		{
			std::cerr << "无效的匹配法: " << sNearestMatchingMethod << std::endl;
			return EXIT_FAILURE;
		}
		system::Timer timer;
		{

			Pair_Set pairs;
			switch (ePairmode)
			{
			case PAIR_EXHAUSTIVE: pairs = exhaustivePairs(sfm_data.GetViews().size()); break;
			case PAIR_CONTIGUOUS: pairs = contiguousWithOverlap(sfm_data.GetViews().size(), iMatchingVideoMode); break;
			case PAIR_FROM_FILE:
				if (!loadPairs(sfm_data.GetViews().size(), sPredefinedPairList, pairs))
				{
					return EXIT_FAILURE;
				}
				break;
			}

			collectionMatcher->Match(regions_provider, pairs, map_PutativesMatches, &progress);

			//EXPORT
			if (!Save(map_PutativesMatches, std::string(OutputPath + "/matches.putative.bin")))
			{
				std::cerr
					<< "无法保存计算的匹配数据: "
					<< std::string(OutputPath + "/matches.putative.bin");
				return EXIT_FAILURE;
			}
		}
		std::cout << "Regions匹配完成，用时(秒): " << timer.elapsed() << std::endl;
	}

	PairWiseMatchingToAdjacencyMatrixSVG(vec_fileNames.size(),
		map_PutativesMatches,
		stlplus::create_filespec(OutputPath, "PutativeAdjacencyMatrix", "svg"));

	{
		std::set<IndexT> set_ViewIds;
		std::transform(sfm_data.GetViews().begin(), sfm_data.GetViews().end(),
			std::inserter(set_ViewIds, set_ViewIds.begin()), stl::RetrieveKey());
		graph::indexedGraph putativeGraph(set_ViewIds, getPairs(map_PutativesMatches));
		graph::exportToGraphvizData(stlplus::create_filespec(OutputPath, "putative_matches"), putativeGraph);
	}


	std::unique_ptr<ImageCollectionGeometricFilter> filter_ptr(
		new ImageCollectionGeometricFilter(&sfm_data, regions_provider));

	if (filter_ptr)
	{
		system::Timer timer;
		const double d_distance_ratio = 0.6;

		PairWiseMatches map_GeometricMatches;
		switch (eGeometricModelToCompute)
		{
		case HOMOGRAPHY_MATRIX:
		{
			const bool bGeometric_only_guided_matching = true;
			filter_ptr->Robust_model_estimation(
				GeometricFilter_HMatrix_AC(4.0, imax_iteration),
				map_PutativesMatches, bGuided_matching,
				bGeometric_only_guided_matching ? -1.0 : d_distance_ratio, &progress);
			map_GeometricMatches = filter_ptr->Get_geometric_matches();
		}
		break;
		case FUNDAMENTAL_MATRIX:
		{
			filter_ptr->Robust_model_estimation(
				GeometricFilter_FMatrix_AC(4.0, imax_iteration),
				map_PutativesMatches, bGuided_matching, d_distance_ratio, &progress);
			map_GeometricMatches = filter_ptr->Get_geometric_matches();
		}
		break;
		case ESSENTIAL_MATRIX:
		{
			filter_ptr->Robust_model_estimation(
				GeometricFilter_EMatrix_AC(4.0, imax_iteration),
				map_PutativesMatches, bGuided_matching, d_distance_ratio, &progress);
			map_GeometricMatches = filter_ptr->Get_geometric_matches();

			std::vector<PairWiseMatches::key_type> vec_toRemove;
			for (const auto & pairwisematches_it : map_GeometricMatches)
			{
				const size_t putativePhotometricCount = map_PutativesMatches.find(pairwisematches_it.first)->second.size();
				const size_t putativeGeometricCount = pairwisematches_it.second.size();
				const float ratio = putativeGeometricCount / static_cast<float>(putativePhotometricCount);
				if (putativeGeometricCount < 50 || ratio < .3f) {
					vec_toRemove.push_back(pairwisematches_it.first);
				}
			}
			for (const auto & pair_to_remove_it : vec_toRemove)
			{
				map_GeometricMatches.erase(pair_to_remove_it);
			}
		}
		break;
		case ESSENTIAL_MATRIX_ANGULAR:
		{
			filter_ptr->Robust_model_estimation(
				GeometricFilter_ESphericalMatrix_AC_Angular<false>(4.0, imax_iteration),
				map_PutativesMatches, bGuided_matching, d_distance_ratio, &progress);
			map_GeometricMatches = filter_ptr->Get_geometric_matches();
		}
		break;
		case ESSENTIAL_MATRIX_ORTHO:
		{
			filter_ptr->Robust_model_estimation(
				GeometricFilter_EOMatrix_RA(2.0, imax_iteration),
				map_PutativesMatches, bGuided_matching, d_distance_ratio, &progress);
			map_GeometricMatches = filter_ptr->Get_geometric_matches();
		}
		break;
		case ESSENTIAL_MATRIX_UPRIGHT:
		{
			filter_ptr->Robust_model_estimation(
				GeometricFilter_ESphericalMatrix_AC_Angular<true>(4.0, imax_iteration),
				map_PutativesMatches, bGuided_matching, d_distance_ratio, &progress);
			map_GeometricMatches = filter_ptr->Get_geometric_matches();
		}
		break;
		}

		if (!Save(map_GeometricMatches,
			std::string(OutputPath + "/" + sGeometricMatchesFilename)))
		{
			std::cerr
				<< "无法保存计算的匹配: "
				<< std::string(OutputPath + "/" + sGeometricMatchesFilename);
			return EXIT_FAILURE;
		}

		std::cout << "计算完成，用时(秒): " << timer.elapsed() << std::endl;

		graph::getGraphStatistics(sfm_data.GetViews().size(), getPairs(map_GeometricMatches));


		std::cout << "\n 输出几何匹配的邻接矩阵文件"
			<< std::endl;
		PairWiseMatchingToAdjacencyMatrixSVG(vec_fileNames.size(),
			map_GeometricMatches,
			stlplus::create_filespec(OutputPath, "GeometricAdjacencyMatrix", "svg"));


		std::set<IndexT> set_ViewIds;
		std::transform(sfm_data.GetViews().begin(), sfm_data.GetViews().end(),
			std::inserter(set_ViewIds, set_ViewIds.begin()), stl::RetrieveKey());
		graph::indexedGraph putativeGraph(set_ViewIds, getPairs(map_GeometricMatches));
		graph::exportToGraphvizData(stlplus::create_filespec(OutputPath, "geometric_matches"), putativeGraph);
	}



	Global::processState = 100;
	Global::saveProcess();
	return EXIT_SUCCESS;
}

