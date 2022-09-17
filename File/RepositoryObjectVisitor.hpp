#include <algorithm>
#include <functional>

#ifndef REPOSITORYOBJECTVISITOR_HPP
#define REPOSITORYOBJECTVISITOR_HPP

namespace fs = std::filesystem;

namespace FS
{
	class RepositoryObjectVisitor
	{
		
		std::string name;
		std::function<bool(std::string)> is;
	public:
		RepositoryObjectVisitor() = delete;
		RepositoryObjectVisitor(std::string n, std::function<bool(std::string)> is_): name{n}, is{is_} {};
		std::string Name() { this->is(name) ;return this->name;}
	};
	
	std::ostream& operator<<(std::ostream& out, const RepositoryObjectVisitor& c)	{ return out;	}

}
#endif
