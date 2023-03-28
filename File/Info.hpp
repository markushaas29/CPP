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
	
	class Metainfo : public BaseVisitable<>
	{
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
	public:
		DEFINE_VISITABLE();
		DEFINE_CONSTVISITABLE();
		virtual ~Metainfo(){};
		
		virtual long Size() const {return size; };
		virtual std::unique_ptr<std::vector<const Metainfo*>> GetNodes(std::unique_ptr<std::vector<const Metainfo*>> nodes) const 
		{ 
			nodes->push_back(this); 
			return nodes;
		};
		const std::string& Name() const{ return name; };
		const std::string& Path() const { return path; };
		bool Exists() const {  return exists; };
		const std::time_t LastModification()const { return to_time_t(this->lastModification); };
		decltype(auto) LastWriteTime()const { return this->lastModification; };
		decltype(auto) GetInfo() const { return this->Name() + std::string("\t") + std::to_string(this->Size()) + std::string("\t") + to_timestring(this->LastModification()) + std::string("\t") + this->Path();}
		const std::string virtual PrintInfo(std::ostream& out) const { return this->GetInfo() ; };
		
		std::ostream& Display(std::ostream& out) const { 
			auto i = this->PrintInfo(out); 
			return out<<i;
		};
	};
	
	std::ostream& operator<<(std::ostream& out, const Metainfo& n)	{	return n.Display(out);	}

	class FileInfo : public Metainfo
	{
	private:
		char* extension;
		const fs::file_time_type lastModification;
		
	protected:
		virtual Metainfo* Child(int n) { return 0; }
	public:
		DEFINE_VISITABLE();
		~FileInfo(){};

		FileInfo(std::filesystem::path p, std::filesystem::file_time_type lm = std::filesystem::file_time_type(), std::uintmax_t s = 0): Metainfo(p, p.parent_path(),lm, s)
		{ 
			size_t length = strlen( p.extension().c_str() );
			extension = new char[ length + 1 ];
			strcpy( extension, p.extension().c_str() );
		};
		
		const char*  Extension() const { return this->extension; };
	};
	
	std::ostream& operator<<(std::ostream& out, const FileInfo* n)	{	return out<<n->PrintInfo(out);	}
	
	class DirectoryInfo : public Metainfo
	{   
	private:
		std::unique_ptr<std::vector<std::unique_ptr<Metainfo>>> nodes;
	protected:
		virtual Metainfo* Child(int n) { return 0; }
	public: 
		DEFINE_VISITABLE();
		~DirectoryInfo(){};
		
		DirectoryInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::unique_ptr<std::vector<std::unique_ptr<Metainfo>>> n):Metainfo(p,lm, 0), nodes(std::move(n)){	this->size = this->Size();	};
		DirectoryInfo(std::filesystem::path p, std::filesystem::file_time_type lm):Metainfo(p,lm, 0), nodes{}{	this->size = this->Size();	};
		
		long Size() const
		{
			long result = 0;
			Metainfo* child;
			for(auto it = nodes->cbegin(); it != nodes->cend(); ++it)
				result += (*it)->Size();
							
			return result;
		}
		
		std::unique_ptr<std::vector<const Metainfo*>> GetNodes(std::unique_ptr<std::vector<const Metainfo*>> ptr) const
		{
			ptr->push_back(this);
			for(auto it = nodes->cbegin(); it != nodes->cend(); ++it)
				ptr = (*it)->GetNodes(std::move(ptr));
							
			return ptr;
		}
		
		const std::string virtual PrintInfo(std::ostream& out) const 
		{ 
			auto s = this->Name() + std::string("\t") + std::to_string(this->Size()) + std::string("\t") + to_timestring(this->LastModification()) + std::string("\t") + this->Path() ; 
			for(auto it = nodes->cbegin(); it != nodes->cend(); ++it)
				out<<"|-->"<<*it<<std::endl;
				
			return s;
		};
		
//		std::vector<std::unique_ptr<Metainfo>> Nodes() 
//		{ 
//			std::vector<std::unique_ptr<Metainfo>> result;
//			std::for_each(nodes->cbegin(),nodes->cend(),[&](auto& m)
//					{
//					if
//					result.push_back(std::make_unique<Metainfo>(m)); });
//			return result; 
//		}
	};
	
	std::ostream& operator<<(std::ostream& out, const DirectoryInfo* n)	
	{	
		n->PrintInfo(out);	
		return out;	
	}
}


#endif
