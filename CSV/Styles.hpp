#include "../ObjectFactory/FactoryUnit.hpp" 
#include "../String/Literal.hpp" 
#include "../Is/Is.hpp" 
#include "../CSV/HtmlElement.hpp" 
#include "../CSV/Elements.hpp" 

#pragma once

class TagDecorator
{
	inline static constexpr const char TypeIdentifier[] = "TagDecorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
};

struct B: public TagDecorator				{	virtual std::string decorate() const { return "b"; } };
struct Td: public TagDecorator				{	inline static constexpr const char* Identifier= "td"; };
struct Tr: public TagDecorator				{	inline static constexpr const char* Identifier= "tr"; };
struct Th: public TagDecorator				{	virtual std::string decorate() const { return "th"; } };
struct Section: public TagDecorator			{	inline static constexpr const char* Identifier= "section"; 	};
struct Table: public TagDecorator			{	inline static constexpr const char* Identifier= "table"; 	};
struct DivTag: public TagDecorator				{	inline static constexpr const char* Identifier= "div"; 	};
struct Html: public TagDecorator			{	virtual std::string decorate() const { return "html"; } };
struct head: public TagDecorator			{	virtual std::string decorate() const { return "head"; } };
