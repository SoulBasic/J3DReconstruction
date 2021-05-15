// Jutil.cpp : 定义静态库的函数。
//
#include "Jutil.h"

namespace Jutil
{
	SparseFileName::SparseFileName(const std::string& fullFileName)
	{
		this->extName = fullFileName.substr(fullFileName.find_last_of('.') + 1);

		int i = 0;
		for (i = fullFileName.length() - 1; i >= 0; i--)
		{
			if ('/' == fullFileName[i] || '\\' == fullFileName[i])
			{
				break;
			}
		}
		this->dir = fullFileName.substr(0, i);

		this->frontName = fullFileName.substr(i + 1, fullFileName.find_last_of('.') - i - 1);

	}

	SparseFileName::SparseFileName()
	{
		dir = "";
		extName = "";
		frontName = "";
	}



	BinaryPrintFile::BinaryPrintFile(std::string fileName)
	{
		this->is = new std::ifstream();
		this->fileName = fileName;
	}

	bool BinaryPrintFile::init()
	{
		is->open(fileName, std::ios::in | std::ios::binary);
		if (!is->is_open())
		{
			return false;
		}
		return true;
	}

	void BinaryPrintFile::sparseByte(int buf)
	{
		if (buf) sparseByte(buf / 2);
		else return;
		buffer += std::to_string(buf % 2);
	}

	std::string BinaryPrintFile::getByte()
	{
		int buf = 0;
		is->read(reinterpret_cast<char*>(&buf), 1);
		buffer.clear();
		sparseByte(buf);
		if (buffer.length() < 8)
		{
			while (8 != buffer.length())
			{
				buffer.insert(buffer.begin(), '0');
			}
		}
		return buffer;
	}

	std::vector<std::string> BinaryPrintFile::getBytes(int num)
	{
		std::vector<std::string> temp;
		for (int i = 0; i < num; i++)
		{
			temp.push_back(getByte());
		}
		return temp;
	}

	Clock::Clock() { reset(); }

	void Clock::reset() { _timePoint = std::chrono::high_resolution_clock::now(); }

	double Clock::getElapsedTimeInSec()
	{
		return this->getElapsedTimeInMircoSec() * 0.000001;
	}

	double Clock::getElapsedTimeInMilliSec()
	{
		return this->getElapsedTimeInMircoSec() * 0.001;
	}

	long long Clock::getElapsedTimeInMircoSec()
	{
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - _timePoint).count();
	}

	void binaryPrint(int x)
	{
		std::bitset<32> bs(x);
		std::cout << bs << std::endl;
	}
	void binaryPrint(unsigned int x)
	{
		std::bitset<32> bs(x);
		std::cout << bs << std::endl;
	}
	void binaryPrint(char x)
	{
		std::bitset<8> bs(x);
		std::cout << bs << std::endl;
	}
	void binaryPrint(unsigned char x)
	{
		std::bitset<8> bs(x);
		std::cout << bs << std::endl;
	}
	void binaryPrint(float x)
	{
		int t = x;
		std::bitset<32> bs(t);
		std::cout << bs << std::endl;
	}
	void binaryPrint(double x)
	{
		std::bitset<64> bs(x);
		std::cout << bs << std::endl;
	}
	void binaryPrint(std::string str)
	{
		int len = str.length();
		auto cstr = str.c_str();
		for (int i = 0; i <= len; i++, cstr++)
		{
			std::cout << std::bitset<8>(*cstr) << " ";
		}
		std::cout << std::endl;
	}

	std::vector<std::string> split(std::string str, std::string pattern)
	{
		std::string::size_type pos;
		std::vector<std::string> result;
		str += pattern;//扩展字符串以方便操作
		int size = str.size();
		for (int i = 0; i < size; i++)
		{
			pos = str.find(pattern, i);
			if (pos < size)
			{
				std::string s = str.substr(i, pos - i);
				result.push_back(s);
				i = pos + pattern.size() - 1;
			}
		}
		return result;
	}

	std::string getExtentionName(const std::string& str)
	{
		return str.substr(str.find_last_of('.') + 1);
	}

	std::string getFrontName(const std::string& str)
	{
		return str.substr(0, str.find_last_of('.'));
	}

	std::string getMiddleText(const std::string& str, const std::string& front, const std::string& behind)
	{
		auto lft = str.find(front);
		auto rht = str.find(behind);
		if (-1 == lft || -1 == rht) return "";
		return str.substr(lft + front.length(), rht - lft - front.length());

	}

#ifdef _WIN32
	wchar_t * charTowchar(const char* cchar)
	{
		wchar_t *m_wchar;
		int len = MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), NULL, 0);
		m_wchar = new wchar_t[len + 1];
		MultiByteToWideChar(CP_ACP, 0, cchar, strlen(cchar), m_wchar, len);
		m_wchar[len] = '\0';
		return m_wchar;
	}

	char * wcharTochar(const wchar_t* wchar)
	{
		char * m_char;
		int len = WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), NULL, 0, NULL, NULL);
		m_char = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, wchar, wcslen(wchar), m_char, len, NULL, NULL);
		m_char[len] = '\0';
		return m_char;
	}
#endif // _WIN32



	std::string getFileDir(const std::string& fileName)
	{
		if (fileName.length() <= 1)return "";
		int i = 0;
		for (i = fileName.length() - 1; i >= 0; i--)
		{
			if ('/' == fileName[i] || '\\' == fileName[i])
			{
				break;
			}
		}
		return fileName.substr(0, i);
	}

	std::string getFileName(const std::string& fullName)
	{
		if (fullName.length() <= 1)return "";
		int i = 0;
		for (i = fullName.length() - 1; i >= 0; i--)
		{
			if ('/' == fullName[i] || '\\' == fullName[i])
			{
				break;
			}
		}
		return fullName.substr(i + 1, fullName.length());
	}
}


