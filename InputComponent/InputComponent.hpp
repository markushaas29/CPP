#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Wrapper/Wrapper.hpp"
#include <map>
#include <memory>

#pragma once

class IInput
{
public:
	virtual std::istream& operator()(std::istream& i) = 0;
//private:
	virtual void input() = 0;
};

template<typename T>
class InputImpl : public IInput
{
public:
	using InputImplType = InputImpl<T>;
	using InputContainerType = std::vector<std::string>;
	using InputIterator = std::vector<std::string>::const_iterator;

	friend T;
	inline static const char* Filename;
	inline static const char* TypeIdentifier = Filename;
	inline static constexpr const char* FileExtension = "input";
	inline static constexpr const char* Identifier = "Input";
	
	virtual std::istream& operator()(std::istream& i)
	{
		std::cout<<"InputComp Update"<<T::Instance().readings->Size()<<std::endl;
		return i;
	};
	InputImpl()	{ 	Logger::Log<Info>()<<"InputImpl initialized "<<std::endl; 	};
	~InputImpl()	{ }
	InputImpl& operator=(const InputImpl&) = delete;
	InputImpl(const InputImpl& c) = delete;
private:
	void input() 
	{
		std::cout<<"InputComp initialized "<<T::Instance().readings->Size()<<std::endl;
	};
};

//template<template<typename...> class C, typename S = T::char_<'\t'>>
//std::ostream& operator<<(std::ostream& strm, const InputImpl<C> c){	return c.Display(strm);}

template<typename T>
class TestInput : public IInput
{
public:
	using TestInputType = TestInput<T>;
	using InputContainerType = std::vector<std::string>;
	using InputIterator = std::vector<std::string>::const_iterator;
	
	inline static const char* Filename;
	inline static const char* TypeIdentifier = Filename;
	inline static constexpr const char* FileExtension = "input";
	inline static constexpr const char* Identifier = "TestInput";
	
	void input() {  std::cout<<"TestInput initialized "<<std::endl;};
	TestInput()	{ 	Logger::Log<Info>()<<"TestInput initialized "<<std::endl; 	};
	~TestInput()	{ }
	TestInput& operator=(const TestInput&) = delete;
	TestInput(const TestInput& c) = delete;
};
