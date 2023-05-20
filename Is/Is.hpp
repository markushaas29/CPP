#include "../Logger/Logger.hpp"
#include "../Wrapper/Wrapper.hpp"
//~ #include <source_location>

#pragma once

template<typename T> class Is;
class CompileTime 
{
	template<typename T> friend class Is;
	static void exec(const std::string& m)	{	throw m; }
};
class Ignore {};
class Throwing 
{
	template<typename T> friend class Is;
	static void exec(const std::string& m)	{	throw m; }
};
class Termiating 
{
	template<typename T> friend class Is;
	static void exec(const std::string& m)	{	throw m; }
};
class Logging 
{
	template<typename T> friend class Is;
	static void exec(const std::string& m)	{	throw m; }
};


template<typename D>
class IsBase 
{
public:
	bool operator()(bool c)
	{
		return c;
	}
protected:
	IsBase(const std::string& m): message{m} {}
private:
	const std::string message;
};

template<typename T>
class Is: public IsBase<Is<T>> 
{
	using Base = IsBase<Is<T>>;
public:
	Is(const std::string& m = ""): Base{m} {}
	bool operator()(bool c)
	{
		return c;
	}
};

template<>
class Is<CompileTime>: public IsBase<Is<CompileTime>>
{
	using Base = IsBase<Is<CompileTime>>;
public:
	Is(const std::string& m = ""): Base{m} {}
	constexpr bool operator()(bool c)
	{
		return c;
	}
};

//template<>
//class Is<Logging> 
//{
//public:
//	bool operator()(bool c)
//	{
//		Logger::Log("Is false");
//		return c;
//	}
//};
//
//template<>
//class Is<Logging, Throwing> 
//{
//public:
//	bool operator()(bool c)
//	{
//		Logger::Log("Is false");
//		if(!c)
//			throw "Test";
//		return c;
//	}
//};
//constexpr bool expect(C cond, const std::string& message)
//{
//	if constexpr(action == ErrorAction::Logging)
//	{
//		if(!cond)
//		{
//			Logger::Log<LogLevel>(message);
//			return false;
//		}
//		return true;
//	}
//	if constexpr(action == ErrorAction::Throwing)
//	{	
//		if(!cond)
//			throw "Test";
//		return true;
//	}	
//}

