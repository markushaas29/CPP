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
	public:
		void SetRootPath(std::string p){ rootPath = std::filesystem::path(p);;}
		using Type = Head;
		using DataType = std::unique_ptr<Type>;
		using ContainerType = std::vector<DataType>;
		
		void Add(std::shared_ptr<FileInfo> fi)
		{
			std::cout<<"ADD"<<*fi<<std::endl;
			container->push_back(std::make_unique<Type>(fi));
		}
		
		void CopyTo(std::string dest)
		{
			Logger::Log<Info>("Begin copy files of type: ", Head::Extension);
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{
				std::string dst = BuildDestPath(it->Info().Path(),dest);
				if(it->BelongsTo(rootPath))
					it->CopyTo(dst);
			}
			Logger::Log<Success>("All files of type ", Head::Extension," copied:");
		}
		
		void List()
		{
			Logger::Log<Info>(Type::Extension,": ",container->size());
			std::for_each(container->cbegin(), container->cend(), [&](const auto& it) {Logger::Log<Info>("\t- ",it->Path(),it->Name());}); 
		}

		template<typename Cont>
		void RegisterTo(Cont& cont)	{	Head::Instance().RegisterTo(cont);	}
		std::ostream& Display(std::ostream& os)	{	return	Head::Display(os); 	}
	protected:
		std::filesystem::path rootPath;
		std::filesystem::path BuildDestPath(const std::string& src, const std::string &dst)
		{
			auto srcPath = std::filesystem::path(src);
			auto dstPath = std::filesystem::path(dst);
			std::string result = "";

			for (auto it = --srcPath.end() ; it != srcPath.begin(); --it)
			{
				if(*it == *(--rootPath.end()))
				{
					result = (*it).string() + "/" + result ;
					break;
				}
				
				result = (*it).string() + "/" + result ;
			}
			
			result = dst + result;
			
			return std::filesystem::path(std::string(result.cbegin(),(--result.cend())));
		}
	private:
		inline static std::unique_ptr<ContainerType> container = std::make_unique<ContainerType>();
	};
	
	template<typename Head, typename... Tail>
	class FileTypeContainer<Typelist<Head,Tail...>>: public FileTypeContainer<Typelist<Tail...>>
	{
	public:
		using Type = Head;
		using DataType = std::unique_ptr<Type>;
		using ContainerType = std::vector<DataType>;
		using Base = FileTypeContainer<Typelist<Tail...>>;
		
		void Add(std::shared_ptr<FileInfo> fi)
		{
			std::cout<<"ADD"<<*fi<<std::endl;
			container->push_back(std::make_unique<Type>(fi));
		}

		void CopyTo(std::string dest)
		{
			Logger::Log<Info>("Begin copy files of type: ", Head::Extension);
			for(auto it = Head::Nodes().cbegin(); it != Head::Nodes().cend(); ++it)
			{
				std::string dst = BuildDestPath(it->Path(),dest);
				if(it->BelongsTo(this->rootPath))
					it->CopyTo(dst);
			}
			Logger::Log<Success>("All files of type ", Head::Extension," copied:");
			
			Base::CopyTo(dest);
		}
		
		void List()
		{
			Logger::Log<Info>(Type::Extension,": ",container->size());
			std::for_each(container->cbegin(), container->cend(), [&](const auto& it) {Logger::Log<Info>("\t- ",it->Path(),it->Name());});
			Base::List();
		}
		
		template<typename Cont>
		void RegisterTo(Cont& cont)
		{
			Head::Instance().RegisterTo(cont);
			Base::RegisterTo(cont);
		}
		
		std::ostream& Display(std::ostream& os)	{ return Base::Display(Head::Display(os)); }
	private:
		inline static std::unique_ptr<ContainerType> container = std::make_unique<ContainerType>();
	};

}


#endif
