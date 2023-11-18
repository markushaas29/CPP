#include <string.h>
#include <map>
#include <regex>
#include <memory>
#include <array>
#include <chrono>
#include <ctime>
#include "Token.hpp"
#include "../Quantity/Quantity.hpp"
#include "../String/String_.hpp"
#include "../To/To.hpp"

#pragma once

class Matcher
{
public:
	inline static const std::string Identifier = "Matcher";
 	Matcher(std::unique_ptr<std::vector<std::unique_ptr<IToken>>> t): tokens(std::move(t)) { };

	//virtual std::unique_ptr<IElement> Create(const std::string& s) const  { return std::make_unique<Type>(s); };	
	virtual std::vector<std::unique_ptr<IElement>> Split(const std::string& s) const  
	{
		std::vector<std::unique_ptr<IElement>> result;
		std::smatch match;

		std::regex rgx(";");
		std::sregex_token_iterator iter(s.begin(),s.end(), rgx, -1);
		std::sregex_token_iterator end;
		for ( ; iter != end; ++iter)
		{
			std::string si(*iter);
			si = String_::Trim(si); 
			std::for_each(tokens->cbegin(), tokens->cend(), [&](auto& t)
					{
						if (t->Match(si))
							result.push_back(t->Create(si));
					});
		}
		return result;
	};	
	
	virtual std::unique_ptr<IElement> operator()(const std::string& s) const  
	{
		for(auto i = 0; i < tokens->size(); ++i)
			if (tokens->at(i)->Match(s))
				return tokens->at(i)->Create(s);
		return nullptr;
	};	
private:
	std::unique_ptr<std::vector<std::unique_ptr<IToken>>> tokens;
};
