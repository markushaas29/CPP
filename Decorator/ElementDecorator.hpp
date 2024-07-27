#include "../ObjectFactory/FactoryUnit.hpp" 
#include "../String/Literal.hpp" 
#include "../Is/Is.hpp" 
#include "../CSV/HtmlElement.hpp" 
#include "../CSV/Elements.hpp" 

#pragma once

class IElementDecorator
{
public:
	auto Decorate() const  { return ""; };
private:
	//friend std::ostream& operator<<(std::ostream& s, const IDecorator& c) { return c.display(s); }
};

class TagDecorator: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "TagDecorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
};

struct B: public TagDecorator{	virtual std::string decorate() const { return "b"; } };
struct Td: public TagDecorator				{	inline static constexpr const char* Identifier= "td"; };
struct Tr: public TagDecorator				{	inline static constexpr const char* Identifier= "tr"; };
struct Th: public TagDecorator{	virtual std::string decorate() const { return "th"; } };
struct Section: public TagDecorator			{	inline static constexpr const char* Identifier= "section"; 	};
struct Table: public TagDecorator			{	inline static constexpr const char* Identifier= "table"; 	};
struct Html: public TagDecorator{	virtual std::string decorate() const { return "html"; } };
struct head: public TagDecorator{	virtual std::string decorate() const { return "head"; } };

class StyleDecorator: public IElementDecorator
{
	inline static constexpr const char TypeIdentifier[] = "HtmlDecorator";
    inline static constexpr Literal TypeId{TypeIdentifier};
    template<typename U> using IsT =  Is<U,TypeId>;
public:
//	auto operator()(const IHtmlElement& e) const { return HtmlElement<Entry>(e.Content(), e.Tag(),Decorate()); }
};

//class Red: public StyleDecorator{	virtual std::string decorate() const { return "style=\"color:red;\""; } };
//class Green: public StyleDecorator{	virtual std::string decorate() const { return "style=\"color:green;\""; } };


//template<typename Y, typename N, typename Ptr=StyleDecorator>
//class DecoratorDecider
//{
//	inline static constexpr const char TypeIdentifier[] = "HtmlDecorator";
//    inline static constexpr Literal TypeId{TypeIdentifier};
//    template<typename U> using IsT =  Is<U,TypeId>;
//public:
//	HtmlElement operator()(bool is, HtmlElement& e) const 
//	{
//		if(is)
//			return Y()(e); 
//		return N()(e); 
//	}
//};
