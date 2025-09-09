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
	//virtual std::unique_ptr<IStyle> get(uint n) const  = 0;	
};
////--------------------------------Style------------------------------------------------

template<typename TEl, typename TVal>
class Style: public IStyle
{
	template<uint N>
	inline static auto create(auto&& v)
	{
		if constexpr (N == 200) 
			return v;
			v->push_back(std::make_unique<Style<TEl,TVal>>());
			create<N+1>(std::move(v));
	}
public:
 	Style(): element{TEl::Id}, value{TVal::Id} { };
	auto operator[](uint n) { return get(n); }
	static auto Get(uint n) { return get(n); }
	virtual std::string Element() const { return element; };	
	virtual std::string Value() const  { return value; };	
	virtual std::unique_ptr<IStyle> Clone() const  { return std::make_unique<Style>(); };	
private:
	std::string element;
	std::string	value;
	inline static std::unique_ptr<std::vector<std::unique_ptr<IStyle>>> styles = create<0>(std::make_unique<std::vector<std::unique_ptr<IStyle>>>());
	virtual std::string data() const  { return " " + element + ":" + value + ";"; };	
	inline static std::unique_ptr<IStyle> get(uint n) { return styles->at(n)->Clone(); };	
};
