#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../Info.hpp"
#include "../Node.hpp"
#include "../../Common/DateTimes.hpp"
namespace fs = std::filesystem;
using namespace FS;

int main()
{      
    	std::cout<<"START"<<std::endl;
	auto path = fs::path{ "/home/markus/Dokumente/cpp/Home/Test/TestFiles/NodeTest.txt" };
	
	auto fi = FS::FileInfo(path);

	auto f = File(&fi);

//	File::Display(std::cout);
	f<<"TEST"<<std::endl;
	f<<"TEST";
	return 0;
}
