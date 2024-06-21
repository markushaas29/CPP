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
class IElement: public BaseVisitable<void>, public BoolVisitable<bool>
{
public:
	virtual void Accept(BaseVisitor& visitor) = 0;
	virtual bool Is(BaseVisitor& visitor) = 0;
	//virtual void Accept(BaseVisitor& visitor) const = 0;
	virtual bool operator==(const IElement& e) const { return Data() == e.Data(); };
//	virtual constexpr std::strong_ordering operator<=>( const IElement& e) const noexcept = 0;
	virtual const std::string& Data() const  = 0;	
	virtual std::unique_ptr<IElement> Clone() const  = 0;	
	virtual std::unique_ptr<IElement> Clone(const std::string& s) const  = 0;	
	template<typename T>
    T To() const { return ::To<T>(Data()); }
private:
	friend std::ostream& operator<<(std::ostream& out, const IElement& e) {	return out<<e.Data();}
//	virtual constexpr bool operator==(const IElement& e) const = 0;
//	virtual constexpr std::strong_ordering operator<=>( const IElement& e) const noexcept = 0;
//	virtual constexpr const_iterator Begin() const = 0;
//	constexpr decltype(auto) End() { return data.begin() + size; }
//	constexpr decltype(auto) Size() { return size; }
//
//	constexpr bool operator==(const Element& e) const{ return Data() == e.Data(); };
//	constexpr std::strong_ordering operator<=>( const Element& e) const noexcept { return Data() <=> e.Data(); }
};
//--------------------------------Element------------------------------------------------

template<typename D>
class Element: public IElement, public TypeCounter<D>
{
	using Derived = D;
public:
	inline static const std::string Identifier = D::Identifier;
 	Element(const std::string& s): value{Derived::check(s)}, size{s.size()} { };
// 	template<typename T>
//	Element(T t): Element(std::to_string(t)) { };

	const std::string& Data() const  {	return value; };	
	virtual std::unique_ptr<IElement> Clone() const  { return std::make_unique<Derived>(value); };	
	virtual std::unique_ptr<IElement> Clone(const std::string& s) const  { return std::make_unique<Derived>(s); };	
	static std::unique_ptr<IElement> Make(const std::string& s) { return std::make_unique<Derived>(s);	}
	explicit operator std::string() const  {	return value; };	
	constexpr decltype(auto) Size() { return size; }

	virtual void Accept(BaseVisitor& visitor) {	return AcceptImpl<D>(*dynamic_cast<D*>(this), visitor); }
	virtual void Accept(BaseVisitor& visitor) const 
	{
		auto c2 = dynamic_cast<const D*>(this);
		auto c = const_cast<D*>(c2);
		return AcceptImpl<D>(*c, visitor); 
	}
	virtual bool Is(BaseVisitor& visitor) { return AcceptPredicate<D>(*dynamic_cast<D*>(this), visitor); };
	constexpr bool operator==(const IElement& e) const{ return Data() == e.Data(); };
	constexpr std::strong_ordering operator<=>(const IElement& e) const noexcept { return Data() <=> e.Data(); }
private:
	std::string value;
	std::size_t size;
};


template <typename T>
concept ElementConcept = requires(T val) {	val.Data(); };
