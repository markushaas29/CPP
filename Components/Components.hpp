#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "Interfaces.hpp"
#include <map>
#include <memory>

#pragma once

template<typename> class Counter;

template<typename T> class InImpl : public IIn {};

template<typename C>
class InImpl<Counter<C>> : public IIn
{
	using T = Counter<C>;
public:
	using Type = InImpl<T>;
	inline static constexpr const char* Identifier = "In";
	
	virtual std::istream& operator()(std::istream& i)
	{
		std::cout<<"InComp Update"<<T::Instance().readings->Size()<<std::endl;
		return i;
	};
	InImpl()	{ 	Logger::Log<Info>()<<"InImpl initialized "<<std::endl; 	};
	~InImpl()	{ }
	InImpl& operator=(const InImpl&) = delete;
	InImpl(const InImpl& c) = delete;
private:
	friend T;
	void input() 
	{
		std::cout<<"InComp initialized "<<T::Instance().readings->Size()<<std::endl;
	};
};

template<typename T> class OutImpl : public IOut {};

template<typename C>
class OutImpl<Counter<C>> : public IOut
{
	using T = Counter<C>;
public:
	using Type = OutImpl<T>;
	inline static constexpr const char* Identifier = "Out";
	
	virtual std::ostream& operator()(std::ostream& o)	{	return display(o);	};
	virtual std::unique_ptr<std::ofstream> operator()(std::unique_ptr<std::ofstream> of) { return write(std::move(of)); }
	OutImpl()	{ 	Logger::Log<Info>()<<"OutImpl initialized "<<std::endl; 	};
	~OutImpl()	{ }
	OutImpl& operator=(const OutImpl&) = delete;
	OutImpl(const OutImpl& c) = delete;
private:
	friend T;
    std::ostream& display(std::ostream& out)
    {
		T::Config::Display(out);
        for(auto it = T::readings->CBegin(); it != T::readings->CEnd(); ++it)
        {
              (*it)->Display(out);
              out<<std::endl;
        }
          
    	return out;
	}

	std::unique_ptr<std::ofstream> write(std::unique_ptr<std::ofstream> of)
    {
    	(*of)<<T::name<<":"<<";;"<<T::Config::Unit::Sign()<<";"<<T::MeterType::Name<<";";
        if(T::readings->CBegin() != T::readings->CEnd())
            (*of)<<*(*(T::readings->CEnd() - 1));
        (*of)<<std::endl;
         
        return of;
    }
};

template<typename T>
class IO : public IIO
{
public:
	virtual std::istream& operator()(std::istream& i)	{		return (*in)(i);	};
	virtual std::ostream& operator()(std::ostream& o)	{		return (*out)(o);	};
	virtual std::unique_ptr<std::ofstream> operator()(std::unique_ptr<std::ofstream> of) { return (*out)(std::move(of)); }
	IO(std::unique_ptr<IIn> i, std::unique_ptr<IOut> o): in{std::move(i)}, out{std::move(o)}	{ 	Logger::Log<Info>("IO initialized ");	};
	~IO()	{ }
	IO& operator=(const IO&) = delete;
	IO(const IO& c) = delete;
private:
	friend T;
	std::unique_ptr<IOut> out;
	std::unique_ptr<IIn> in;
};
//template<template<typename...> class C, typename S = T::char_<'\t'>>
//std::ostream& operator<<(std::ostream& strm, const InputImpl<C> c){	return c.Display(strm);}

template<typename T>
class TestInput : public IIn
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
