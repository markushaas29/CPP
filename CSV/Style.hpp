#include "IModel.hpp"

#pragma once

struct Border		{   inline static constexpr const char* Id = "border"; };
struct ColorTag		{   inline static constexpr const char* Id = "color"; };
struct BackgroundColor		{   inline static constexpr const char* Id = "background-color"; };
struct Display		{   inline static constexpr const char* Id = "display"; };
struct Grid			{   inline static constexpr const char* Id = "grid"; };
struct GridArea			{   inline static constexpr const char* Id = "grid-area"; };
struct GridTemplateAreas			{   inline static constexpr const char* Id = "grid-template-areas"; };
struct Padding		{   inline static constexpr const char* Id = "padding"; };
struct Margin		{   inline static constexpr const char* Id = "margin"; };

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
