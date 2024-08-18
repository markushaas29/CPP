#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include "../Visitor/Visitor.hpp"
#include "../CSV/Element.hpp"
#include "../CSV/IHtmlOut.hpp"
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

class IHtmlElement: public IHtmlOut
{
public:
//	virtual bool operator==(const IHtmlElement& e) const { return Data() == e.Data(); };
////	virtual constexpr std::strong_ordering operator<=>( const IHtmlElement& e) const noexcept = 0;
	std::string Data() const  { return Out(0); };	
	virtual const std::string& Content() const  = 0;	
	virtual const std::string& Tag() const  = 0;	
	virtual std::unique_ptr<IHtmlElement> Clone() const  = 0;
//	virtual std::unique_ptr<IHtmlElement> Clone() const  = 0;	
//	template<typename T>
//    T To() const { return ::To<T>(out()); }
private:
	friend std::ostream& operator<<(std::ostream& out, const IHtmlElement& e) {	return out<<e.Out(0);}
////	virtual constexpr bool operator==(const IHtmlElement& e) const = 0;
////	virtual constexpr std::strong_ordering operator<=>( const IHtmlElement& e) const noexcept = 0;
////	virtual constexpr const_iterator Begin() const = 0;
////	constexpr decltype(auto) End() { return out.begin() + size; }
////	constexpr decltype(auto) Size() { return size; }
////
////	constexpr bool operator==(const HtmlElement& e) const{ return out() == e.out(); };
////	constexpr std::strong_ordering operator<=>( const HtmlElement& e) const noexcept { return out() <=> e.out(); }
};
////--------------------------------HtmlElementBase------------------------------------------------

class HtmlElementComposition: public IHtmlOut
{
public:
 	HtmlElementComposition(std::unique_ptr<std::vector<std::unique_ptr<IHtmlOut>>> v): elements{std::move(v)} { };
private:
	virtual std::string out(const std::string& intent, uint i = 0) const  
	{	
		std::string result;
		std::for_each(elements->begin(), elements->end(), [&](auto& e) { result += e->Out(i); });
		return result;
	};	
	std::unique_ptr<std::vector<std::unique_ptr<IHtmlOut>>> elements;
};

template<typename E, typename T>
class HtmlElementBase: public IHtmlElement
{
protected:
	inline static const std::string Identifier = E::Identifier + "HtmlElement";
	inline const static std::string tag = T::Identifier;
	inline const static std::string end =  "</" + tag + ">";	
 	HtmlElementBase(const E& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Red>>>()): begin(createBegin((*css)())), css{std::move(css)},element{c}, content{c.Out()}, iout{c.Clone()} { };
	HtmlElementBase(const HtmlElementBase& html): css(html.css->Clone()), begin(html.begin), element{html.element}, content{element.Out()} { }
//// 	template<typename T>
////	HtmlElementBase(T t): HtmlElementBase(std::to_string(t)) { };
//
//	virtual std::unique_ptr<IHtmlElementBase> Clone() const  { return std::make_unique<Derived>(value); };	
//	static std::unique_ptr<IHtmlElementBase> Make(const std::string& s) { return std::make_unique<Derived>(s);	}
//	explicit operator std::string() const  {	return value; };	
//	constexpr decltype(auto) Size() { return size; }
//
//	virtual void Accept(BaseVisitor& visitor) {	return AcceptImpl<D>(*dynamic_cast<D*>(this), visitor); }
//	virtual void Accept(BaseVisitor& visitor) const 
//	{
//		auto c2 = dynamic_cast<const D*>(this);
//		auto c = const_cast<D*>(c2);
//		return AcceptImpl<D>(*c, visitor); 
//	}
//	virtual bool Is(BaseVisitor& visitor) { return AcceptPredicate<D>(*dynamic_cast<D*>(this), visitor); };
//	constexpr bool operator==(const IHtmlElement& e) const{ return out() == e.out(); };
//	constexpr std::strong_ordering operator<=>(const IHtmlElement& e) const noexcept { return out() <=> e.out(); }
public:
	std::unique_ptr<IHtmlElement> Clone() const { return std::make_unique<HtmlElement<E,T>>(element, css->Clone()); };
	const std::string& Tag() const { return tag; }
	const std::string& Content() const { return content; }
	const auto Data() const { return Out(0); }
protected:
	void apply(std::unique_ptr<ICss> cs) 
	{ 
		css = std::move(cs);
		begin = createBegin((*css)());
	}
private:
	E element;
	std::shared_ptr<IHtmlOut> iout;
	std::string begin;
	std::unique_ptr<ICss> css;
	std::string content;
	virtual std::string out(const std::string& intent, uint i = 0) const  {	return intent + begin + "\n" + element.Out(++i) + "\n" + intent + end; };	
	static std::string createBegin(const std::string& s)  { return "<" + tag  + s + ">"; };	
};

struct Td;

template<typename E, typename T = Td>
class HtmlElement: public HtmlElementBase<E,T>
{
public:
 	HtmlElement(const E& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Black>>>()): HtmlElementBase<E,T>(c, std::move(css)) { };
};

template<typename T>
class HtmlElement<HtmlElementComposition,T>: public HtmlElementBase<HtmlElementComposition,T>
{
public:
 	HtmlElement(const HtmlElementComposition& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Black>>>()): HtmlElementBase<HtmlElementComposition,T>(c, std::move(css)) { };
};

class Name;

template<typename T>
class HtmlElement<Name,T>: public HtmlElementBase<Name,T>
{
public:
 	HtmlElement(const Name& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Blue>>>()): HtmlElementBase<Name,T>(c, std::move(css)) { };
};

class Header;

template<typename T>
class HtmlElement<Header,T>: public HtmlElementBase<Header,T>
{
public:
 	HtmlElement(const Header& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Blue>>>()): HtmlElementBase<Header,T>(c, std::move(css)) { };
};

template<typename T, typename T2>
class HtmlElement<Quantity<T>, T2>: public HtmlElementBase<Quantity<T>,T2>
{
public:
 	HtmlElement(const Quantity<T>& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Green>>>()): HtmlElementBase<Quantity<T>,T2>(c, std::move(css)) 
	{
		if(c < Quantity<T>{0})
			this->apply(std::make_unique<Css<Style<ColorTag,Red>>>());
	};
};

template<typename E, typename T = Td>
HtmlElement(const E&) -> HtmlElement<E,T>;

template <typename T>
concept HtmlElementConcept = requires(T val) {	val.Data(); };
