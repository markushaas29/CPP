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
#include "Account.hpp"
#include "Comdirect.hpp"
#include "AccountContainer.hpp"
#include "Raiba.hpp"
#include "Custom.hpp"
#include "Direction.hpp"
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
	
	struct Repository
	{
		using InputIterator = std::vector<std::string>::const_iterator;
		using FileTypes = Typelist<FS::INPUT,FS::KEYS,FS::CPP, FS::HPP, FS::CTRV,FS::CSV>::Type;
		using TypeContainer = FS::FileTypeContainer<FileTypes>;
		using DirectionType = Bank::Direction;
		using ParseTypes = Typelist<InputManager<int>,CE1,CVat,CG1,CWA,CWO,CWOut, CBCW,CBHW, CMCW,CMHW,CTCW,CTHW, Bank::Custom<0>, Bank::Raiba<0>, Bank::Comdirect<0>,StageContainerType>::Type;
		using ParseTypeContainer = FS::FileTypeContainer<ParseTypes>;
		using ParseMethod = void(*)(InputIterator, InputIterator);
		using ParserContainer = std::map<std::string, ParseMethod>;
	
		template<typename Iterator>
		static void Map(const Iterator& begin, const Iterator& end)
		{
			for(Iterator it = begin; it != end; ++it)
			{
				(*it)->Accept(treeParser);
			}
			
		}
		
		static void CopyTo(std::string dest)
		{
			typeContainer.SetRootPath(Repository::Root);
			typeContainer.CopyTo(dest);
		}
		
		static void List()
		{
			typeContainer.List();
		}
		
		static void SetRootPath(std::string s) { Root = s; }
		static void SetDestPath(std::string s) { Dest = s; }
		
		static void Map(std::string path)
		{
			auto infos = FileSystem::List(path);
			
			for(auto i : infos)
				CSV::Repository::nodes->push_back(i);

			auto root = fs::directory_entry(path);
			auto dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),infos);
			
			CSV::Repository::nodes->push_back(dir);
			CSV::Repository::Map(CSV::Repository::nodes->cbegin(), CSV::Repository::nodes->cend());
		}
		
		static std::vector<std::string> Read(std::string s)
		{
			return typeContainer.Read(s);			
		}
		
		static void Register()
		{
			parseTypeContainer.RegisterTo(Repository::parseContainer);	
			
			for(auto kv : parseContainer)
				Logger::Log()<<kv.first<<std::endl;
		}
		
		template<typename ParseType>
		static typename ParseType::ParseCont Parse(std::string s)
		{
			Logger::Log()<<s<<std::endl;
			return typeContainer.Parse<ParseType>(s);			
		}
		
		static void ParseAll()
		{
			for (auto it = parseContainer.begin(); it != parseContainer.end(); it++)
			{
				for(auto itNode = CSV::Repository::nodes->cbegin(); itNode != CSV::Repository::nodes->cend(); ++itNode )
				{
					if((String_::Contains((*itNode)->Name(),it->first)))
					{
					Logger::Log()<<it->first<<"\t"<<(*itNode)->Path()<<std::endl;
						auto lines = CSV::Repository::Read((*itNode)->Name());	
						it->second(lines.cbegin(), lines.cend());
						
					}
				}			
				
			}
		}
		
		static void Display(std::ostream& os)
		{
			parseTypeContainer.Display(os);
		}
		
	private:
		static inline ParserContainer parseContainer = ParserContainer();
		static inline TypeContainer typeContainer = TypeContainer();
		static inline ParseTypeContainer parseTypeContainer = ParseTypeContainer();
		inline static std::string Root = ""; 
		inline static std::string Dest = ""; 
				
		class TreeParserVisitor: 
			public BaseVisitor,
			public Visitor<FS::DirectoryInfo>,
			public Visitor<FS::FileInfo>
		{
		public:
			virtual void Visit(FS::DirectoryInfo& di) {	FS::Directory::Add(&di); };
			virtual void Visit(FS::FileInfo& fi) { typeContainer.Add(&fi); };
		};	
		
		static inline TreeParserVisitor treeParser = TreeParserVisitor();
		static inline std::unique_ptr<std::vector<FS::Info*>> nodes = std::unique_ptr<std::vector<FS::Info*>>(new std::vector<FS::Info*>());
		
	};
}

namespace Backup
{
	struct Repository
	{
		using FileTypes = Typelist<FS::HPP,FS::H,FS::CSV,FS::CPP>::Type;
		using TypeContainer = FS::FileTypeContainer<FileTypes>;
	
		template<typename Iterator>
		static void Map(const Iterator& begin, const Iterator& end)
		{
			for(Iterator it = begin; it != end; ++it)
			{
				(*it)->Accept(treeParser);
			}
			
		}
		
		static void CopyTo(std::string dest)
		{
			typeContainer.SetRootPath(Repository::Root);
			typeContainer.CopyTo(dest);
		}
		
		static void List()
		{
			typeContainer.List();
		}
		
		static void SetRootPath(std::string s) { Root = s; }
		static void SetDestPath(std::string s) { Dest = s; }
		
		static void Backup(std::string from, std::string to)
		{
			Repository::Root = from;
			Repository::Dest = to;
			
			auto nodes = FileSystem::List(from);

			auto root = fs::directory_entry(from);
			auto dir = new FS::DirectoryInfo(root.path(),root.last_write_time(),nodes);
			
			nodes.push_back(dir);
			Backup::Repository::Map(nodes.cbegin(), nodes.cend());
			
			FileSystem::CreateDirectories(from,to);
			Backup::Repository::List();
			Backup::Repository::CopyTo(to);

// 			FileSystem::List(to);
		}
		
		static std::vector<std::string> Read(std::string s)
		{
			return typeContainer.Read(s);			
		}
		
		template<typename ParseType>
		static typename ParseType::ParseCont Parse(std::string s)
		{
			return typeContainer.Parse<ParseType>(s);			
		}
		
	private:
		static inline TypeContainer typeContainer = TypeContainer();
		inline static std::string Root = ""; 
		inline static std::string Dest = ""; 
				
		class TreeParserVisitor: 
			public BaseVisitor,
			public Visitor<FS::DirectoryInfo>,
			public Visitor<FS::FileInfo>
		{
		public:
			virtual void Visit(FS::DirectoryInfo& di) {	FS::Directory::Add(&di); };
			virtual void Visit(FS::FileInfo& fi) { typeContainer.Add(&fi); };
		};	
		
		static inline TreeParserVisitor treeParser = TreeParserVisitor();
	};
}

#endif
