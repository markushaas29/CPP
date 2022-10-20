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
#include <map>
#include "../Logger/Logger.hpp"
#include "../File/Account.hpp"
#include "../File/Repository.hpp"
#include "../File/FileSystem.hpp"
namespace fs = std::filesystem;

std::vector<std::string> ReadFile(std::string filename = "Backup_.txt")
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

decltype(auto) ParseFile(const std::vector<std::string>& lines)
{
	std::map<std::string, std::vector<std::string>> result;
	std::string source = "";
	for(auto it = lines.cbegin(); it != lines.cend(); ++it)
	{
			Logger::Log<Info>(*it);
		auto vals = String_::Split<T::char_<':'>>(*it);
		if(vals[0] == "Source")
			source = vals[1];
		else if(vals[0] == "Directory")
		{
			auto path = source + vals[1];
			Logger::Log<Info>(path);
			result[source].push_back(path);
		}
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
	const std::string usb128 ="/media/markus/539512b8-9fc1-41a3-b359-2be6c2eecdcc/";
	
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
	auto lines = ReadFile();
	Logger::Log(lines.cbegin(), lines.cend());
	auto dirs = ParseFile(lines);
	
	if(dirs.size() == 0)
	{
		Logger::Log<Info>()<<"No files to backup!"<<std::endl;
		std::cout<<"No files to backup!"<<std::endl;
	}
	
	auto source = GetTarget();
	
	if(source == "")
	{
		//~ Logger::Log<Info>()<<"No backup medium!"<<std::endl;
		std::cout<<"No backup medium!"<<std::endl;
		return 0;
	}
	
	//~ auto to = source + "CPP/";
	auto to = source + "Backup/";
	
	if(Directory_exists(to))
		std::filesystem::remove_all(to);
	
	fs::create_directories(to);
	
	for(auto dir : dirs)
	{
		for(auto subdir : dir.second)
		{
			Logger::Log()<<"Dir: "<<subdir<<"\tTo"<<to<<std::endl;
			Backup::Repository::Backup(subdir,to);
		}
	}		
		
    return 0;
};

//----------------------------------------------------------------------------TEST----------------------------------------------------------
