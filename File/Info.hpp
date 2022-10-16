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
#include "../Typelist/Typelist.h"
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
	
	
//---------------------------------------------------------------------------------------------------Info----------------------------------------------------------------------------------------
	class Info : public BaseVisitable<>
	{
	protected:
		const std::string name;
		const std::string path;
		std::filesystem::path fs_path;
		const std::filesystem::file_time_type lastModification;
		std::uintmax_t size;

		Info(std::filesystem::path p, std::filesystem::file_time_type lm, std::uintmax_t s): fs_path(p), name(p.filename()), path(p), size(s), lastModification(lm){ };
		Info(std::filesystem::path p, std::filesystem::path pp, std::filesystem::file_time_type lm, std::uintmax_t s): fs_path(pp), name(p.filename()), path(p), size(s), lastModification(lm){ };

		virtual Info* Child(int n) = 0;
	public:
		DEFINE_VISITABLE();
		DEFINE_CONSTVISITABLE();
		virtual ~Info(){};
		
		virtual long Size() const {return size; };
		virtual std::unique_ptr<std::vector<const Info*>> GetNodes(std::unique_ptr<std::vector<const Info*>> nodes) const 
		{ 
			nodes->push_back(this); 
			return nodes;
		};
		const std::string& Name() const{ return name; };
		const std::string& Path() const { return path; };
		const std::time_t LastModification()const { return to_time_t(this->lastModification); };
		const decltype(auto) LastWriteTime()const { return this->lastModification; };
		const decltype(auto) GetInfo() const { return this->Name() + std::string("\t") + std::to_string(this->Size()) + std::string("\t") + to_timestring(this->LastModification()) + std::string("\t") + this->Path();}
		const std::string virtual PrintInfo(std::ostream& out) const { return this->GetInfo() ; };
		
		std::ostream& Display(std::ostream& out) const { 
			auto i = this->PrintInfo(out); 
			return out<<i;
		};
	};
	
	std::ostream& operator<<(std::ostream& out, const Info& n)	{	return n.Display(out);	}

//---------------------------------------------------------------------------------------------------FileInfo----------------------------------------------------------------------------------------

	class FileInfo : public Info
	{
	private:
		char* extension;
		const fs::file_time_type lastModification;
		
	protected:
		virtual Info* Child(int n) { return 0; }
	public:
		DEFINE_VISITABLE();
		~FileInfo(){};

		FileInfo(std::filesystem::path p, std::filesystem::file_time_type lm = std::filesystem::file_time_type(), std::uintmax_t s = 0): Info(p, p.parent_path(),lm, s)
		{ 
			size_t length = strlen( p.extension().c_str() );
			extension = new char[ length + 1 ];
			strcpy( extension, p.extension().c_str() );
		};
		
		const char*  Extension() const { return this->extension; };
	};
	
	std::ostream& operator<<(std::ostream& out, const FileInfo* n)	{	return out<<n->PrintInfo(out);	}
	
//---------------------------------------------------------------------------------------------------DirectoryInfo----------------------------------------------------------------------------------------

	class DirectoryInfo : public Info
	{   
	private:
		std::vector<Info*> nodes;
	protected:
		virtual Info* Child(int n) { return 0; }
	public: 
		DEFINE_VISITABLE();
		~DirectoryInfo(){};
		
		DirectoryInfo(std::filesystem::path p, std::filesystem::file_time_type lm, std::vector<Info*> n):Info(p,lm, 0), nodes(n)
		{
			this->size = this->Size();
			Logger::Log("Count: ",n.size()," Size: ", size);
		};
		
		DirectoryInfo(std::filesystem::path p, std::filesystem::file_time_type lm):Info(p,lm, 0), nodes{}
		{
			this->size = this->Size();
			Logger::Log(" Size: ", size);
		};
		
		long Size() const
		{
			long result = 0;
			Info* child;
			for(auto it = nodes.cbegin(); it != nodes.cend(); ++it)
				result += (*it)->Size();
							
			return result;
		}
		
		std::unique_ptr<std::vector<const Info*>> GetNodes(std::unique_ptr<std::vector<const Info*>> ptr) const
		//~ std::vector<const Info*> GetNodes(std::vector<const Info*> ptr) const
		{
			ptr->push_back(this);
			//~ std::cout<<"Adresss"<<this<<std::endl;
			for(auto it = nodes.cbegin(); it != nodes.cend(); ++it)
				ptr = (*it)->GetNodes(std::move(ptr));
							
			return ptr;
		}
		
		const std::string virtual PrintInfo(std::ostream& out) const 
		{ 
			auto s = this->Name() + std::string("\t") + std::to_string(this->Size()) + std::string("\t") + to_timestring(this->LastModification()) + std::string("\t") + this->Path() ; 
			for(auto n : nodes)
				out<<"|-->"<<*n<<std::endl;
				
			return s;
		};
		
		const std::vector<Info*>& Nodes() { return this->nodes; }
	};
	
	std::ostream& operator<<(std::ostream& out, const DirectoryInfo* n)	
	{	
		n->PrintInfo(out);	
		return out;	
	}
}


#endif
