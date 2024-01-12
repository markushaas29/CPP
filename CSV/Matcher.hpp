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
 	Matcher(const std::vector<std::unique_ptr<IToken>>& t): tokens(std::make_unique<std::vector<std::unique_ptr<IToken>>>()) 
	{
		std::for_each(t.cbegin(), t.cend(), [&](auto& p) { tokens->push_back(p->Clone()); });
	};

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
	
	virtual std::vector<std::unique_ptr<Index<int>>> Indices(const std::string& s) const  
	{
		std::vector<std::unique_ptr<Index<int>>> result;
		std::smatch match;

		std::regex rgx(";");
		std::sregex_token_iterator iter(s.begin(),s.end(), rgx, -1);
		std::sregex_token_iterator end;
		for (size_t i = 0; iter != end; ++iter, ++i)
		{
			std::string si(*iter);
			si = String_::Trim(si); 
			std::for_each(tokens->cbegin(), tokens->cend(), [&](auto& t)
					{
						if (t->Match(si))
						{
							std::cout<<"ID"<<i<<std::endl;
							auto index = std::make_unique<Index<int>>(si);
							index->id = i;
							result.push_back(std::move(index));
						}
					});
		}
		return result;
	};	
	
	virtual std::vector<std::unique_ptr<Index<int>>> Indices(const std::vector<std::string>& s) const  
	{
		std::vector<std::unique_ptr<Index<int>>> result;

		for (size_t i = 0; i < s.size(); ++i)
		{
			auto si = String_::Trim(s[i]); 
			std::for_each(tokens->cbegin(), tokens->cend(), [&](auto& t)
					{
						if (t->Match(si))
						{
							std::cout<<"ID"<<i<<std::endl;
							auto index = std::make_unique<Index<int>>(si);
							index->id = i;
							result.push_back(std::move(index));
						}
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

	virtual bool Match(const std::string& s) const  
	{
		for(auto i = 0; i < tokens->size(); ++i)
			if (tokens->at(i)->Match(s))
				return true;
		return false;
	};	
private:
	std::unique_ptr<std::vector<std::unique_ptr<IToken>>> tokens;
	friend std::ostream& operator<<(std::ostream& out, const Matcher& m) 
	{ 
		std::for_each(m.tokens->cbegin(), m.tokens->cend(), [&](auto& t) { out<<*t<<"\n"; });
		return out;
	}
};
