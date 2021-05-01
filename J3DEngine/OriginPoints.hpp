#pragma once
#include "openMVG/geometry/Similarity3.hpp"
#include "openMVG/sfm/sfm.hpp"
#include "openMVG/stl/stl.hpp"
#include "openMVG/stl/split.hpp"

#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"

#include "openMVG/geometry/pose3.hpp"
#include "openMVG/sfm/sfm_landmark.hpp"

#include <iomanip>
#include <string>
#include <vector>

using namespace openMVG;
using namespace openMVG::sfm;
using namespace openMVG::geometry;

int ConvertCoorsToOrigin
(
	std::string sSfM_Data_Filename_In,
	std::string sOutDir
)
{
	Vec3 local_Frame_Origin;
	bool b_first_frame_origin;

	SfM_Data sfm_data;
	if (!Load(sfm_data, sSfM_Data_Filename_In, ESfM_Data(ALL)))
	{
		std::cerr << std::endl
			<< "未能找到SFM数据文件 \"" << sSfM_Data_Filename_In << "\"" << std::endl;
		return EXIT_FAILURE;
	}

	if (sfm_data.poses.empty()) {
		std::cerr << "该重建场景没有任何相机姿态数据！" << std::endl;
		return EXIT_FAILURE;
	}
	local_Frame_Origin = (sfm_data.poses.cbegin()->second).center();

	std::cout << "Using frame origin: " << local_Frame_Origin.transpose() << std::endl;

	// Define the transformation (Will substract the local_Frame_Origin):
	Similarity3 sim(Pose3(Mat3::Identity(), local_Frame_Origin), 1.0);
	// Apply the transformation to the sfm_data scene
	const bool b_transform_priors = true;
	ApplySimilarity(sim, sfm_data, b_transform_priors);

	// Save changed sfm data
	  //-- Export to disk computed scene (data & visualizable results)
	if (!Save(sfm_data,
		stlplus::create_filespec(sOutDir, "sfm_data_local", ".bin"),
		ESfM_Data(ALL))
		|| !Save(sfm_data,
			stlplus::create_filespec(sOutDir, "cloud_and_poses_local", ".ply"),
			ESfM_Data(ALL)))
	{
		std::cerr << "无法保存精细化场景，请检查权限" << std::endl;
	}

	std::ofstream file_LocalFrameOrigin(stlplus::create_filespec(sOutDir, "local_frame_origin", ".txt"));
	file_LocalFrameOrigin << std::setprecision(8) << std::fixed;
	file_LocalFrameOrigin << local_Frame_Origin << "\n";
	file_LocalFrameOrigin.close();

	return EXIT_SUCCESS;
}
