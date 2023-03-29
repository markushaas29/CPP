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
	std::filesystem::file_time_type lm;	
	
	auto fi = std::make_unique<FS::FileInfo>(path);
	auto ci = std::make_unique<FS::FileInfo>(cpath);
	auto di = std::make_unique<FS::DirectoryInfo>(dpath,lm);

	auto f = File(std::move(fi));
	auto c = File(std::move(ci));
	auto d = Directory(std::move(di));

      	f.Display(std::cout);
      	c.Display(std::cout);
      	d.Display(std::cout);

      	std::cout<<d.Path()<<std::endl;

	FileSystem::List(dpath);
	//FileSystem::GetInfos(*di);

	return 0;
}
