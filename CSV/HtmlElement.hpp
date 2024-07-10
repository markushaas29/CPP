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
////--------------------------------HtmlElement------------------------------------------------

class HtmlElement: public IHtmlElement
{
public:
//	inline static const std::string Identifier = D::Identifier;
 	HtmlElement(const IElement& c, const std::string& t = "body", const std::string& s = ""): HtmlElement(c.Data(),t,s) { };
 	HtmlElement(const std::string& c, const std::string& t = "body", const std::string& s = ""): css{std::make_unique<Css<Style<Border,Red>,Style<Border,Red>>>()}, tag{t}, content{c}, style{s} { };
//// 	template<typename T>
////	HtmlElement(T t): HtmlElement(std::to_string(t)) { };
//
//	virtual std::unique_ptr<IHtmlElement> Clone() const  { return std::make_unique<Derived>(value); };	
//	static std::unique_ptr<IHtmlElement> Make(const std::string& s) { return std::make_unique<Derived>(s);	}
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
	const auto& Tag() const { return tag; }
	const auto& Content() const { return content; }
	const auto& Styl() const { return style; }
	const auto& Data() const { return dataS; }
private:
	std::unique_ptr<ICss> css;
	std::string tag;
	std::string style;
	std::string content;
	virtual std::string data() const  {	return begin() + content + end(); };	
	std::string dataS = begin() + content + end();	
	std::string begin() const  {	return tag == "" ? tag : "<" + tag  +(style.size() == 0 ? "" : " ") + style + ">"; };	
	std::string end() const  {	return tag == "" ? tag : "</" + tag + ">"; };	
};


template <typename T>
concept HtmlElementConcept = requires(T val) {	val.Data(); };
