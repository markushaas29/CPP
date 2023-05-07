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
#include "Comdirect.hpp"
#include "AccountContainer.hpp"
#include "Raiffeisenbank.hpp"
#include "Custom.hpp"
#include "Direction.hpp"
#include "RepositoryValue.hpp"
#include "RepositoryObjectVisitor.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.hpp"
#include "../Home/CounterContainer.hpp"
#include "../Home/StageContainer.hpp"
#include "../Common/DateTimes.hpp"
#include "../Common/Configuration.hpp"
#include "../Visitor/Visitor.hpp"


#ifndef REPOSITORY_HPP
#define REPOSITORY_HPP

namespace CSV
{
	
	template<typename T, typename ResultT = typename T::ResultValueType> decltype(auto) Get();
	
	using CommonParsers = Typelist<Bank::Raiffeisenbank<Configuration::Raiffeisenbank::House>, CEHouse, CEHall,CVat,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTHW, Bank::Raiffeisenbank<Configuration::Raiffeisenbank::Private>, StageContainerType>::Type;
	using CounterParsers = Typelist<CEHouse, CEHall,CVat,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTHW>::Type;

	
	template<typename ParserTypes>
	class RepositoryConfiguration
	{
	public:
		using Parsers = ParserTypes;
	};

	class CommonRepositoryConfiguration: public RepositoryConfiguration<CommonParsers>
	{ 
	public:
		static constexpr const char* Path = Configuration::Repository::SourcePath;
	};
	
	class CounterRepositoryConfiguration: public RepositoryConfiguration<CounterParsers>
	{ 
	public:
		static constexpr const char* Path = "/home/markus/Downloads/CSV_TestFiles_2";
	};
	
	template<typename Config>
	class Repository
	{
	public:
		using InputIterator = std::vector<std::string>::const_iterator;
		using FileTypes = Configuration::Repository::FileTypes;
		using TypeContainer = FS::FileTypeContainer<FileTypes>;
		using DirectionType = Bank::Direction;
		using Parsers = Config::Parsers;
		using ParserContainer = FS::FileTypeContainer<Parsers>;
		using VisitorType = FS::RepositoryObjectVisitor<InputIterator>;
		using VisitorContainer = std::map<std::string, VisitorType>;
		static constexpr const char* Path = Config::Path;
	
		void CopyTo(std::string dest)
		{
			types.SetRootPath(Root);
			types.CopyTo(dest);
		}
		
		void List()	{	types.List();	}
		void SetRootPath(std::string s) { Root = s; }
		void SetDestPath(std::string s) { Dest = s; }
		
		void Map(const std::string& path)
		{
			nodes = FileSystem::List(path);
			auto infos = std::shared_ptr<std::vector<std::shared_ptr<FS::Metainfo>>>(new std::vector<std::shared_ptr<FS::Metainfo>>());

			for(auto it = nodes->begin(); it != nodes->end(); ++it )
			{
				std::shared_ptr<FS::Metainfo> m = *it;
				if(auto fi = std::dynamic_pointer_cast<FS::FileInfo>(m))
					types.Add(fi);
				infos->push_back(m);
			}

			auto root = fs::directory_entry(path);
			auto dir = std::make_shared<FS::DirectoryInfo>(root.path(),root.last_write_time(),infos);
			types.List();
			nodes->push_back(dir);
		}
		
		void Register()
		{
			parsers.RegisterTo(visitors);	
			std::for_each(visitors.cbegin(), visitors.cend(), [&](const auto& kv) {Logger::Log<Info>("Register:",kv.first);});
		}
		
		void ParseAll()
		{
			for (auto it = visitors.begin(); it != visitors.end(); it++)
			{
				for(auto itNode = nodes->cbegin(); itNode != nodes->cend(); ++itNode )
				{
					if((it->second.IsVisitorOf( (*itNode)->Name() )))
					{
						auto f = FS::File( std::dynamic_pointer_cast<FS::FileInfo>(*itNode));
						types.Get(std::dynamic_pointer_cast<FS::FileInfo>(*itNode));
						auto lines = f.Read();	
						it->second.Parse(lines.cbegin(), lines.cend());
						it->second.Update(lines.cbegin(), lines.cend());
					}
				}			
			}
		}
		
		void Write()
		{
			std::ofstream file("/home/markus/Dokumente/cpp/Home/Test/TestFiles/Counter.txt");
			for (auto it = visitors.begin(); it != visitors.end(); it++)
			{
				for(auto itNode = nodes->cbegin(); itNode != nodes->cend(); ++itNode )
				{
					if((it->second.IsVisitorOf( (*itNode)->Name() )))
					{
						Logger::Log<Info>("Repository Write: ",it->first);
						if (auto d = std::dynamic_pointer_cast<FS::FileInfo>(*itNode); d != nullptr)
						{
							auto fi = FS::File(std::shared_ptr<FS::FileInfo>(d));
							
							it->second.Print(std::cout);
							it->second.Print(*(fi.Handle()));
							fi.Write();
						}
						//it->second.Update(lines.cbegin(), lines.cend());
					}
				}			
			}
		}

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
			Map(std::string(Path));
			Register();
			ParseAll();	
		};
		
		~Repository()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
		Repository& operator=(const Repository&) = delete;
		Repository(const Repository& c) = delete;
	
		template<typename T, typename ResultT,typename P>
		friend ResultT Get();
		
		static inline VisitorContainer visitors = VisitorContainer();
		static inline TypeContainer types = TypeContainer();
		static inline ParserContainer parsers = ParserContainer();
		inline static std::string Root = ""; 
		inline static std::string Dest = ""; 
		static inline std::unique_ptr<std::vector<std::shared_ptr<FS::Metainfo>>> nodes = std::unique_ptr<std::vector<std::shared_ptr<FS::Metainfo>>>(new std::vector<std::shared_ptr<FS::Metainfo>>());
		
	};
	
	template<typename T, typename ResultT, typename P>
	ResultT Get()
	{
		for (auto it = Repository<P>::visitors.begin(); it != Repository<P>::visitors.end(); it++)
		{
			Logger::Log<Info>("ID: ", Bank::Raiffeisenbank<Configuration::Raiffeisenbank>::Identifier);
			if(it->second.Identifier() == T::Identifier)
			{
				Logger::Log<Info>("FOUND ID: ", it->second.Identifier());
				//~ auto p = it->second.Get("");
				//~ std::cout<<"GET OUT"<<*p<<std::endl;
				return Cast::static_unique_ptr<ResultT>(std::move(it->second.Get(""))); 
				//~ std::cout<<"GET OUT"<<*(cv)<<std::endl;
				
				//~ return cv;:
			}
			
			return ResultT{};
		}
	};
}

namespace Backup
{
//	struct Repository
//	{
//		using FileTypes = Configuration::Backup::FileTypes;
//		using TypeContainer = FS::FileTypeContainer<FileTypes>;
//	
//		static Repository& Instance()
//		{
//			static Repository instance;
//			return instance;
//		}
//		
//		void Backup(std::string from, std::string to)
//		{
//			Repository::Root = from;
//			Repository::Dest = to;
//			
//			auto root = fs::directory_entry(from);
//			auto di =  std::make_unique<FS::DirectoryInfo>(root.path(),root.last_write_time());
//			auto newDi = FileSystem::GetInfos(std::(di));
//			
//			auto vec =  std::make_unique<std::vector<std::unique_ptr<FS::Metainfo>>>();
//			auto result = newDi->GetNodes(std::(vec));
//
////			for(auto r : *result)
////				std::cout<<r->GetInfo()<<std::endl;
//			
//			Backup::Repository::Map(result->cbegin(), result->cend());
//			typeContainer.Display(std::cout);
//			FileSystem::CreateDirectories(directories->cbegin(), directories->cend(),from,to);
//			Backup::Repository::CopyTo(to);
//		}
//		
//		template<typename Iterator>
//		void Map(const Iterator& begin, const Iterator& end)
//		{
//			for(Iterator it = begin; it != end; ++it)
//			{
//				(*it)->AcceptConst(treeParser);
//
//				Logger::Log<Info>("Map: ", (*it)->Name());
//				if(FS::FileInfo* fi = dynamic_cast<FS::FileInfo*>(const_cast<FS::Metainfo*>(*it)))
//					typeContainer.Add(fi); 
//				if(FS::DirectoryInfo* di = dynamic_cast<FS::DirectoryInfo*>(const_cast<FS::Metainfo*>(*it)))
//					directories->push_back(std::make_unique<FS::Directory>(di));
//			}
//			Logger::Log<Info>("Directories: ", directories->size());
//		}
//		
//		void CopyTo(std::string dest)
//		{
//			typeContainer.SetRootPath(Repository::Root);
//			typeContainer.CopyTo(dest);
//		}
//		
//		void List()	{ typeContainer.List();	}
//		
//		void SetRootPath(std::string s) { Root = s; }
//		void SetDestPath(std::string s) { Dest = s; }
//		std::vector<std::string> Read(std::string s)	{	return typeContainer.Read(s); }
//		
//		template<typename ParseType>
//		typename ParseType::ParseCont Parse(std::string s){	return typeContainer.Parse<ParseType>(s); }
//	private:
//		Repository()	
//		{ 
//			Logger::Log<Info>("Repository constructor");
//		};
//		
//		~Repository()	{ /*Logger::Log()<<"Destructor"<<std::endl;*/ }
//		Repository& operator=(const Repository&) = delete;
//		Repository(const Repository& c) = delete;
//	
//		inline static std::unique_ptr<std::vector<std::unique_ptr<FS::Directory>>> directories = std::make_unique<std::vector<std::unique_ptr<FS::Directory>>>();
//		static inline TypeContainer typeContainer = TypeContainer();
//		inline static std::string Root = ""; 
//		inline static std::string Dest = ""; 
//		
//		class TreeParserVisitor: 
//			public BaseVisitor,
//			public ConstVisitor<FS::DirectoryInfo>,
//			public ConstVisitor<FS::FileInfo>
//		{
//		public:
//			virtual void Visit(const FS::DirectoryInfo& di) { Logger::Log<Info>("Visit Directory");	 };
//			virtual void Visit(const FS::FileInfo& fi) { Logger::Log<Info>("Visit File"); };
//		};
//		
//		static inline TreeParserVisitor treeParser = TreeParserVisitor();	
//	};
}

#endif
