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

static std::string Format(auto... ts) 
{	
	std::stringstream s;
	format(s,ts...);    
	return s.str();    
};
