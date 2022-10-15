#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <filesystem>
#include "Info.hpp"
#include "../Logger/Logger.hpp"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"

#ifndef NODE_HPP
#define NODE_HPP

namespace fs = std::filesystem;

namespace FS
{

	static std::vector<std::string> ReadLines(std::string path)
	{
		std::string line;
		auto result = std::vector<std::string>();			
		
		std::ifstream ifs (path);
		if (ifs.is_open())
		{
			while ( getline (ifs,line) ){	result.push_back(line);		}
			ifs.close();
		}
		
		return result;
	}
	
	static void WriteLines(std::string path, std::vector<std::string> lines)
	{
		std::ofstream ofs(path);
		if (ofs.is_open())
		{
			for(auto line : lines)
				ofs<<line<<std::endl;
			
			ofs.close();
		}
	}
	
	//---------------------------------------------------------------------------------------------------Node----------------------------------------------------------------------------------------
	template<typename Derived, typename DerivedInfo,typename T = Derived>
	struct Node
	{
		using Type = Derived;
		using ElementType = T;
		using ContainerType = std::vector<ElementType>;
	protected:
		static inline ContainerType elements = ContainerType();
		const DerivedInfo info;
	public:		
		Node(DerivedInfo* fi): info(*fi){};
		
		static void Add(Info* fi)
		{ 
			elements.push_back(ElementType(static_cast<DerivedInfo*>(fi))); 
		};
		static ElementType Get(Info* fi){return ElementType();};
		static const ContainerType& Nodes() { return elements; };
		
		static void Display(std::ostream& os)
		{
			os<<Type::Extension<<std::endl;
			for(auto e : elements)
				os<<"-"<<e.Info()<<std::endl;
		}
		
		const DerivedInfo& Info() const {return info;}
		bool BelongsTo(const fs::path& p) const
		{  
			auto pIt = fs::path(this->info.Path()).begin();
			
			for(auto sp = p.begin(); sp != p.end(); ++sp, ++pIt)
			{
				if(*sp != *pIt)
					return false;
			}
				
			return true;
		};
	};
	
	//---------------------------------------------------------------------------------------------------Directory----------------------------------------------------------------------------------------

	struct Directory: Node<Directory, DirectoryInfo>{	Directory(DirectoryInfo * di): Node(di){};	};
	
	//---------------------------------------------------------------------------------------------------File----------------------------------------------------------------------------------------

	struct File: Node<File, FileInfo>
	{
		File(FileInfo* fi): Node(fi){};
		void CopyTo(std::string destinationName) const 
		{ 
			auto srcName = fs::path(this->info.Path()).parent_path().string() +"/"+ this->info.Name();
			fs::copy(srcName, fs::path(destinationName));
		};
	
		
		std::vector<std::string> Read() const {	return FS::ReadLines(this->info.Path());};
		void Write(std::vector<std::string> lines)	{	};
		
		template<typename ParseType, typename ParseTypeContainer = ParseType::ParseCont>
		ParseTypeContainer Parse() const
		{
			auto content = this->Read();
			
			ParseType::Parse(content);
			
			return ParseType::Transactions;
		}
	};

	//---------------------------------------------------------------------------------------------------FileTypes----------------------------------------------------------------------------------------

	template<typename FileT>
	struct FileTypeBase: Node<FileTypeBase<FileT>, FileInfo, File>
	{
		FileTypeBase(FileInfo* fi): Node<FileTypeBase<FileT>, FileInfo, File>(fi){};
		using ParsedType = std::string;
		using ParserContainer = std::vector<ParsedType>;
		static const char* Extension;		
		
		template<typename Separator = T::char_<';'>>
		static std::vector<std::string> ExtractValues(std::string line)	{		return String_::Split<Separator>(line);	};
	};
	
	struct CTRV: public FileTypeBase<CTRV>{};
	struct KEYS: public FileTypeBase<KEYS>{};
	struct CPP: public FileTypeBase<CPP>{};
	struct PDF: public FileTypeBase<PDF>{};
	struct JPG: public FileTypeBase<JPG>{};
	struct ZIP: public FileTypeBase<ZIP>{};
	struct ODS: public FileTypeBase<ODS>{};
	struct DOCX: public FileTypeBase<DOCX>{};
	struct TXT: public FileTypeBase<TXT>{};
	struct HPP: public FileTypeBase<HPP>{};
	struct H: public FileTypeBase<H>{};
	struct INPUT: public FileTypeBase<INPUT>{};
	
	struct CSV: public FileTypeBase<CSV>
	{
		CSV(FileInfo* fi): FileTypeBase(fi), destinationPath(this->Info().Path() + CSV::Extension){};

		template<typename Ctr, typename Separator = T::char_<';'>>
		void Write()
		{
			auto ofs = std::unique_ptr<std::ofstream>(new std::ofstream(destinationPath)); 
			Ctr::template Display<Separator>(*ofs);
			ofs->close();
		}
		
		const std::string& GetDestinationPath() const { return this->destinationPath; };
	private:
		const std::string destinationPath;
	};

	
	template<> const char* FileTypeBase<CTRV>::Extension = ".ctrv";
	template<> const char* FileTypeBase<KEYS>::Extension = ".keys";
	template<> const char* FileTypeBase<CPP>::Extension = ".cpp";
	template<> const char* FileTypeBase<HPP>::Extension = ".hpp";
	template<> const char* FileTypeBase<PDF>::Extension = ".pdf";
	template<> const char* FileTypeBase<JPG>::Extension = ".jpg";
	template<> const char* FileTypeBase<ZIP>::Extension = ".zip";
	template<> const char* FileTypeBase<ODS>::Extension = ".ods";
	template<> const char* FileTypeBase<DOCX>::Extension = ".docx";
	template<> const char* FileTypeBase<TXT>::Extension = ".txt";
	template<> const char* FileTypeBase<H>::Extension = ".h";
	template<> const char* FileTypeBase<CSV>::Extension = ".csv";
	template<> const char* FileTypeBase<INPUT>::Extension = ".input";
}


#endif
