#include <string>
#include <iostream>
#include "../Logger/Logger.hpp"
#include "../String/String_.hpp"

#ifndef PARSERSTATE_HPP
#define PARSERSTATE_HPP

namespace CommandLine
{
	class ParserState
	{
	protected:
		const std::string ID;
		ParserState(std::string id): ID(id){}
		using CIterator = std::vector<std::string>::const_iterator;
		using Iterator = std::vector<std::string>::iterator;
		static std::vector<std::string> Values;
		inline static std::string extractedValue = "";
	public:
		
		virtual ParserState* Event(std::string c) 
		{ 
			Logger::Log()<<ID<<"\tC: "<<c<<" Extracted:"<<this->extractedValue<<std::endl;
			return this; 
		}
		
		virtual ~ParserState() {}
		
		static ParserState* Reset() { return Incomplete; }
		static void SetValues(Iterator begin, Iterator end) { ParserState::Values = std::vector<std::string>(begin,end); }
		static ParserState *Invalid, *Valid, *Incomplete, *Exit;		
	};
	
	class InvalidState: public ParserState
	{
	public:
		InvalidState(): ParserState("Invalid"){};
		virtual ParserState* Event(std::string c) 
		{ 
			ParserState::Event(c);
			this->extractedValue = "";
			std::cout<<"No match found!"<<std::endl;
			
			return this; 			
		}
	};
	
	class ValidState: public ParserState
	{
	public:
		ValidState(): ParserState("Valid"){};
		virtual ParserState* Event(std::string c)
		{ 
			ParserState::Event(c);
			std::cout<<"Match found!"<<std::endl;
			
			return this; 			
		}
	};
	
	class IncompleteState: public ParserState
	{
	public:
		IncompleteState(): ParserState("Incomplete"){};
		
		virtual ParserState* Event(std::string c) 
		{ 
			this->extractedValue += c;
			ParserState::Event(c);			
			
			auto possibilities = std::vector<std::string>();
			for(CIterator it = ParserState::Values.begin(); it != ParserState::Values.end(); ++it)
			{
				if(*it == this->extractedValue)
					return ParserState::Valid->Event(c);
				
				if(String_::Contains(*it,this->extractedValue))
					possibilities.push_back(*it);
			}
			
			if(possibilities.size() < 1)
				return ParserState::Invalid->Event(c);			
			
			std::cout<<"No match, did you mean:\t";
			for(CIterator it = possibilities.cbegin(); it != possibilities.cend(); ++it)
			{
				std::cout<<*it<<"\t";
			}
			
			std::cout<<std::endl;
			std::cout<<this->extractedValue;
			
			return this; 
		}
	};
	
	class ExitState: public ParserState
	{
	public:
		ExitState(): ParserState("Exit"){};
		virtual ParserState* Event(std::string c) 
		{ 
			ParserState::Event(c);
			return this; 			
		}
	};	
	
	ParserState* ParserState::Invalid = new InvalidState; 
	ParserState* ParserState::Valid = new ValidState; 
	ParserState* ParserState::Incomplete = new IncompleteState; 
	ParserState* ParserState::Exit = new ExitState; 
	std::vector<std::string> ParserState::Values = std::vector<std::string>();
}

#endif
