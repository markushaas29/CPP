#include<cassert> 
#include<array> 
#include<iostream> 
#include<sstream> 
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../String/Literal.hpp"
//~ #include <source_location>

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
template<typename T, Literal L, bool B> class Is;
class CompileTime 
{
	template<typename T, Literal L, bool B> friend class Is;
	constexpr static void exec(const char* m)	{	; }
};

class Ignore {};

class Asserting 
{
	template<typename T, Literal L, bool B> friend class Is;
	static void exec(const std::string& m)	
	{	
		Logger::Log<Info>(m);
		assert(false); 
	}
};

class Throwing 
{
	template<typename T, Literal L, bool B> friend class Is;
	static void exec(const std::string& m)	
	{
		Logger::Log<Error>(m);
		throw m; 
	}
};

class Terminating 
{
	template<typename T, Literal L, bool B> friend class Is;
	static void exec(const std::string& m)	{	std::terminate(); }
};

class Logging 
{
	template<typename T, Literal L, bool B> friend class Is;
	static void exec(const std::string& m)	{	Logger::Log<Info>(m); }
};


template<typename D, Literal L>
class IsBase 
{
public:
	constexpr auto Lit() { return literal; };
	constexpr const char* Message() { return message; };
protected:
	constexpr IsBase(const char* m, Literal<L.Size> l): message{m}, literal{l} {}
	const std::string ToString() { return std::string(literal.Value.cbegin(), literal.Value.cend()) + " > " + std::string{message}; };
	friend std::ostream& operator<<(std::ostream& s, const IsBase& i) { return s<<i.literal;  }  
private:
	Literal<L.Size> literal;
	const char*  message;
};

template<typename T, Literal L = "", bool B = true>
class Is: public IsBase<Is<T,L,B>, L> 
{
	using Policy = T;
	using Base = IsBase<Is<T,L,B>, L>;
public:
	Is(const std::string& m = ""): Base{m.c_str(), L} {}
	bool operator()(bool c)
	{
		if(!c)
			Policy::exec( Base::ToString());
		return c;
	}
	static constexpr std::size_t N = L.Size;
private:
};

template<Literal L, bool B>
class Is<CompileTime, L,B>: public IsBase<Is<CompileTime,L,B>, L>
{
	using Base = IsBase<Is<CompileTime,L,B>, L>;
public:
	constexpr Is(const char* m = ""): Base{m, L} {}
	constexpr bool operator()(bool con)
	{
		static_assert(B);
		return con;
	}
};
