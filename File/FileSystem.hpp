#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>  // for high_resolution_clock
#include <iterator>
#include <vector>
#include <cstdlib>
#include <memory>
#include "../Logger/Logger.hpp"
#include "../String/String_.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Node.hpp"

#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

class FileSystem
{
	using Delimiter = T::char_<'/'> ;
	inline static int level = 0;
	inline static std::shared_ptr<std::vector<FS::Info*>> nodes = std::shared_ptr<std::vector<FS::Info*>>();
		
public:

	static std::unique_ptr<std::vector<FS::Info*>> List(const fs::path& pathToScan) {
		std::unique_ptr<std::vector<FS::Info*>> result = std::make_unique<std::vector<FS::Info*>>();
		auto start = std::chrono::high_resolution_clock::now();
		Logger::Log<Info>("List Start:");
		std::cout<<"|->"<<pathToScan<<std::endl;
		for (const auto& entry : fs::directory_iterator(pathToScan)) {
			const auto filenameStr = entry.path().filename().string();
			for(int i = 0; i < level; ++i)
				std::cout<<"|";
			
			if (entry.is_directory()) {
				++level;
				auto dirnodes = FileSystem::List(entry.path());
				FS::DirectoryInfo* dir = new FS::DirectoryInfo(entry.path(),entry.last_write_time(),*dirnodes);
				//~ nodes->push_back(dir);
				result->push_back(dir);
			}
			else {
				FS::FileInfo* file = new FS::FileInfo(entry.path(), entry.last_write_time(), entry.file_size());
				//~ nodes->push_back(file);
				result->push_back(file);
				std::cout<<"|--"<<*file<<std::endl;
			}
		}
		
		--level;
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		//~ Logger::Log<Info>("List End:", finish, " Duration: ", elapsed.count() );
		Logger::Log<Info>(" =>Duration: ", elapsed.count() );
		
		return result;
	}
	
	static std::vector<std::string> ReadLines(std::string path)	{	return FS::ReadLines(path);	}
	static void WriteLines(std::string path, std::vector<std::string>)	{	}
	
	static void CreateDirectories(std::string src, std::string dest)
	{
		auto srcPath = std::filesystem::path(src);
		auto dstPath = std::filesystem::path(dest);
		
		for(auto it = FS::Directory::Nodes().cbegin(); it != FS::Directory::Nodes().cend(); ++it)
		{
			if(it->BelongsTo(srcPath))
			{
				auto destPath = BuildDestPath(srcPath, it->Info().Path(),dstPath);
				fs::create_directories(destPath);
				Logger::Log()<<"Directory created :"<<destPath<<std::endl;
			}
		}			
	}
	
	static std::filesystem::path BuildDestPath(const fs::path& root, const fs::path& src, const fs::path& dst)
	{
		auto folder = src.end();
		auto rootFolder = *(--root.end());
		fs::path result;
		
		while(*(--folder) != rootFolder)
			result = *folder / result;
		
		return dst / rootFolder / result;
	}
};

#endif
