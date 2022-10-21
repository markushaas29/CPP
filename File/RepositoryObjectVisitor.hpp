#include <algorithm>
#include <functional>
#include <memory>
#include "RepositoryValue.hpp"
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
		using UpdateFuncType = std::function<bool(Iterator,Iterator)>;
		using GetFuncType = std::function<std::unique_ptr<RepositoryValue>(const std::string&)>;
	public:
		RepositoryObjectVisitor() = delete;
		RepositoryObjectVisitor(std::string n, ParseFuncType p, IsVisitorFuncType i): name{n}, parse{p}, isVisitorOf{i} {};
		RepositoryObjectVisitor(std::string n, ParseFuncType p): name{n}, parse{p}{};
		RepositoryObjectVisitor(std::string n, ParseFuncType p, GetFuncType g, UpdateFuncType u = [](Iterator b, Iterator e){ return true; } ): name{n}, parse{p}, get{g}, update{u}{};
		std::string Name() { return this->name;}
		void Parse(Iterator b, Iterator e) { Logger::Log("RepositoryObjectVisitor: Parse", name);this->parse(b,e); }
		bool Update(Iterator b, Iterator e) { Logger::Log("RepositoryObjectVisitor: Update", name); return update(b,e); }
		bool IsVisitorOf(const std::string& s) { return this->isVisitorOf(s,this->name); }
		std::unique_ptr<RepositoryValue> Get(const std::string& s) { return get(s); }
	private:
		std::string name;
		ParseFuncType parse;
		UpdateFuncType update;
		GetFuncType get = [&](const std::string& s){ Logger::Log("RepositoryObjectVisitor GET", name); };
		IsVisitorFuncType isVisitorOf = [&](const std::string& s, const std::string& n){ return String_::Contains(s,n); };
	};
}
#endif
