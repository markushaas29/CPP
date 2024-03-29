#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <cstring>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Visitor/Visitor.hpp"

#ifndef INFO_H
#define INFO_H

namespace fs = std::filesystem;

namespace FS
{
//---------------------------------------------------------------------------------------------------time_Conversion----------------------------------------------------------------------------------------
	template <typename TP>
	std::time_t to_time_t(TP tp)
	{
		using namespace std::chrono;
		auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
				+ system_clock::now());
		return system_clock::to_time_t(sctp);
	}
	
	std::string to_timestring(std::time_t tt)
	{
		std::tm *gmt = std::gmtime(&tt);
		std::stringstream buffer;
		buffer << std::put_time(gmt, "%A, %d %B %Y %H:%M");
		std::string formattedFileInfoTime = buffer.str();
		return formattedFileInfoTime;
	}
	
	class Metainfo 
	{
	public:
		using NodesType = std::vector<std::shared_ptr<Metainfo>>;
		virtual ~Metainfo(){};
		virtual long Size() const {return size; };
		virtual std::shared_ptr<NodesType> GetNodes(std::shared_ptr<NodesType> nodes) 
		{ 
			nodes->push_back(std::shared_ptr<Metainfo>(this)); 
			return nodes;
		};
		const std::string& Name() const{ return name; };
		const std::string& Path() const { return path; };
		bool Exists() const {  return exists; };
		const std::time_t LastModification()const { return to_time_t(lastModification); };
		decltype(auto) LastWriteTime()const { return lastModification; };
		decltype(auto) GetInfo() const { return Name() + std::string("\t") + std::to_string(Size()) + std::string("\t") + to_timestring(LastModification()) + std::string("\t") + Path();}
		const std::string virtual PrintInfo(std::ostream& out) const { return GetInfo() ; };
		bool operator==(const Metainfo& m) const { return m.name == name && m.path == path; 	}
		//constexpr std::strong_ordering operator<=>( const Metainfo& ) noexcept { return ymd <=> d.ymd; }

		std::ostream& Display(std::ostream& out) const { 
			auto i = PrintInfo(out); 
			return out<<i;
		};
	protected:
		const std::string name;
		const std::string path;
		bool exists = std::filesystem::exists(path);
		std::filesystem::path fs_path;
		const std::filesystem::file_time_type lastModification;
		std::uintmax_t size;
		Metainfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::uintmax_t s): fs_path(p), name(p.filename()), path(p), size(s), lastModification(lm){ };
		Metainfo(std::filesystem::path p, std::filesystem::path pp, std::filesystem::file_time_type lm, std::uintmax_t s): fs_path(pp), name(p.filename()), path(p), size(s), lastModification(lm){ };
		virtual Metainfo* Child(int n) = 0;
	};
	
	std::ostream& operator<<(std::ostream& out, const Metainfo& n)	{	return n.Display(out);	}

	class FileInfo : public Metainfo
	{
	public:
		~FileInfo(){};
		FileInfo(std::filesystem::path p, std::filesystem::file_time_type lm = std::filesystem::file_time_type(), std::uintmax_t s = 0): Metainfo(p, p.parent_path(),lm, s), extension{p.extension()} { 	};
		const std::string&  Extension() const { return extension; };
	protected:
		virtual Metainfo* Child(int n) { return 0; }
	private:
		const std::string extension;
		const fs::file_time_type lastModification;
	};
	
	std::ostream& operator<<(std::ostream& out, const FileInfo* n)	{	return out<<n->PrintInfo(out);	}
	
	class DirectoryInfo : public Metainfo
	{   
	public: 
		~DirectoryInfo(){};
		DirectoryInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::shared_ptr<NodesType> n = std::make_shared<NodesType>()):Metainfo(p,lm, 0), nodes(n){ size = Size();	};
		
		long Size() const
		{
			long result = 0;
			Metainfo* child;
			for(auto it = nodes->cbegin(); it != nodes->cend(); ++it)
				result += (*it)->Size();
							
			return result;
		}
		
		std::shared_ptr<NodesType> GetNodes(std::shared_ptr<NodesType> ptr)
		{
			ptr->push_back(std::shared_ptr<DirectoryInfo>(this));
			for(auto it = nodes->cbegin(); it != nodes->cend(); ++it)
				ptr = (*it)->GetNodes(std::move(ptr));
							
			return ptr;
		}
		
		const std::string virtual PrintInfo(std::ostream& out) const 
		{ 
			auto s = Name() + std::string("\t") + std::to_string(Size()) + std::string("\t") + to_timestring(LastModification()) + std::string("\t") + Path() ; 
			for(auto it = nodes->cbegin(); it != nodes->cend(); ++it)
				out<<"|-->"<<*it<<std::endl;
				
			return s;
		};
		
//		std::vector<std::shared_ptr<Metainfo>> Nodes() 
//		{ 
//			std::vector<std::shared_ptr<Metainfo>> result;
//			std::for_each(nodes->cbegin(),nodes->cend(),[&](auto& m)
//					{
//					if
//					result.push_back(std::make_shared<Metainfo>(m)); });
//			return result; 
//		}
	protected:
		virtual Metainfo* Child(int n) { return 0; }
	private:
		std::shared_ptr<NodesType> nodes;
	};
	
	std::ostream& operator<<(std::ostream& out, const DirectoryInfo* n)	
	{	
		n->PrintInfo(out);	
		return out;	
	}
}


#endif
