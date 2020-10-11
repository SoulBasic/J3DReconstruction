#include "Global.h"


int Global::processProject = 0;
int Global::processState = 0;
int Global::process = PROCESSWORKING;

bool Global::saveProcess()
{

	ofstream file;
	locale loc = locale::global(locale(""));
	file.open("C:\\ProgramData\\J3DEngine\\ProcessCache.tmp", ios::out | ios::trunc);
	locale::global(loc);
	if (!file.is_open())
		return false;
	std::string project = std::to_string(Global::processProject);
	std::string state = std::to_string(Global::processState);
	std::string process = std::to_string(Global::process);
	file << process << endl;
	file << project << endl;
	file << state;
	file.close();
	return true;
}
//bool Global::resizeTextureFile(std::string fileName)
//{
//	unsigned int size = Global::getImageSize(fileName);
//	if (size == -1) 
//	{
//		return false;
//	}
//	if (size < 41943040) 
//	{
//		return true;
//	}
//
//	while (size >= 41943040)
//	{
//		Mat srcImage = imread(fileName, IMREAD_UNCHANGED);
//		Size imgSize = srcImage.size();
//		imgSize.height *= 0.75;
//		imgSize.width *= 0.75;
//		Mat destImage = Mat(imgSize, srcImage.type());
//		//关键就是这句，要用参数CV_INTER_LINEAR
//		resize(srcImage, destImage, imgSize, CV_INTER_LINEAR);
//		if (false == imwrite(fileName, destImage)) 
//		{
//			return false;
//		}
//		size = Global::getImageSize(fileName);
//	}
//	return true;
//}
//
//unsigned int Global::getImageSize(std::string fileName) {
//	ifstream fin(fileName);
//	if (!fin.is_open())
//	{
//		return -1;
//	}
//	fin.seekg(0, ios::end);
//	unsigned int size = fin.tellg();
//	fin.close();
//	return size;
//}