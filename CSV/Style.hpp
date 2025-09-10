#include <vector>
#include <memory>
#include "IModel.hpp"

#pragma once

class IStyle: public IModel
{
public:
	virtual std::string Element() const  = 0;	
	virtual std::string Value() const  = 0;	
	virtual std::unique_ptr<IStyle> Clone() const  = 0;	
	auto operator()() { return data(); }
private:
	friend std::ostream& operator<<(std::ostream& out, const IStyle& e) {	return out<<e.data();}
	virtual std::string out(const std::string& intent, uint i = 0) const  {	return intent + data(); }
	virtual std::string data() const  = 0;	
};
////--------------------------------Style------------------------------------------------

template<typename TEl, typename TVal>
class Style: public IStyle
{
public:
 	Style(): element{TEl::Id}, value{TVal::Id} { };
	virtual std::string Element() const { return element; };	
	virtual std::string Value() const  { return value; };	
	virtual std::unique_ptr<IStyle> Clone() const  { return std::make_unique<Style>(); };	
private:
	std::string element;
	std::string	value;
	virtual std::string data() const  { return " " + element + ":" + value + ";"; };	
};

template<typename TEl>
class DynStyle: public IStyle
{
public:
 	DynStyle(): element{TEl::Id}, value{""} { };
	virtual std::string Element() const { return element; };	
	virtual std::string Value() const  { return value; };	
	virtual std::unique_ptr<IStyle> Clone() const  { return std::make_unique<DynStyle>(); };	
private:
	std::string element;
	std::string	value;
	virtual std::string data() const  { return " " + element + ":" + value + ";"; };	
};
