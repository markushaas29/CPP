#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include "../Visitor/Visitor.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/IModel.hpp"
#include "../CSV/CssStyle.hpp"
#include "../TypeCounter/TypeCounter.hpp"
#include <string.h>
#include <map>
#include <regex>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

template<typename, bool> class PredicateVisitor;

#pragma once

class IHtmlElement: public IModel
{
public:
	std::string Data() const  { return Out(0); };	
	virtual const std::string& Content() const  = 0;	
	virtual const std::string& Tag() const  = 0;	
	virtual std::unique_ptr<IHtmlElement> Clone() const  = 0;
//	template<typename T>
//    T To() const { return ::To<T>(out()); }
private:
	friend std::ostream& operator<<(std::ostream& out, const IHtmlElement& e) {	return out<<e.Out(0);}
};
////--------------------------------HtmlBase------------------------------------------------

template<typename T>
class HtmlBase: public IHtmlElement
{
protected:
	inline const static std::string tag = T::Identifier;
	inline const static std::string end =  "</" + tag + ">";	
 	HtmlBase(std::unique_ptr<ICss> css = nullptr, const std::string& n = ""): begin(createBegin(css ? (*css)() : "",n)), name{n},css{std::move(css)}, content{""} { };
	HtmlBase(const HtmlBase& html): css(html.css ? html.css->Clone() : nullptr), begin(html.begin), content{html.Out()} { }
public:
	const std::string& Tag() const { return tag; }
	const std::string& Content() const { return content; }
	const auto Data() const { return Out(0); }
protected:
	void apply(std::unique_ptr<ICss> cs) 
	{ 
		css = std::move(cs);
		begin = createBegin(css ? (*css)() : "", name);
	}
private:
	std::string begin;
	std::string name;
	std::unique_ptr<ICss> css;
	std::string content;
	virtual std::string out(const std::string& intent, uint i = 0) const  {	return intent + begin + "\n" + showContent(intent, ++i) + "\n" + intent + end; };	
	virtual std::string showContent(const std::string& intent, uint i = 0) const  = 0;	
	static std::string createBegin(const std::string& s, const std::string& n)  { return "<" + tag + (n != "" ? (" class=\"" + n + "\"") : "") + s + ">"; };	
};

template<typename T>
class HtmlElements: public HtmlBase<T>
{
	using Base = HtmlBase<T>;
public:
 	HtmlElements(std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> v = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>(), std::unique_ptr<ICss> css = nullptr, const std::string& n = ""): Base{std::move(css), n}, elements{std::move(v)} { };
 	HtmlElements( const std::string& n, std::unique_ptr<ICss> css = nullptr, std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> v = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>()): HtmlElements{std::move(v),std::move(css),n}{};
	HtmlElements(const HtmlElements& html): Base{html}, elements{html.cloneElements()} { }
	void Add(std::unique_ptr<IHtmlElement> html) { elements->push_back(std::move(html)); }
	virtual std::unique_ptr<IHtmlElement> Clone() const { return std::make_unique<HtmlElements>(cloneElements()); };
private:
	virtual std::string showContent(const std::string& intent, uint i = 0) const  
	{	
		std::string result;
		if(elements)
			std::for_each(elements->begin(), elements->end(), [&](auto& e) { result += e->Out(i) + "\n"; });
		return result;
	};	
	std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> elements;
	std::unique_ptr<std::vector<std::unique_ptr<IHtmlElement>>> cloneElements() const
	{	
		auto result = std::make_unique<std::vector<std::unique_ptr<IHtmlElement>>>();
		std::for_each(elements->begin(), elements->end(), [&](auto& e) { result->push_back(e->Clone()); });
		return result;
	};	
};

class StyleElement: public HtmlBase<StyleElement>
{
	using Base = HtmlBase<StyleElement>;
public:
 	StyleElement(std::unique_ptr<std::vector<std::unique_ptr<ICss>>> v = std::make_unique<std::vector<std::unique_ptr<ICss>>>(), std::unique_ptr<ICss> css = nullptr, const std::string& n = ""): Base{std::move(css), n}, elements{std::move(v)} { };
 	StyleElement( const std::string& n, std::unique_ptr<ICss> css = nullptr , std::unique_ptr<std::vector<std::unique_ptr<ICss>>> v = std::make_unique<std::vector<std::unique_ptr<ICss>>>()): StyleElement{std::move(v),std::move(css),n}{};
	StyleElement(const StyleElement& html): Base{html}, elements{html.cloneElements()} { }
	void Add(std::unique_ptr<ICss> html) { elements->push_back(std::move(html)); }
	virtual std::unique_ptr<IHtmlElement> Clone() const { return nullptr; };
	inline const static std::string Identifier = "Style";
private:
	virtual std::string showContent(const std::string& intent, uint i = 0) const  
	{	
		std::string result;
		std::for_each(elements->begin(), elements->end(), [&](auto& e) { result += e->Out(i) + "\n"; });
		return result;
	};	
	std::unique_ptr<std::vector<std::unique_ptr<ICss>>> elements;
	std::unique_ptr<std::vector<std::unique_ptr<ICss>>> cloneElements() const
	{	
		auto result = std::make_unique<std::vector<std::unique_ptr<ICss>>>();
		std::for_each(elements->begin(), elements->end(), [&](auto& e) { result->push_back(e->Clone()); });
		return result;
	};	
};

template<typename T, typename E>
class HtmlElementBase: public HtmlBase<T>
{
	using Base = HtmlBase<T>;
protected:
	inline static const std::string Identifier = E::Identifier + "HtmlElement";
 	HtmlElementBase(const E& c, std::unique_ptr<ICss> css = nullptr, const std::string& n = ""): Base{std::move(css),n}, element{c} { };
	HtmlElementBase(const HtmlElementBase& html): Base{html}, element{html.element} { }
public:
	std::unique_ptr<IHtmlElement> Clone() const { return std::make_unique<HtmlElement<T,E>>(element); };
private:
	E element;
	virtual std::string showContent(const std::string& intent, uint i = 0) const  { return element.Out(i);	};	
};

struct Td;

template<typename T, typename E>
class HtmlElement: public HtmlElementBase<T,E>
{
public:
 	HtmlElement(const E& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Black>>>()): HtmlElementBase<T,E>(c, std::move(css)) { };
};

class Name;

template<typename T>
class HtmlElement<T,Name>: public HtmlElementBase<T,Name>
{
public:
 	HtmlElement(const Name& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Blue>>>()): HtmlElementBase<T,Name>(c, std::move(css)) { };
};

class Header;

template<typename T>
class HtmlElement<T,Header>: public HtmlElementBase<T,Header>
{
public:
 	HtmlElement(const Header& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Blue>>>()): HtmlElementBase<T,Header>(c, std::move(css)) { };
};

template<typename T, typename T2>
class HtmlElement<T2,Quantity<T>>: public HtmlElementBase<T2,Quantity<T>>
{
public:
 	HtmlElement(const Quantity<T>& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Green>>>()): HtmlElementBase<T2,Quantity<T>>(c, std::move(css)) 
	{
		if(c < Quantity<T>{0})
			this->apply(std::make_unique<Css<Style<ColorTag,Red>>>());
	};
};

template<typename T, typename E>
HtmlElement(const E&) -> HtmlElement<T,E>;

template <typename T>
concept HtmlElementConcept = requires(T val) {	val.Data(); };
