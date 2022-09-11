#include <algorithm>
#include <functional>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <iterator>
#include <vector>
#include <cstdlib>
#include <unordered_map>
#include <boost/mpl/vector.hpp>
#include <boost/filesystem.hpp>
#include <boost/mpl/for_each.hpp>
#include <boost/mpl/placeholders.hpp>
#include <filesystem>
#include "../Logger/Logger.hpp"
#include "../CSV/CSV.hpp"
#include "../Quantity/Quantity.h"
#include "../Typelist/Typelist.h"
#include "../Visitor/Visitor.hpp"
#include "../String/String_.hpp"

#ifndef CODE_HPP
#define CODE_HPP

namespace fs = std::filesystem;

namespace FS
{

	class Code
	{
		
		std::string line;
		uint lineNumber;
	public:
		using ParseType = Code;
		using ParseCont = std::vector<ParseType>;
		
		Code(uint lineNo, std::string l): lineNumber(lineNo), line(l){};
		
		const uint LineNumber() const { return lineNumber; }
		const std::string& Line() const { return line; }
		
		static ParseCont Parse(std::vector<std::string> content)
		{
			uint ctr = 0;
			auto result = ParseCont();
			for(auto l : content)
				result.push_back(ParseType(++ctr, l));
			
			return result;
		}
		
	};
	
	std::ostream& operator<<(std::ostream& out, const Code& c)	{ return out<<c.LineNumber()<<"\t"<<c.Line();	}

}
#endif
