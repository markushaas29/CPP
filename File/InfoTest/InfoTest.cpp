#include <iostream>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include "../Info.hpp"
#include "../../Common/DateTimes.hpp"
namespace fs = std::filesystem;

int main()
{      
	auto p = std::filesystem::temp_directory_path() / "example.bin";
	std::ofstream{p.c_str()}.put('a'); // create file
 
    	std::filesystem::file_time_type ftime = std::filesystem::last_write_time(p);
 
    	// read back from the filesystem
    	ftime = std::filesystem::last_write_time(p);
    	std::cout<<"START"<<std::endl;
	auto path = fs::path{ "/home/marius/docs" };
	
	auto fi = FS::FileInfo(path,ftime);

	std::cout<<fi<<std::endl;
	std::cout<<fi.Name()<<std::endl;
	std::cout<<fi.Path()<<std::endl;
	std::cout<<path<<std::endl;
	return 0;
}
