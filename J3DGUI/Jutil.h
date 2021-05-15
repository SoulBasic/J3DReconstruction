#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <chrono>
#include <bitset>
#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#ifndef NOWTIME_SEC
#define NOWTIME_SEC std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
#endif
#ifndef NOWTIME_MILLI
#define NOWTIME_MILLI std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
#endif
#ifndef NOWTIME_MICRO
#define NOWTIME_MICRO std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
#endif
#ifndef NOWTIME_NANO
#define NOWTIME_NANO std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
#endif


namespace Jutil
{
	//高精度计时器 
	class Clock
	{
	public:
		Clock();
		~Clock() = default;
		void reset();
		double getElapsedTimeInSec();
		double getElapsedTimeInMilliSec();
		long long getElapsedTimeInMircoSec();
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> _timePoint;
	};




	//解析文件名的各成分
	class SparseFileName
	{
	public:
		SparseFileName();
		SparseFileName(const std::string& fullFileName);
		inline std::string getDir() { return dir; }
		inline std::string getFrontName() { return frontName; }
		inline std::string getExtName() { return extName; }
	private:
		std::string dir;
		std::string frontName;
		std::string extName;
	};


	//按字节二进制打印文件
	class BinaryPrintFile
	{
	public:
		BinaryPrintFile(std::string fileName);
		~BinaryPrintFile()
		{
			this->is->close();
		}
		bool init();
		std::string getByte();
		std::vector<std::string> getBytes(int num);
	private:
		std::string fileName;
		std::ifstream is1;
		std::ifstream* is;
		std::string buffer;
		void sparseByte(int buf);

	};

	void binaryPrint(int x);
	void binaryPrint(unsigned int x);
	void binaryPrint(char x);
	void binaryPrint(unsigned char x);
	void binaryPrint(float x);
	void binaryPrint(double x);
	void binaryPrint(std::string str);

	//分割字符串
	std::vector<std::string> split(std::string str, std::string pattern);

	//unordered_map value找key
	template<typename keyT, typename valueT>
	bool getKey(std::unordered_map<keyT, valueT>& map, valueT val, keyT& key)
	{
		for (auto& i : map)
		{
			if (i.second == val)
			{
				key = i.first;
				return true;
			}
		}
		return false;
	}


	//获取文件拓展名，返回值不带前面的.
	std::string getExtentionName(const std::string& str);
	//获取不带拓展名的文件名
	std::string getFrontName(const std::string& str);

	//获取中间文本
	std::string getMiddleText(const std::string& str, const std::string& front, const std::string& behind);

#ifdef _WIN32
	//ANSI字符串转换为unicode字符串
	wchar_t * charTowchar(const char* cchar);

	//unicode字符串转换为ANSI字符串
	char * wcharTochar(const wchar_t* wchar);
#endif // _WIN32




	//返回文件目录，返回值结尾不带/
	std::string getFileDir(const std::string& fileName);

	//输入文件全路径，返回文件名
	std::string getFileName(const std::string& fullName);










}