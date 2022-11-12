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
	using InputFuncType = std::function<std::unique_ptr<std::ofstream>(std::unique_ptr<std::ofstream>)>;
	InputVisitor() = delete;
	InputVisitor(const std::string& n, AddFuncType a, InputFuncType i): name{n}, identifier{n},add{a}, input{i} {};
			
	const std::string& Name() { return this->name;}
	const std::string& Identifier() { return this->identifier;}
		
	void Add(Iterator b, Iterator e) { Logger::Log("RepositoryObjectVisitor: Add", name); add(b,e); }
	std::unique_ptr<std::ofstream> Input(std::unique_ptr<std::ofstream> of) { return input(std::move(of)); }
private:
	const std::string name;
	const std::string identifier;
	AddFuncType add;
	InputFuncType input;
};
#endif
