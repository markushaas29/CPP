#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/CSV.hpp"
#include "../File/Info.hpp"
#include "../File/Node.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Home/Counter.hpp"
#include "../Wrapper/Wrapper.hpp"
#include <map>
#include <memory>

#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

template<template<typename...> class ConfigT>
class InputManager
{
public:
	//~ using Config = ConfigT;
	//~ using InputManagerType = InputManager<Config>;
	using ReadingContainerType = std::vector<std::string>;
	using OutIterator = ReadingContainerType::const_iterator;
	using InputIterator = std::vector<std::string>::const_iterator;
	
	inline static constexpr const char* Filename = "Input";
	inline static constexpr const char* Identifier = "Input";

	static InputManager& Instance()
	{
		static InputManager instance;
		return instance;
	}
	
	template<typename Cont>
	void RegisterTo(Cont& cont)
	{
		cont.insert(std::make_pair(Filename,  
			typename Cont::mapped_type(Filename, Identifier, 
			[&](InputIterator begin, InputIterator end){ Parse(begin,end); }, 
			[&](const std::string& s){ return Get(s); }, 
			[&](InputIterator begin, InputIterator end){ return true; })));
	}
	template<typename Separator = T::char_<'\t'>>
	static std::ostream& Display(std::ostream& out)
	{
		//~ for(auto it = readings->cbegin(); it != readings->cend(); ++it)
			//~ (*it)->Display(out);
		
		return out;
	}
	
private:
	
	void Parse(InputIterator begin, InputIterator end)
	{
		Instance();
		for(auto it = begin; it != end; ++it)
		{
			auto vals = String_::Split<T::char_<':'>>(*it);
			for(auto v : vals)
				Logger::Log()<<"InputManagerValue: "<<v<<std::endl;
		}
	}
	
	void Write(const std::string sourcePath = ".")
	{
		//~ Logger::Log<Info>()<<"Write InputManager: "<<GetName()<<" to: "<<csv->GetDestinationPath()<<std::endl;
		//~ csv->Write<InputManagerType>();
	}
	
	
	decltype(auto) Get(const std::string& s) { return std::make_unique<FS::AccountValue<IBAN>>(); }
	
	template<typename Op>
	decltype(auto) Get(const Op&& op){ return op(readings->cbegin(),readings->cend());	}
		
	decltype(auto) ReadingsBegin() { return readings->cbegin(); }
	decltype(auto) ReadingsEnd() { return readings->cend(); }		
	inline static std::unique_ptr<ReadingContainerType> readings = std::make_unique<ReadingContainerType>();
	
	InputManager()	{ 	Logger::Log<Info>()<<"InputManager initialized "<<std::endl; 	};
	~InputManager()	{ }
	InputManager& operator=(const InputManager&) = delete;
	InputManager(const InputManager& c) = delete;
};

template<template<typename...> class C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const InputManager<C> c){	return c.Display(strm);}

#endif
