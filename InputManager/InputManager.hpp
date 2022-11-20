#include "../String/String_.hpp"
#include "../Logger/Logger.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Unit/SIPrefix.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "InputVisitor.hpp"
#include <map>
#include <memory>

#ifndef INPUTMANAGER_HPP
#define INPUTMANAGER_HPP

template<template<typename... Ts> class Config>
class InputManager
{
public:
	using InputManagerType = InputManager<Config>;
	using InputContainerType = std::vector<std::string>;
	using InputIterator = std::vector<std::string>::const_iterator;
	using VisitorMap = std::map<std::string,std::unique_ptr<InputVisitor>>;
	
	inline static const char* Filename;
	inline static const char* TypeIdentifier = Filename;
	inline static constexpr const char* FileExtension = "input";
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
	
	void CreateFile()
	{
		auto fs = std::make_unique<std::ofstream>(std::string(TypeIdentifier) + "." + std::string(FileExtension) );
		
		(*fs)<<DateTimes::Date::Identifier<<":"<<"12.11.2022"<<std::endl;
		
		for(auto it = visitors->cbegin(); it != visitors->cend(); ++it)
			fs = (*it).second->Input(std::move(fs));
			
		fs->close();
	}
	
	void Register(std::unique_ptr<InputVisitor> visitor) { 
		Logger::Log<Info>("Register", visitor->Name(),"in InputManager"); 
		visitors->insert({visitor->Identifier(), std::move(visitor)}); 
	}
	
	template<typename Separator = T::char_<'\t'>>
	static std::ostream& Display(std::ostream& out)
	{
		for(auto it = visitors->cbegin(); it != visitors->cend(); ++it)
			out<<"Update"<<(*it).second->Name()<<std::endl;
		
		return out;
	}
	
private:
	inline static std::unique_ptr<InputContainerType> inputs = std::make_unique<InputContainerType>();
	inline static std::unique_ptr<VisitorMap> visitors = std::make_unique<VisitorMap>();

	void Parse(InputIterator begin, InputIterator end);
		
	decltype(auto) Get(const std::string& s) { return std::make_unique<FS::AccountValue<IBAN>>(); }
	
	template<typename Op>
	decltype(auto) Get(const Op&& op){ return op(inputs->cbegin(),inputs->cend());	}
	
	InputManager()	{ 	Logger::Log<Info>()<<"InputManager initialized "<<std::endl; 	};
	~InputManager()	{ }
	InputManager& operator=(const InputManager&) = delete;
	InputManager(const InputManager& c) = delete;
};

template<template<typename...> class C, typename S = T::char_<'\t'>>
std::ostream& operator<<(std::ostream& strm, const InputManager<C> c){	return c.Display(strm);}

#endif
