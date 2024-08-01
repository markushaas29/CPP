#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include "../Visitor/Visitor.hpp"
#include "../CSV/Element.hpp"
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

class IHtmlElement//: public BaseVisitable<void>, public BoolVisitable<bool>
{
public:
//	virtual bool operator==(const IHtmlElement& e) const { return Data() == e.Data(); };
////	virtual constexpr std::strong_ordering operator<=>( const IHtmlElement& e) const noexcept = 0;
	std::string Data() const  { return data(); };	
	virtual const std::string Content() const  = 0;	
	virtual const std::string& Tag() const  = 0;	
//	virtual std::unique_ptr<IHtmlElement> Clone() const  = 0;	
//	template<typename T>
//    T To() const { return ::To<T>(Data()); }
private:
	friend std::ostream& operator<<(std::ostream& out, const IHtmlElement& e) {	return out<<e.Data();}
	virtual std::string data() const  = 0;	
////	virtual constexpr bool operator==(const IHtmlElement& e) const = 0;
////	virtual constexpr std::strong_ordering operator<=>( const IHtmlElement& e) const noexcept = 0;
////	virtual constexpr const_iterator Begin() const = 0;
////	constexpr decltype(auto) End() { return data.begin() + size; }
////	constexpr decltype(auto) Size() { return size; }
////
////	constexpr bool operator==(const HtmlElement& e) const{ return Data() == e.Data(); };
////	constexpr std::strong_ordering operator<=>( const HtmlElement& e) const noexcept { return Data() <=> e.Data(); }
};
////--------------------------------HtmlElementBase------------------------------------------------

template<typename E, typename T>
class HtmlElementBase: public IHtmlElement
{
protected:
	inline static const std::string Identifier = E::Identifier + "HtmlElement";
	inline const static std::string tag = T::Identifier;
	inline const static std::string end =  "</" + tag + ">";	
 	HtmlElementBase(const E& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Red>>>()): begin(createBegin((*css)())), css{std::move(css)},element{c}, content{c.Out()} { };
 	HtmlElementBase(const std::string& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Red>>>()): begin(createBegin((*css)())),css{std::move(css)}, content{c} { };
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
//	constexpr bool operator==(const IHtmlElement& e) const{ return Data() == e.Data(); };
//	constexpr std::strong_ordering operator<=>(const IHtmlElement& e) const noexcept { return Data() <=> e.Data(); }
public:
	const std::string& Tag() const { return tag; }
	const std::string Content() const { return content=="" ? element.Out() : content; }
	const auto Data() const { return data(); }
private:
	std::string begin;
	E element;
	std::unique_ptr<ICss> css;
	std::string content;
	virtual std::string data() const  {	return begin + Content() + end; };	
	static std::string createBegin(const std::string& s)  { return "<" + tag  + s + ">"; };	
};

struct Td;

template<typename E, typename T = Td>
class HtmlElement: public HtmlElementBase<E,T>
{
public:
 	HtmlElement(const E& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Red>>>()): HtmlElementBase<E,T>(c, std::move(css)) { };
};

class Name;

template<typename T>
class HtmlElement<Name,T>: public HtmlElementBase<Name,T>
{
public:
 	HtmlElement(const Name& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Blue>>>()): HtmlElementBase<Name,T>(c, std::move(css)) { };
};

template<typename T, typename T2>
class HtmlElement<Quantity<T>, T2>: public HtmlElementBase<Quantity<T>,T2>
{
public:
 	HtmlElement(const Quantity<T>& c, std::unique_ptr<ICss> css = std::make_unique<Css<Style<ColorTag,Green>>>()): HtmlElementBase<Quantity<T>,T2>(c, std::move(css)) { };
};

template<typename E, typename T = Td>
HtmlElement(const E&) -> HtmlElement<E,T>;

template <typename T>
concept HtmlElementConcept = requires(T val) {	val.Data(); };
