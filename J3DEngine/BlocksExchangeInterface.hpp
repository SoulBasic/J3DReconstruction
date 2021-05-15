#include <iostream>
#include <string>
#include <memory>
#include <boost/typeof/typeof.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "openMVG/cameras/cameras.hpp"
#include "openMVG/sfm/sfm_data.hpp"
#include "openMVG/sfm/sfm_data_io.hpp"
#include "openMVG/geodesy/geodesy.hpp"
#include "third_party/stlplus3/filesystemSimplified/file_system.hpp"
#include <cmath>


using namespace boost::property_tree;
using namespace openMVG::cameras;
using namespace openMVG::sfm;


std::string getFileName(const std::string& path)
{
	auto p = path.rfind('/');
	if (p == path.npos)
	{
		p = path.rfind('\\');
		if (p == path.npos)return "";
	}
	return path.substr(p + 1, path.length());
}

std::string getFileDir(const std::string& path)
{
	auto p = path.rfind('/');
	if (p == path.npos)
	{
		p = path.rfind('\\');
		if (p == path.npos)return "";
	}
	return path.substr(0, p);
}


void BlocksExchange2openMVG(const std::string& fileName, const std::string& workDir)
{
	SfM_Data sfm_data;
	auto& intrinsics = sfm_data.intrinsics;
	auto& views = sfm_data.views;
	auto& poses = sfm_data.poses;
	auto& structure = sfm_data.structure;
	ptree pt;
	std::string photoName = "";
	xml_parser::read_xml(workDir + "\\" + fileName, pt);

	//auto SpatialReferenceSystems = pt.get_child("BlocksExchange.SpatialReferenceSystems.SRS");
	//for (auto pos = SpatialReferenceSystems.begin(); pos != SpatialReferenceSystems.end(); ++pos)
	//{
	//	if ("<xmlattr>" == pos->first) {}
	//	else if ("<xmlcomment>" == pos->first) {}
	//	else
	//	{
	//		std::cout << pos->first << " " << pos->second.data() << std::endl;
	//	}
	//}


	auto Photogroups = pt.get_child("BlocksExchange.Block.Photogroups.Photogroup");
	double width = -1, height = -1, sensorSize = -1, focal = -1, ppx = -1, ppy = -1, k1 = -1, k2 = -1, k3 = -1;
	for (auto pos = Photogroups.begin(); pos != Photogroups.end(); ++pos)
	{
		if ("<xmlattr>" == pos->first) {}
		else if ("<xmlcomment>" == pos->first) {}
		else
		{
			if (pos->first == "ImageDimensions")
			{
				auto ImageDimensions = pos->second.get_child("");
				auto it = ImageDimensions.begin();
				if (it->first != "Width")std::cout << "error" << std::endl;
				width = atof(it++->second.data().c_str());
				if (it->first != "Height")std::cout << "error" << std::endl;
				height = atof(it->second.data().c_str());
			}
			else if (pos->first == "SensorSize")sensorSize = atof(pos->second.data().c_str());
			else if (pos->first == "FocalLength")
			{
				focal = atof(pos->second.data().c_str());
			}
			else if (pos->first == "PrincipalPoint")
			{
				auto PrincipalPoint = pos->second.get_child("");
				auto it = PrincipalPoint.begin();
				if (it->first != "x")std::cout << "error" << std::endl;
				ppx = atof(it++->second.data().c_str());
				if (it->first != "y")std::cout << "error" << std::endl;
				ppy = atof(it->second.data().c_str());
			}
			else if (pos->first == "Distortion")
			{
				auto Distortion = pos->second.get_child("");
				auto it = Distortion.begin();
				if (it->first != "K1")std::cout << "error" << std::endl;
				k1 = atof(it++->second.data().c_str());
				if (it->first != "K2")std::cout << "error" << std::endl;
				k2 = atof(it++->second.data().c_str());
				if (it->first != "K3")std::cout << "error" << std::endl;
				k3 = atof(it->second.data().c_str());
				focal = std::max(width, height) * focal / sensorSize;
				intrinsics[0] = std::make_shared<Pinhole_Intrinsic_Radial_K3>(width, height, focal, ppx, ppy, k1, k2, k3);
			}
			else if (pos->first == "Photo")
			{
				int id = -1;
				std::string imagePath = "";
				openMVG::Vec3 center;
				openMVG::Mat3 rotation;
				auto Photo = pos->second.get_child("");
				for (auto pos1 = Photo.begin(); pos1 != Photo.end(); ++pos1)
				{
					if (pos1->first == "Id")id = atoi(pos1->second.data().c_str());
					else if (pos1->first == "ImagePath")
					{
						if (photoName == "")photoName = pos1->second.data();
						imagePath = getFileName(pos1->second.data());
					}
					else if (pos1->first == "Pose")
					{

						auto Pose = pos1->second.get_child("");
						for (auto pos2 = Pose.begin(); pos2 != Pose.end(); ++pos2)
						{
							if (pos2->first == "Rotation")
							{
								auto Rotation = pos2->second.get_child("");
								auto it = Rotation.begin();
								for (int i = 0; i < 3; i++)
									for (int j = 0; j < 3; j++)
										rotation(i, j) = atof(it++->second.data().c_str());
							}
							else if (pos2->first == "Center")
							{
								auto Center = pos2->second.get_child("");
								auto it = Center.begin();
								if (it->first != "x")std::cout << "error" << std::endl;
								center(0) = atof(it++->second.data().c_str());
								if (it->first != "y")std::cout << "error" << std::endl;
								center(1) = atof(it++->second.data().c_str());
								if (it->first != "z")std::cout << "error" << std::endl;
								center(2) = atof(it->second.data().c_str());
							}
						}
					}
					else if (pos1->first == "ExifData")
					{

					}
				}
				ViewPriors v(imagePath, id, 0, id, width, height);
				v.b_use_pose_center_ = true;
				//center = openMVG::geodesy::lla_to_ecef(center(0), center(1), center(2));
				v.SetPoseCenterPrior(center, openMVG::Vec3{ 1.0,1.0,1.0 });
				views[id] = std::make_shared<ViewPriors>(v);
				poses[id] = Pose3(rotation, center);
			}
			else
			{
				std::cout << pos->first << " " << pos->second.data() << std::endl;
			}
		}
	}


	try
	{
		auto TiePoints = pt.get_child("BlocksExchange.Block.TiePoints");
		for (auto pos = TiePoints.begin(); pos != TiePoints.end(); ++pos)
		{
			if (pos->first == "TiePoint")
			{
				Landmark l;
				auto& obs = l.obs;
				double x, y, z, r, g, b;
				auto TiePoint = pos->second.get_child("");
				for (auto pos1 = TiePoint.begin(); pos1 != TiePoint.end(); ++pos1)
				{
					if (pos1->first == "Position")
					{
						auto Position = pos1->second.get_child("");
						for (auto pos2 = Position.begin(); pos2 != Position.end(); ++pos2)
						{
							if (pos2->first == "x")x = atof(pos2->second.data().c_str());
							else if (pos2->first == "y")y = atof(pos2->second.data().c_str());
							else if (pos2->first == "z")z = atof(pos2->second.data().c_str());
						}
					}
					else if (pos1->first == "Color")
					{
						auto Color = pos1->second.get_child("");
						for (auto pos2 = Color.begin(); pos2 != Color.end(); ++pos2)
						{
							if (pos2->first == "Red")r = atof(pos2->second.data().c_str());
							else if (pos2->first == "Green")g = atof(pos2->second.data().c_str());
							else if (pos2->first == "Blue")b = atof(pos2->second.data().c_str());
						}
					}
					else if (pos1->first == "Measurement")
					{
						int pid = -1;
						double px, py;
						auto Measurement = pos1->second.get_child("");
						for (auto pos2 = Measurement.begin(); pos2 != Measurement.end(); ++pos2)
						{
							if (pos2->first == "PhotoId")pid = atoi(pos2->second.data().c_str());
							else if (pos2->first == "x")px = atof(pos2->second.data().c_str());
							else if (pos2->first == "y")py = atof(pos2->second.data().c_str());
						}
						Observation ob(openMVG::Vec2{ px,py }, openMVG::UndefinedIndexT);
						obs[pid] = ob;

					}
				}
				l.X = openMVG::Vec3(x, y, z);
				structure[structure.size()] = l;
			}
		}
	}
	catch (ptree_bad_path& bp)
	{
		printf("警告:xml中无有效连接点，后续J3D数据仅可用于坐标映射，无法用于MVS引擎的重建任务\n");
	}

	printf("一共成功导入 相机 = %d 姿态 = %d 像片 = %d 连接点 = %d\n", sfm_data.intrinsics.size(), sfm_data.poses.size(), sfm_data.views.size(), sfm_data.structure.size());
	sfm_data.s_root_path = getFileDir(photoName);
	Save(sfm_data,
		stlplus::create_filespec(workDir, "sfm_data", ".bin"),
		ESfM_Data(ALL));

	Save(sfm_data,
		stlplus::create_filespec(workDir, "cloud_and_poses", ".ply"),
		ESfM_Data(ALL));
}