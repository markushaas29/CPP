#include <algorithm>
#include <functional>
#include "../String/String_.hpp"

#ifndef REPOSITORYOBJECTVISITOR_HPP
#define REPOSITORYOBJECTVISITOR_HPP

namespace fs = std::filesystem;

namespace FS
{
	template<typename Iterator = std::vector<std::string>::const_iterator>
	class RepositoryObjectVisitor
	{
		std::string name;
		std::function<bool(std::string)> is;
		std::function<void(Iterator,Iterator)> parse;
	public:
		RepositoryObjectVisitor() = delete;
		RepositoryObjectVisitor(std::string n, std::function<bool(std::string)> is_ = [](const std::string& s){ return String_::Contains(name,s); }): name{n}, is{is_} {};
		std::string Name() { this->is(name) ;return this->name;}
	};
	
	//~ std::ostream& operator<<(std::ostream& out, const RepositoryObjectVisitor& c)	{ return out;	}

}
#endif
