#include <algorithm>
#include <functional>
#include <memory>
#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"

#ifndef INPUTVISITOR_HPP
#define INPUTVISITOR_HPP
class InputVisitor
{
public:
	using Iterator = std::vector<std::string>::const_iterator;
	using AddFuncType = std::function<void(Iterator,Iterator)>;
	InputVisitor() = delete;
	InputVisitor(const std::string& n, AddFuncType a): name{n}, identifier{n},add{a} {};
		
	const std::string& Name() { return this->name;}
	const std::string& Identifier() { return this->identifier;}
		
	void Add(Iterator b, Iterator e) { Logger::Log("RepositoryObjectVisitor: Add", name);this->add(b,e); }
private:
	const std::string name;
	const std::string identifier;
	AddFuncType add;
};
#endif
