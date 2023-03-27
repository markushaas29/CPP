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
#include "Info.hpp"
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Visitor/Visitor.hpp"

#ifndef NODE_HPP
#define NODE_HPP

namespace fs = std::filesystem;

namespace FS
{
	static std::vector<std::string> ReadLines(const std::string& path)
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
	
	static void WriteLines(const std::string& path, const std::vector<std::string>& lines)
	{
		std::ofstream ofs(path);
		if (ofs.is_open())
		{
			for(auto line : lines)
				ofs<<line<<std::endl;
			
			ofs.close();
		}
	}
	
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
		static void Add(Metainfo* fi){ elements.push_back(ElementType(static_cast<DerivedInfo*>(fi))); 	};
		static ElementType Get(Metainfo* fi){return ElementType();};
		static const ContainerType& Nodes() { return elements; };
		const DerivedInfo& Info() const {return info;}
		
		static void Display(std::ostream& os)
		{
			os<<Type::Extension<<std::endl;
			for(auto e : elements)
				os<<"-"<<e.Info()<<std::endl;
		}
		
		bool BelongsTo(const fs::path& p) const
		{  
			auto path = fs::path(this->info.Path());
			auto pIt = path.begin();

			for(auto sp = p.begin(); sp != p.end(); ++sp, ++pIt)
			{
				if(*sp != *pIt)
					return false;
			}
				
			return true;
		};
	};
	
	struct Directory: Node<Directory, DirectoryInfo>{	Directory(DirectoryInfo * di): Node(di){};	};
	
	class File: public Node<File, FileInfo>
	{
	public:
		File(FileInfo* fi): Node(fi){};
		void CopyTo(std::string destinationName) const 
		{ 
			try
			{
				auto srcName = fs::path(this->info.Path()).parent_path().string() +"/"+ this->info.Name();
				fs::copy(srcName, fs::path(destinationName));
				Logger::Log<::Info>(srcName," copied.");
			}
			catch (const std::exception& e)
			{
				Logger::Log<Error>("File", this->info.Name(), "could not copied!");
				Logger::Log<Error>("Reason", e.what());
			} 
		};
	
		
		std::vector<std::string> Read() const {	return ReadLines(this->info.Path());};
		
		//~ template<typename It>
		//~ void Write(It begin, It end)	{ std::cout<<"WRITE NODE"<<Sstd::endl;	};
		void Write()	{ std::cout<<"WRITE NODE"<<std::endl;	};

		std::unique_ptr<std::ofstream> Handle() { return std::make_unique<std::ofstream>("/home/markus/Dokumente/cpp/Home/Test/TestFiles/NodeTest.txt");}

		template<typename ParseType, typename ParseTypeContainer = ParseType::ParseCont>
		ParseTypeContainer Parse() const
		{
			auto content = this->Read();
			ParseType::Parse(content);
			
			return ParseType::Transactions;
		}
	private:
		friend  File& operator<<(File& out, const auto& s){   std::cout<<"<<"<<std::endl; return out;  }
	};

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
	struct XLSX: public FileTypeBase<XLSX>{};
	struct XLS: public FileTypeBase<XLS>{};
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
			auto ofs = std::make_unique<std::ofstream>(destinationPath); 
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
	template<> const char* FileTypeBase<XLSX>::Extension = ".xlsx";
	template<> const char* FileTypeBase<XLS>::Extension = ".xls";
	template<> const char* FileTypeBase<TXT>::Extension = ".txt";
	template<> const char* FileTypeBase<H>::Extension = ".h";
	template<> const char* FileTypeBase<CSV>::Extension = ".csv";
	template<> const char* FileTypeBase<INPUT>::Extension = ".input";
}


#endif
