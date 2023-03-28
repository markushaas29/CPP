#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.hpp"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"
#include "Info.hpp"
#include "Node.hpp"

#ifndef FILETYPECONTAINER_HPP
#define FILETYPECONTAINER_HPP

namespace FS
{
	//---------------------------------------------------------------------------------------------------FileTypeContainer----------------------------------------------------------------------------------------
	template<typename List>
	class FileTypeContainer	{};
	
	template<typename Head>
	class FileTypeContainer<Typelist<Head>>
	{
	protected:
		std::filesystem::path rootPath;
		std::filesystem::path BuildDestPath(const std::string& src, const std::string &dst)
		{
			auto srcPath = std::filesystem::path(src);
			auto dstPath = std::filesystem::path(dst);
			std::string result = "";

			for (auto it = --srcPath.end() ; it != srcPath.begin(); --it)
			{
				if(*it == *(--this->rootPath.end()))
				{
					result = (*it).string() + "/" + result ;
					break;
				}
				
				result = (*it).string() + "/" + result ;
			}
			
			result = dst + result;
			
			return std::filesystem::path(std::string(result.cbegin(),(--result.cend())));
		}
		
		 std::vector<std::string> readFile = std::vector<std::string>();
	public:
		void SetRootPath(std::string p){ this->rootPath = std::filesystem::path(p);;}
		using Type = Head;
		
		void Add(FileInfo* fi)
		{
			if(strcmp(Type::Extension, fi->Extension()) == 0)
				Head::Add(fi); 
		}
		
		void CopyTo(std::string dest)
		{
			Logger::Log<Info>("Begin copy files of type: ", Head::Extension);
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{
				std::string dst = this->BuildDestPath(it->Info().Path(),dest);
				if(it->BelongsTo(this->rootPath))
					it->CopyTo(dst);
			}
			Logger::Log<Success>("All files of type ", Head::Extension," copied:");
		}
		
		void List()
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
				Logger::Log()<<it->Info()<<std::endl;
		}
		
		std::vector<std::string> Read(std::string name)
		{
			std::vector<std::string> r, result;
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{				
				if(String_::Contains(it->Info().Name() ,name) && std::find(this->readFile.begin(), this->readFile.end(), it->Info().Name()) == this->readFile.end())
				{
					Logger::Log()<<it->Info().Path()<<"\t"<<name<<std::endl;
					this->readFile.push_back(it->Info().Name());
					result = it->Read();
					return result;
				}
			}			

			return result;
		}
		
		template<typename ParseType>
		decltype(auto) Parse(const std::string& name)
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{				
				if(it->Info().Name() == name)
				{
					Logger::Log<Info>("Parsing: ", name);
					return it->template Parse<ParseType>();
				}
			}
			
			return typename ParseType::ParseCont();
		}
		
		template<typename Cont>
		void RegisterTo(Cont& cont)	{	Head::Instance().RegisterTo(cont);	}
		void Display(std::ostream& os)	{	Head::Display(os);	}
		FileTypeContainer()	{ }
	};
	
	template<typename Head, typename... Tail>
	class FileTypeContainer<Typelist<Head,Tail...>>: public FileTypeContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;

		void Add(FileInfo* fi)
		{
			if(strcmp(Type::Extension, fi->Extension()) == 0)
				Head::Add(fi); 
			else
				FileTypeContainer<Typelist<Tail...>>::Add(fi);
		}
		
		void CopyTo(std::string dest)
		{
			Logger::Log<Info>("Begin copy files of type: ", Head::Extension);
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{
				std::string dst = this->BuildDestPath(it->Path(),dest);
				if(it->BelongsTo(this->rootPath))
					it->CopyTo(dst);
			}
			Logger::Log<Success>("All files of type ", Head::Extension," copied:");
			
			FileTypeContainer<Typelist<Tail...>>::CopyTo(dest);
		}
		
		void List()
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
				Logger::Log()<<it->Path()<<it->Name()<<std::endl;
			
			FileTypeContainer<Typelist<Tail...>>::List();
		}
		
		std::vector<std::string> Read(std::string name)
		{
			std::vector<std::string> r, result;
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{				
				if(String_::Contains(it->Name() ,name) && std::find(this->readFile.begin(), this->readFile.end(), it->Name()) == this->readFile.end())
				{
					Logger::Log()<<it->Name()<<"\t"<<name<<std::endl;
					this->readFile.push_back(it->Name());
					result = it->Read();
					return result;
				}
			}						

			return FileTypeContainer<Typelist<Tail...>>::Read(name);
		}
		
		template<typename ParseType>
		decltype(auto) Parse(const std::string& name)
		{
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{				
				if(it->Info().Name() == name)
				{
					Logger::Log<Info>("Parsing: ", name);
					return it->template Parse<ParseType>();
				}
			}
			
			return FileTypeContainer<Typelist<Tail...>>::template Parse<ParseType>(name);
		}
		
		template<typename Cont>
		void RegisterTo(Cont& cont)
		{
			Head::Instance().RegisterTo(cont);
			FileTypeContainer<Typelist<Tail...>>::RegisterTo(cont);
		}
		
		void Display(std::ostream& os)
		{
			Head::Display(os);
			FileTypeContainer<Typelist<Tail...>>::Display(os);
		}
		
		FileTypeContainer() { };
	};

}


#endif
