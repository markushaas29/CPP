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
public:
	using Iterator = std::vector<FS::Metainfo*>::const_iterator;
	using ContainerType = std::vector<std::shared_ptr<FS::Metainfo>>;
	
	static std::unique_ptr<ContainerType> List(const fs::path& pathToScan) {
		auto result = std::make_unique<ContainerType>();

		std::cout<<"|->"<<pathToScan<<std::endl;
		for (const auto& entry : fs::directory_iterator(pathToScan)) {
			const auto filenameStr = entry.path().filename().string();
			for(int i = 0; i < level; ++i)
				std::cout<<"|";
			
			if (entry.is_directory()) {
				++level;
				std::unique_ptr<ContainerType> dirnodes = FileSystem::List(entry.path());
				auto dir = std::make_shared<FS::DirectoryInfo>(entry.path(),entry.last_write_time(),std::move(dirnodes));
				result->push_back(dir);
				nodes->push_back(dir);
			}
			else {
				auto file = std::make_shared<FS::FileInfo>(entry.path(), entry.last_write_time(), entry.file_size());
				result->push_back(file);
				nodes->push_back(file);
				std::cout<<"|--"<<*file<<std::endl;
			}
		}
		
		--level;
		
		return result;
	}
	
	static decltype(auto) GetInfos(std::shared_ptr<FS::DirectoryInfo> di) {
		std::unique_ptr<ContainerType> result = std::make_unique<ContainerType>();
		for (const auto& entry : fs::directory_iterator(di->Path())) {
			const auto filenameStr = entry.path().filename().string();
			
			if (entry.is_directory()) {
				auto dirnodes = FileSystem::List(entry.path());
				auto dir = std::make_shared<FS::DirectoryInfo>(entry.path(),entry.last_write_time(),std::make_shared<ContainerType>(dirnodes->cbegin(),dirnodes->cend()));
				result->push_back(dir);
			}
			else 
				result->push_back(std::make_shared<FS::FileInfo>(entry.path(), entry.last_write_time(), entry.file_size()));
		}
		
		return result;
	}
	
	//static std::unique_ptr<FS::DirectoryInfo> GetInfos(std::unique_ptr<FS::DirectoryInfo> di) 
	//{	
	//	auto infos = GetInfos(*di);
	//	return std::make_unique<FS::DirectoryInfo>(di->Path(), di->LastWriteTime(), std::move(infos));	
	//}
	
	static decltype(auto) Begin(){ return nodes->cbegin(); }
	static decltype(auto) End(){ return nodes->cend(); }
	static std::vector<std::string> ReadLines(std::string path)	{	return FS::ReadLines(path);	}
	static void WriteLines(std::string path, std::vector<std::string>)	{	}
	
	template<typename Iterator>
	static void CreateDirectories(Iterator begin, Iterator end, const std::string& src, const std::string& dest)
	{
		auto srcPath = std::filesystem::path(src);
		auto dstPath = std::filesystem::path(dest);
		
		for(auto it = begin; it != end; ++it)
		{
			if((*it)->BelongsTo(srcPath))
			{
				auto destPath = BuildDestPath(srcPath, (*it)->Path(),dstPath);
				fs::create_directories(destPath);
				Logger::Log<Info>()<<"Directory created :"<<destPath<<std::endl;
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
private:
	using Delimiter = T::char_<'/'> ;
	inline static int level = 0;
	inline static std::unique_ptr<ContainerType> nodes = std::make_unique<ContainerType>();
		
};

#endif
