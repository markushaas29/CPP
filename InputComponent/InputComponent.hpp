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
	virtual ~IInput(){};
};

class IOutput
{
public:
	virtual std::ostream& operator()(std::ostream& i) = 0;
	virtual ~IOutput(){};
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

template<typename T>
class OutputImpl : public IOutput
{
public:
	using OutputImplType = OutputImpl<T>;
	using OutputContainerType = std::vector<std::string>;
	using OutputIterator = std::vector<std::string>::const_iterator;

	friend T;
	inline static const char* Filename;
	inline static const char* TypeIdentifier = Filename;
	inline static constexpr const char* FileExtension = "Output";
	inline static constexpr const char* Identifier = "Output";
	
	virtual std::ostream& operator()(std::ostream& o)
	{
		std::cout<<"OutputComp Update"<<T::Instance().readings->Size()<<std::endl;
		return o;
	};
	OutputImpl()	{ 	Logger::Log<Info>()<<"OutputImpl initialized "<<std::endl; 	};
	~OutputImpl()	{ }
	OutputImpl& operator=(const OutputImpl&) = delete;
	OutputImpl(const OutputImpl& c) = delete;
private:
	void Output() 
	{
		std::cout<<"OutputComp initialized "<<T::Instance().readings->Size()<<std::endl;
	};
};

template<typename T>
class IO : public IOutput, public IInput
{
public:
	virtual std::istream& operator()(std::istream& i)	{		return (*in)(i);	};
	virtual std::ostream& operator()(std::ostream& o)	{		return (*out)(o);	};
	IO(std::unique_ptr<IInput> i, std::unique_ptr<IOutput> o): in{std::move(i)}, out{std::move(o)}	{ 	Logger::Log<Info>("IO initialized ");	};
	~IO()	{ }
	IO& operator=(const IO&) = delete;
	IO(const IO& c) = delete;
private:
	friend T;
	std::unique_ptr<IOutput> out;
	std::unique_ptr<IInput> in;
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
