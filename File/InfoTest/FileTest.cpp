#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../Info.hpp"
#include "../Node.hpp"
#include "../FileSystem.hpp"
#include "../../Common/DateTimes.hpp"
namespace fs = std::filesystem;
using namespace FS;

int main()
{      
    	std::cout<<"START"<<std::endl;
	auto path = fs::path{ "/home/markus/Dokumente/cpp/Home/Test/TestFiles/NodeTest.txt" };
	auto cpath = fs::path{ "/home/markus/Dokumente/cpp/Home/Test/TestFiles/NodeTest.csv" };
	auto dpath = fs::path{ "/home/markus/Dokumente/cpp/Home/Test/TestFiles" };
	auto d2Path = fs::path{ "/home/markus/Dokumente/cpp" };
	std::filesystem::file_time_type lm;	
	
	auto fi = std::make_shared<FS::FileInfo>(path);
	auto ci = std::make_shared<FS::FileInfo>(cpath);
	auto di = std::make_shared<FS::DirectoryInfo>(d2Path,lm);

	auto f = File(fi);
	auto c = File(ci);
	auto d = Directory(di);

      	f.Display(std::cout);
      	c.Display(std::cout);
      	d.Display(std::cout);

      	std::cout<<d.Path()<<std::endl;
      	std::cout<<di->Path()<<std::endl;

	FileSystem::List(dpath);
	//FileSystem::List(d2Path);
	FileSystem::GetInfos(di);

	return 0;
}
