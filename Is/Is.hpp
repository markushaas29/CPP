#include<cassert> 
#include<iostream> 
#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../String/Literal.hpp"
//~ #include <source_location>

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


template<typename D>
class IsBase 
{
public:
//	bool operator()(bool c)
//	{
//		return c;
//	}
protected:
	constexpr IsBase(const char* m): message{m} {}
	const std::string ToString() { return std::string{message}; };
	constexpr const char* c() { return message; };
private:
	const char*  message;
};

template<typename T, Literal L = "", bool B = true>
class Is: public IsBase<Is<T,L,B>> 
{
	using Policy = T; 
	using Base = IsBase<Is<T,L,B>>;
public:
	Is(const std::string& m = ""): Base{m.c_str()} {}
	bool operator()(bool c)
	{
		if(!c)
			Policy::exec( literal + Base::ToString());
		return c;
	}
private:
	const std::string literal{L.Value()};
};

template<Literal L, bool B>
class Is<CompileTime, L,B>: public IsBase<Is<CompileTime,L,B>>
{
	using Base = IsBase<Is<CompileTime,L,B>>;
public:
	constexpr Is(const char* m = ""): Base{m} {}
	constexpr bool operator()(bool con)
	{
		static_assert(B);
		return con;
	}
};
