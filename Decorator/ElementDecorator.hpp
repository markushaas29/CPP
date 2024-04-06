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

class HtmlDecorator: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "HtmlDecorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	auto operator()(const HtmlElement& e) const { return HtmlElement(e.Data(), Decorate()); }
};

class StyleDecorator: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "HtmlDecorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
	auto operator()(const HtmlElement& e) const { return HtmlElement(e.Data(), Decorate()); }
};

class B: public HtmlDecorator{	virtual std::string decorate() const { return "b"; } };
class Td: public HtmlDecorator{	virtual std::string decorate() const { return "td"; } };
class Tr: public HtmlDecorator{	virtual std::string decorate() const { return "tr"; } };
class Th: public HtmlDecorator{	virtual std::string decorate() const { return "th"; } };
class Table: public HtmlDecorator{	virtual std::string decorate() const { return "table"; } };
class Html: public HtmlDecorator{	virtual std::string decorate() const { return "html"; } };
