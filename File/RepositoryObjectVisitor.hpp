#include <algorithm>
#include <functional>
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"

#ifndef REPOSITORYOBJECTVISITOR_HPP
#define REPOSITORYOBJECTVISITOR_HPP

namespace fs = std::filesystem;

namespace FS
{
	template<typename Iterator = std::vector<std::string>::const_iterator>
	class RepositoryObjectVisitor
	{
		using IsVisitorFuncType = std::function<bool(const std::string&, const std::string&)>;
		using ParseFuncType = std::function<void(Iterator,Iterator)>;
	public:
		RepositoryObjectVisitor() = delete;
		RepositoryObjectVisitor(std::string n, ParseFuncType p, IsVisitorFuncType i): name{n}, parse{p}, isVisitorOf{i} {};
		RepositoryObjectVisitor(std::string n, ParseFuncType p): name{n}, parse{p}{};
		std::string Name() { return this->name;}
		void Parse(Iterator b, Iterator e) { this->parse(b,e); }
		bool IsVisitorOf(const std::string& s) { return this->isVisitorOf(s,this->name); }
	private:
		std::string name;
		ParseFuncType parse;
		IsVisitorFuncType isVisitorOf = [&](const std::string& s, const std::string& n){ return String_::Contains(s,n); };
	};
	
	//~ std::ostream& operator<<(std::ostream& out, const RepositoryObjectVisitor& c)	{ return out;	}

}
#endif
