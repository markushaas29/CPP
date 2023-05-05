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
	virtual void input() = 0;
};

template<typename Config>
class InputComponent : public IInput
{
public:
	using InputComponentType = InputComponent<Config>;
	using InputContainerType = std::vector<std::string>;
	using InputIterator = std::vector<std::string>::const_iterator;

	friend Config;
	inline static const char* Filename;
	inline static const char* TypeIdentifier = Filename;
	inline static constexpr const char* FileExtension = "input";
	inline static constexpr const char* Identifier = "Input";
	
	void input() 
	{
		std::cout<<"InputComp initialized "<<Config::Instance().readings->Size()<<std::endl;};
	InputComponent()	{ 	Logger::Log<Info>()<<"InputComponent initialized "<<std::endl; 	};
	~InputComponent()	{ }
	InputComponent& operator=(const InputComponent&) = delete;
	InputComponent(const InputComponent& c) = delete;
private:
};

//template<template<typename...> class C, typename S = T::char_<'\t'>>
//std::ostream& operator<<(std::ostream& strm, const InputComponent<C> c){	return c.Display(strm);}

template<typename Config>
class TestInput : public IInput
{
public:
	using TestInputType = TestInput<Config>;
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
