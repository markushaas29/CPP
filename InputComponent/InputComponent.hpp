#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Wrapper/Wrapper.hpp"
#include <map>
#include <memory>

#pragma once

template<typename Config>
class InputComponent
{
public:
	using InputComponentType = InputComponent<Config>;
	using InputContainerType = std::vector<std::string>;
	using InputIterator = std::vector<std::string>::const_iterator;
	
	inline static const char* Filename;
	inline static const char* TypeIdentifier = Filename;
	inline static constexpr const char* FileExtension = "input";
	inline static constexpr const char* Identifier = "Input";
	
	InputComponent()	{ 	Logger::Log<Info>()<<"InputComponent initialized "<<std::endl; 	};
	~InputComponent()	{ }
	InputComponent& operator=(const InputComponent&) = delete;
	InputComponent(const InputComponent& c) = delete;
};

//template<template<typename...> class C, typename S = T::char_<'\t'>>
//std::ostream& operator<<(std::ostream& strm, const InputComponent<C> c){	return c.Display(strm);}
