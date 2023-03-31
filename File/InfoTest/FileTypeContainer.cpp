#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../Info.hpp"
#include "../Node.hpp"
#include "../FileSystem.hpp"
#include "../FileTypeContainer.hpp"
#include "../../Common/DateTimes.hpp"
namespace fs = std::filesystem;
using namespace FS;

int main()
{      
    	std::cout<<"START"<<std::endl;
	using TypeContainer = FS::FileTypeContainer<Typelist<FS::KEYS,FS::CPP, FS::HPP, FS::CTRV,FS::CSV>>;
	auto dPath = fs::path{ "/home/markus/Dokumente/cpp" };
	auto d2Path = fs::path{ "/home/markus/Dokumente/cpp" };
	std::filesystem::file_time_type lm;	
	
	auto fi = std::make_shared<FS::FileInfo>(d2Path);
	auto di = std::make_shared<FS::DirectoryInfo>(d2Path,lm);

	auto f = File(fi);
	auto d = Directory(di);

	TypeContainer t;
	t.SetRootPath("/home/markus/Dokumente/cpp");
	t.List();

	auto i = t.Get(fi);

	std::cout<<(i)->Name()<<std::endl;
	return 0;
}
