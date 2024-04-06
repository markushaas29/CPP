#include "../ObjectFactory/FactoryUnit.hpp" 
#include "../String/Literal.hpp" 
#include "../Is/Is.hpp" 
#include "../CSV/HtmlElement.hpp" 

#pragma once

class IElementDecorator
{
public:
	auto Decorate() const  { return decorate(); };
private:
	//friend std::ostream& operator<<(std::ostream& s, const IDecorator& c) { return c.display(s); }
	virtual std::string decorate() const  = 0;
};

class TagDecorator: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "TagDecorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	auto operator()(const HtmlElement& e) const { return HtmlElement(e.Data(), Decorate()); }
};

class B: public TagDecorator{	virtual std::string decorate() const { return "b"; } };
class Td: public TagDecorator{	virtual std::string decorate() const { return "td"; } };
class Tr: public TagDecorator{	virtual std::string decorate() const { return "tr"; } };
class Th: public TagDecorator{	virtual std::string decorate() const { return "th"; } };
class Table: public TagDecorator{	virtual std::string decorate() const { return "table"; } };
class Html: public TagDecorator{	virtual std::string decorate() const { return "html"; } };

class StyleDecorator: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "HtmlDecorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	auto operator()(const HtmlElement& e) const { return HtmlElement(e.Data(), e.Tag(),Decorate()); }
};

class Red: public StyleDecorator{	virtual std::string decorate() const { return "style=\"color:red;\""; } };
