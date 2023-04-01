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
	
	template<typename Derived, typename DerivedInfo>
	struct Node
	{
		using Type = Derived;
		using PtrType = std::shared_ptr<DerivedInfo>;
		using ContainerType = std::vector<Type>; 
	
		Node(PtrType fi): info(fi){};
		std::ostream& Display(std::ostream& os)	{ return os;	}
		const std::string& Path() const { return info->Path(); };
		const std::string& Name() const { return info->Name(); };
		std::shared_ptr<DerivedInfo> Info() const { return info; };
		bool BelongsTo(const fs::path& p) const
		{  
			auto path = fs::path(info->Path());
			auto pIt = path.begin();
			for(auto sp = p.begin(); sp != p.end(); ++sp, ++pIt)
			{
				if(*sp != *pIt)
					return false;
			}
				
			return true;
		};
	protected:
		std::shared_ptr<DerivedInfo> info;
	};
	
	struct Directory: Node<Directory, DirectoryInfo>{	Directory(std::shared_ptr<DirectoryInfo> di): Node(di){};	};
	
	class File: public Node<File, FileInfo>
	{
	public:
		File(std::shared_ptr<FileInfo> fi): Node(fi){};
		void CopyTo(std::string destinationName) const 
		{ 
			try
			{
				auto srcName = fs::path(info->Path()).parent_path().string() +"/"+ info->Name();
				fs::copy(srcName, fs::path(destinationName));
				Logger::Log<::Info>(srcName," copied.");
			}
			catch (const std::exception& e)
			{
				Logger::Log<Error>("File", info->Name(), "could not copied!");
				Logger::Log<Error>("Reason", e.what());
			} 
		};
	
		
		std::vector<std::string> Read() const {	return ReadLines(info->Path());};
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
	struct FileTypeBase: File
	{
		using Base = File;
		using FileType = FileT;
		using ParsedType = std::string;
		using ParserContainer = std::vector<ParsedType>;
		static constexpr const char* Extension = "";		
		FileTypeBase(std::shared_ptr<FileInfo> fi): Base(fi){};

		template<typename Separator = T::char_<';'>>
		static std::vector<std::string> ExtractValues(std::string line)	{		return String_::Split<Separator>(line);	};
	};
	
	struct CTRV: public FileTypeBase<CTRV>	{		static constexpr const char* Extension = ".ctrv";	};
	struct KEYS: public FileTypeBase<KEYS>	{		static constexpr const char* Extension = ".keys";	};
	struct CPP: public FileTypeBase<CPP>	{		static constexpr const char* Extension = ".cpp";	};
	struct PDF: public FileTypeBase<PDF>	{		static constexpr const char* Extension = ".pdf";	};
	struct JPG: public FileTypeBase<JPG>	{		static constexpr const char* Extension = ".jpg";	};
	struct ZIP: public FileTypeBase<ZIP>	{		static constexpr const char* Extension = ".zip";	};
	struct ODS: public FileTypeBase<ODS>	{		static constexpr const char* Extension = ".ods";	};
	struct DOCX: public FileTypeBase<DOCX>	{		static constexpr const char* Extension = ".docx";	};
	struct XLSX: public FileTypeBase<XLSX>	{		static constexpr const char* Extension = ".xlsx";	};
	struct XLS: public FileTypeBase<XLS>	{		static constexpr const char* Extension = ".xls";	};
	struct TXT: public FileTypeBase<TXT>	{		static constexpr const char* Extension = ".txt";	};
	struct HPP: public FileTypeBase<HPP>	{		static constexpr const char* Extension = ".hpp";	};
	struct H: public FileTypeBase<H>	{		static constexpr const char* Extension = ".h";	};
	struct INPUT: public FileTypeBase<INPUT>	{		static constexpr const char* Extension = ".input";	};
	struct CSV: public FileTypeBase<CSV>	{		
		static constexpr const char* Extension = ".csv"; 
		CSV(std::shared_ptr<FileInfo> fi): FileTypeBase(fi), destinationPath(info->Path() + CSV::Extension){};

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
}


#endif
