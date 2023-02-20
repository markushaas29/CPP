#include <string>
#include <iostream>
#include "ParserState.hpp"
#include "../Wrapper/Wrapper.hpp"
#include "../Logger/Logger.hpp"


#ifndef COMMANDLINEPARSER_HPP
#define COMMANDLINEPARSER_HPP

namespace CommandLine
{
	class Parser
	{
		using Exit =  T::char_<27>;
		using Enter =  T::char_<10>;
// 		std::string extractedValue;
		std::string currentValue = "";
		ParserState* state = ParserState::Incomplete; 
	public:
		Parser()
		{
			
		};
		virtual void Event()
		{ 						
			if(this->currentValue[0] == Exit::Value)
				state = ParserState::Exit->Event(this->currentValue);
						
			state = state->Event(this->currentValue);
		};
		
		void Parse()
		{
			std::vector<std::string> vals { "tes", "tess" };
			ParserState::SetValues(vals.begin(), vals.end());
			do
			{
				this->Event();
				if(!dynamic_cast<IncompleteState*>(this->state))
					break;
			}
			while(std::getline(std::cin,this->currentValue) && this->currentValue[0] != Exit::Value);
		}
		
		virtual ~Parser(){};
	};
}

#endif
