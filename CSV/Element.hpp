#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/ArrayHelper.hpp"
#include "../Quantity/Quantity.hpp"
#include "../To/To.hpp"
#include "../Visitor/Visitor.hpp"
#include <string.h>
#include <map>
#include <regex>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>

#pragma once
class IElement: public BaseVisitable<void>
{
public:
	virtual void Accept(BaseVisitor& visitor) = 0;
	virtual const std::string_view Data() const  = 0;	
	virtual std::unique_ptr<IElement> Clone() const  = 0;	
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
class Element: public IElement
{
	using Derived = D;
public:
	inline static const std::string Identifier = "Element";
 	Element(const std::string& s): value{Derived::check(s)}, size{s.size()} { };
// 	template<typename T>
//	Element(T t): Element(std::to_string(t)) { };

	const std::string_view Data() const  {	return value; };	
	virtual std::unique_ptr<IElement> Clone() const  { return std::make_unique<Derived>(value); };	
	explicit operator std::string() const  {	return value; };	
	constexpr decltype(auto) Size() { return size; }

	virtual void Accept(BaseVisitor& visitor)
	{
		if(auto p = dynamic_cast<D*>(this))     
		{
               std::cout<<*p<<std::endl;
		}
		if(Visitor<Date,void>* p = dynamic_cast<Visitor<Date,void>*>(&visitor))     
		{
               std::cout<<"CAST ACCE"<<std::endl;
		}
		return AcceptImpl(*this, visitor); 
	}
	bool operator==(const Element& e) const{ return Data() == e.Data(); };
	std::strong_ordering operator<=>( const Element& e) const noexcept { return Data() <=> e.Data(); }
private:
	std::string value;
	std::size_t size;
};


template <typename T>
concept ElementConcept = requires(T val) {	val.Data(); };
