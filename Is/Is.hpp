#include<cassert> 
#include<array> 
#include<iostream> 
#include<sstream> 
#include<stdexcept> 
#include<source_location>
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../String/Literal.hpp"

#pragma once

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
		throw std::logic_error(m); 
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
	constexpr IsBase(const char* m, Literal<L.Size> l, auto lc): message{m}, literal{l}, loc{lc} {}
	const std::string ToString() { return std::string(literal.Value.cbegin(), literal.Value.cend()) + " > " + std::string{message} + loc.function_name() + std::to_string(loc.line()); };
	friend std::ostream& operator<<(std::ostream& s, const IsBase& i) { return s<<i.literal;  }  
private:
	Literal<L.Size> literal;
	const char*  message;
	std::source_location loc;
};

template<typename T, Literal L = "", bool B = true>
class Is: public IsBase<Is<T,L,B>, L> 
{
	using Policy = T;
	using Base = IsBase<Is<T,L,B>, L>;
public:
	Is(const std::string& m, std::source_location l = std::source_location::current()): Base{m.c_str(), L, l} {}
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
	constexpr Is(const char* m = ""): Base{m, L, std::source_location::current()} {}
	constexpr bool operator()(bool con)
	{
		static_assert(B);
		return con;
	}
};
