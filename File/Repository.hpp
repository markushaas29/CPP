#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <filesystem>
#include <memory>
#include "FileTypeContainer.hpp"
#include "FileSystem.hpp"
#include "Info.hpp"
#include "Node.hpp"
#include "Comdirect.hpp"
#include "AccountContainer.hpp"
#include "Raiba.hpp"
#include "Custom.hpp"
#include "Direction.hpp"
#include "RepositoryValue.hpp"
#include "RepositoryObjectVisitor.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../InputManager/InputManager.hpp"
#include "../Typelist/Typelist.h"
#include "../Home/CounterContainer.hpp"
#include "../Home/StageContainer.hpp"
#include "../Common/DateTimes.hpp"
#include "../Visitor/Visitor.hpp"


#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

namespace CSV
{
	
	template<typename T, typename ResultT = typename T::ResultValueType> decltype(auto) Get();
	
	struct Repository
	{
		using InputIterator = std::vector<std::string>::const_iterator;
		using FileTypes = Typelist<FS::INPUT,FS::KEYS,FS::CPP, FS::HPP, FS::CTRV,FS::CSV>::Type;
		using TypeContainer = FS::FileTypeContainer<FileTypes>;
		using DirectionType = Bank::Direction;
		using Parsers = Typelist<CE1,CVat,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTHW, Bank::Custom<0>, Bank::Raiba<0>, Bank::Comdirect<0>,StageContainerType>::Type;
		using ParserContainer = FS::FileTypeContainer<Parsers>;
		using VisitorType = FS::RepositoryObjectVisitor<InputIterator>;
		using VisitorContainer = std::map<std::string, VisitorType>;
	
		template<typename Iterator>
		void Map(const Iterator& begin, const Iterator& end)
		{
			for(Iterator it = begin; it != end; ++it)
				(*it)->Accept(treeParser);
		}
		
		void CopyTo(std::string dest)
		{
			types.SetRootPath(Root);
			types.CopyTo(dest);
		}
		
		void List()	{	types.List();	}
		void SetRootPath(std::string s) { Root = s; }
		void SetDestPath(std::string s) { Dest = s; }
		
		void Map(std::string path)
		{
			auto infos = FileSystem::List(path);
			
			for(auto i : *infos)
				nodes->push_back(i);

			auto root = fs::directory_entry(path);
			auto dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),*infos);
			
			nodes->push_back(dir);
			Map(nodes->cbegin(), nodes->cend());
		}
		
		std::vector<std::string> Read(std::string s)	{	return types.Read(s);	}
		
		void Register()
		{
			parsers.RegisterTo(visitors);	
			
			for(auto kv : visitors)
				Logger::Log<Info>("Register:",kv.first);
		}
		
		template<typename ParseType>
		typename ParseType::ParseCont Parse(std::string s)
		{
			Logger::Log("Parsing: ",s);
			return types.Parse<ParseType>(s);			
		}
		
		void ParseAll()
		{
			for (auto it = visitors.begin(); it != visitors.end(); it++)
			{
				for(auto itNode = nodes->cbegin(); itNode != nodes->cend(); ++itNode )
				{
					if((it->second.IsVisitorOf( (*itNode)->Name() )))
					{
						Logger::Log("Repository start parsing: ",it->first);
						auto lines = Read((*itNode)->Name());	
						it->second.Parse(lines.cbegin(), lines.cend());
						it->second.Update(lines.cbegin(), lines.cend());
					}
				}			
			}
		}
	public:
		static Repository& Instance()
		{
			static Repository instance;
			return instance;
		}
		
		void Display(std::ostream& os){	parsers.Display(os);	}
	private:
		Repository()	
		{ 
			Logger::Log<Info>("Repository constructor");
			std::string csv = "/home/markus/Downloads/CSV_Files";
			Map(csv);
			Register();
			ParseAll();	
		};
		
		~Repository()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Repository& operator=(const Repository&) = delete;
		Repository(const Repository& c) = delete;
	
		template<typename T, typename ResultT>
		friend decltype(auto) Get();
		
		static inline VisitorContainer visitors = VisitorContainer();
		static inline TypeContainer types = TypeContainer();
		static inline ParserContainer parsers = ParserContainer();
		inline static std::string Root = ""; 
		inline static std::string Dest = ""; 
				
		class TreeParserVisitor: 
			public BaseVisitor,
			public Visitor<FS::DirectoryInfo>,
			public Visitor<FS::FileInfo>
		{
		public:
			virtual void Visit(FS::DirectoryInfo& di) {	FS::Directory::Add(&di); };
			virtual void Visit(FS::FileInfo& fi) { types.Add(&fi); };
		};	
		
		static inline TreeParserVisitor treeParser = TreeParserVisitor();
		static inline std::unique_ptr<std::vector<FS::Metainfo*>> nodes = std::unique_ptr<std::vector<FS::Metainfo*>>(new std::vector<FS::Metainfo*>());
		
	};
	
	template<typename T, typename ResultT>
	decltype(auto) Get()
	{
		for (auto it = Repository::visitors.begin(); it != Repository::visitors.end(); it++)
		{
			Logger::Log<Info>("ID: ", Bank::Raiba<0>::Identifier);
			if(it->second.Identifier() == T::Identifier)
			{
				Logger::Log<Info>("FOUND ID: ", it->second.Identifier());
				auto p = it->second.Get("");
				std::cout<<"GET OUT"<<*p<<std::endl;
				auto cv = Cast::static_unique_ptr<ResultT>(std::move(p)); 
				std::cout<<"GET OUT"<<*(cv->value)<<std::endl;
				
				//~ return cv;
			}
		}
	};
}

namespace Backup
{
	struct Repository
	{
		using FileTypes = Typelist<FS::XLSX,FS::XLS,FS::JPG,FS::TXT,FS::ODS,FS::ZIP,FS::DOCX,FS::HPP,FS::H,FS::CSV,FS::CPP,FS::PDF>::Type;
		using TypeContainer = FS::FileTypeContainer<FileTypes>;
	
		static Repository& Instance()
		{
			static Repository instance;
			return instance;
		}
		
		void Backup(std::string from, std::string to)
		{
			Repository::Root = from;
			Repository::Dest = to;
			
			auto root = fs::directory_entry(from);
			auto di =  std::make_unique<FS::DirectoryInfo>(root.path(),root.last_write_time());
			auto newDi = FileSystem::GetInfos(std::move(di));
			
			auto vec =  std::make_unique<std::vector<const FS::Metainfo*>>();
			auto result = newDi->GetNodes(std::move(vec));

			for(auto r : *result)
				std::cout<<r->GetInfo()<<std::endl;
			
			Backup::Repository::Map(result->cbegin(), result->cend());
			typeContainer.Display(std::cout);
			FileSystem::CreateDirectories(directories->cbegin(), directories->cend(),from,to);
			Backup::Repository::CopyTo(to);
		}
		
		template<typename Iterator>
		void Map(const Iterator& begin, const Iterator& end)
		{
			for(Iterator it = begin; it != end; ++it)
			{
				(*it)->AcceptConst(treeParser);

				Logger::Log<Info>("Map: ", (*it)->Name());
				if(FS::FileInfo* fi = dynamic_cast<FS::FileInfo*>(const_cast<FS::Metainfo*>(*it)))
					typeContainer.Add(fi); 
				if(FS::DirectoryInfo* di = dynamic_cast<FS::DirectoryInfo*>(const_cast<FS::Metainfo*>(*it)))
					directories->push_back(std::make_unique<FS::Directory>(di));
			}
			Logger::Log<Info>("Directories: ", directories->size());
		}
		
		void CopyTo(std::string dest)
		{
			typeContainer.SetRootPath(Repository::Root);
			typeContainer.CopyTo(dest);
		}
		
		void List()	{ typeContainer.List();	}
		
		void SetRootPath(std::string s) { Root = s; }
		void SetDestPath(std::string s) { Dest = s; }
		std::vector<std::string> Read(std::string s)	{	return typeContainer.Read(s); }
		
		template<typename ParseType>
		typename ParseType::ParseCont Parse(std::string s){	return typeContainer.Parse<ParseType>(s); }
	private:
		Repository()	
		{ 
			Logger::Log<Info>("Repository constructor");
		};
		
		~Repository()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Repository& operator=(const Repository&) = delete;
		Repository(const Repository& c) = delete;
	
		inline static std::unique_ptr<std::vector<std::unique_ptr<FS::Directory>>> directories = std::make_unique<std::vector<std::unique_ptr<FS::Directory>>>();
		static inline TypeContainer typeContainer = TypeContainer();
		inline static std::string Root = ""; 
		inline static std::string Dest = ""; 
		
		class TreeParserVisitor: 
			public BaseVisitor,
			public ConstVisitor<FS::DirectoryInfo>,
			public ConstVisitor<FS::FileInfo>
		{
		public:
			virtual void Visit(const FS::DirectoryInfo& di) { Logger::Log<Info>("Visit Directory");	 };
			virtual void Visit(const FS::FileInfo& fi) { Logger::Log<Info>("Visit File"); };
		};
		
		static inline TreeParserVisitor treeParser = TreeParserVisitor();	
	};
}

#endif
