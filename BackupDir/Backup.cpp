#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <cstdint>
#include <filesystem>
#include <unordered_map>
#include "../Logger/Logger.hpp"
#include "../File/Account.hpp"
#include "../File/Repository.hpp"
#include "../File/FileSystem.hpp"
namespace fs = std::filesystem;

std::vector<std::string> GetFiles(std::string filename = "Backup.txt")
{
	std::ifstream file(filename);
	std::vector<std::string> result;
	if (file.is_open()) 
	{
		std::string line;
		while (std::getline(file, line)) {
			result.push_back(line);
		}
		file.close();
	}

	return result;
}

bool Directory_exists(const fs::path& p, fs::file_status s = fs::file_status{})
{
    if(fs::status_known(s) ? fs::exists(s) : fs::exists(p))
        return true;
   
	return false;
}

std::string GetTarget()
{
    const std::string usb ="/media/markus/BACKUP/";
 	const std::string seagate ="/media/markus/Seagate/";
	const std::string usb128 ="/media/markus/eebbb521-a27a-4f77-a520-6628d7a9b923/";
	
	if(Directory_exists(usb)) 
		return usb;
	if(Directory_exists(seagate)) 
		return seagate;	
	if(Directory_exists(usb128)) 
		return usb128;	
	
	return "";
}

using namespace FS;
//----------------------------------------------------------------------------PRODUCT----------------------------------------------------------
int main()
{
	auto dirs = GetFiles();
	
	if(dirs.size() == 0)
	{
		Logger::Log()<<"No files to backup!";
	}
	
	auto source = GetTarget();
	
	if(source == "")
	{
		Logger::Log()<<"No backup medium!";
		return 0;
	}
	
	auto to = source + "CPP/";
	
	if(Directory_exists(to))
		std::filesystem::remove_all(to);
	
	fs::create_directories(to);
	
	for(auto dir : dirs)
	{
		Logger::Log()<<"Dir: "<<dir<<"\tTo"<<to<<std::endl;
		Backup::Repository::Backup(dir,to);
	}		
		
    return 0;
};

//----------------------------------------------------------------------------TEST----------------------------------------------------------
