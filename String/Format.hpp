#include<cassert> 
#include<array> 
#include<iostream> 
#include<sstream> 
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../String/Literal.hpp"

#pragma once

static decltype(auto) format(std::stringstream& s, auto t) {	s<<t;    };

static decltype(auto) format(std::stringstream& s, auto t, auto... ts) 
{	
	s<<t;
	return format(s,ts...);    
};

class Format
{	
public:
	Format(auto... ts): value{exec(ts...)} {}
	operator std::string() const { return value; }
private:
	friend  std::ostream& operator<<(std::ostream& out, const Format& f) { return out<<f.value; }
	std::string value;
	static auto exec(auto... ts) 
	{
		std::stringstream s;
		format(s,ts...);    
		return s.str();
	}
};
