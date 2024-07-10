
#pragma once

struct Red			{   inline static constexpr const char* Id = "Red"; };
struct Border		{   inline static constexpr const char* Id = "Border"; };

class IStyle//: public BaseVisitable<void>, public BoolVisitable<bool>
{
public:
	virtual std::string Element() const  = 0;	
	virtual std::string Value() const  = 0;	
private:
	friend std::ostream& operator<<(std::ostream& out, const IStyle& e) {	return e.data(out);}
	virtual std::ostream& data(std::ostream& out) const  = 0;	
};
////--------------------------------Style------------------------------------------------

template<typename TEl, typename TVal>
class Style: public IStyle
{
public:
 	Style(): element{TEl::Id}, value{TVal::Id} { };
	virtual std::string Element() const { return element; };	
	virtual std::string Value() const  { return value; };	
private:
	std::string element;
	std::string	value;
	virtual std::ostream& data(std::ostream& out) const  { return out<<element<<" : "<< value; };	
};
