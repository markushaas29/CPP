#include <iostream>
#include <regex>
#include <cassert>
#include "../Element.hpp"
#include "../Elements.hpp"
#include "../../Common/DateTimes.hpp"
#include "../../ObjectFactory/Factories.hpp"

class ElementRegexTest
{      
public:
	bool Run()
	{
    	std::cout<<"START RegexTest"<<std::endl;

		std::unique_ptr<IElement> pi = std::make_unique<IBAN>("");
		std::unique_ptr<IElement> pn = std::make_unique<Name>("");
		std::unique_ptr<IElement> vi = std::make_unique<Value<int>>(23);
		std::unique_ptr<IElement> vd = std::make_unique<Value<double>>(23.456);
		std::unique_ptr<IElement> vdt = std::make_unique<Date>(29,9,1986);
		std::vector<std::unique_ptr<IElement>> v;
		v.push_back(std::move(pi));
		v.push_back(std::move(pn));
		v.push_back(std::move(vi));
		v.push_back(std::move(vd));
		v.push_back(std::move(vdt));

		std::smatch m;
		std::string t = "29.5.1986";
		assert(std::regex_match(t,m,v[4]->Pattern()));
		t = "29.05.1986";
		assert(std::regex_match(t,m,v[4]->Pattern()));
		t = "02.05.1986";
		assert(std::regex_match(t,m,v[4]->Pattern()));
		t = "2.5.1986";
		assert(std::regex_match(t,m,v[4]->Pattern()));
		t = "12.11.1986";
		assert(std::regex_match(t,m,v[4]->Pattern()));
		t = "12.13.1986";
		assert(!std::regex_match(t,m,v[4]->Pattern()));
		t = "32.11.1986";
		assert(!std::regex_match(t,m,v[4]->Pattern()));
		std::cout<<m[0];

    	std::cout<<"\nEnd RegexTest"<<std::endl;

    	return true;
	}
};
