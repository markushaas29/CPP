#include <iostream>
#include <regex>
#include <cassert>
#include "../Element.hpp"
#include "../Elements.hpp"
#include "../Token.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../ObjectFactory/Factories.hpp"

class ElementRegexTest
{      
public:
	bool Run()
	{
    	std::cout<<"START RegexTest"<<std::endl;

		std::vector<std::unique_ptr<IToken>> v;
		v.push_back(std::make_unique<DateToken>());
		v.push_back(std::make_unique<IBANToken>());
		v.push_back(std::make_unique<BICToken>());
		v.push_back(std::make_unique<WordToken>());
		v.push_back(std::make_unique<SumToken>());
		v.push_back(std::make_unique<EntryToken>());
		v.push_back(std::make_unique<ValueToken>());
		v.push_back(std::make_unique<QuantityToken>());

		for(auto& p: v)
			std::cout<<*p<<std::endl;

		std::smatch m;
		std::string t = "29.5.1986";
		assert(std::regex_match(t,m,v[0]->Pattern()));
		assert(v[0]->Match(t));
		t = "29.05.1986";
  		assert(v[0]->Match(t));
		t = "02.05.1986";
  		assert(v[0]->Match(t));
		t = "2.5.1986";
  		assert(v[0]->Match(t));
		t = "12.11.1986";
  		assert(v[0]->Match(t));
		t = "12.13.1986";
  		assert(!v[0]->Match(t));
		t = "32.11.1986";
  		assert(!v[0]->Match(t));
		std::cout<<*v[0];

		auto it = IBANToken("DE19660623660009232702");
		t = "DE19660623660009232702";
  		assert(!it.Match(t));
		t = "DE19660623660009234702";
  		assert(it.Match(t));

    	std::cout<<"\nEnd RegexTest"<<std::endl;

    	return true;
	}
};
