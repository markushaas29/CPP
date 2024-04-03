#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include "../Visitor/Visitor.hpp"
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

//class IHtmlElement: public BaseVisitable<void>, public BoolVisitable<bool>
//{
//public:
//	virtual void Accept(BaseVisitor& visitor) = 0;
//	virtual bool Is(BaseVisitor& visitor) = 0;
//	//virtual void Accept(BaseVisitor& visitor) const = 0;
//	virtual bool operator==(const IHtmlElement& e) const { return Data() == e.Data(); };
////	virtual constexpr std::strong_ordering operator<=>( const IHtmlElement& e) const noexcept = 0;
//	virtual const std::string& Data() const  = 0;	
//	virtual std::unique_ptr<IHtmlElement> Clone() const  = 0;	
//	template<typename T>
//    T To() const { return ::To<T>(Data()); }
//private:
//	friend std::ostream& operator<<(std::ostream& out, const IHtmlElement& e) {	return out<<e.Data();}
////	virtual constexpr bool operator==(const IHtmlElement& e) const = 0;
////	virtual constexpr std::strong_ordering operator<=>( const IHtmlElement& e) const noexcept = 0;
////	virtual constexpr const_iterator Begin() const = 0;
////	constexpr decltype(auto) End() { return data.begin() + size; }
////	constexpr decltype(auto) Size() { return size; }
////
////	constexpr bool operator==(const HtmlElement& e) const{ return Data() == e.Data(); };
////	constexpr std::strong_ordering operator<=>( const HtmlElement& e) const noexcept { return Data() <=> e.Data(); }
//};
////--------------------------------HtmlElement------------------------------------------------

//class HtmlElement: public IHtmlElement, public TypeCounter<D>
class HtmlElement
{
//	using Derived = D;
//public:
//	inline static const std::string Identifier = D::Identifier;
 	HtmlElement(const std::string& c, const std::string& t = "", const std::string& s = ""): tag{t}, content{c}, style{s} { };
//// 	template<typename T>
////	HtmlElement(T t): HtmlElement(std::to_string(t)) { };
//
//	const std::string& Data() const  {	return value; };	
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
private:
	std::string tag;
	std::string style;
	std::string content;
};


template <typename T>
concept HtmlElementConcept = requires(T val) {	val.Data(); };
