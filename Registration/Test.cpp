#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Registration.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../CSV/Token.hpp"
#include "../Quantity/Quantity.hpp"
#include "../ObjectFactory/Factory.hpp"

class EC
{
public:
    template<typename T>
    decltype(auto) Make(const std::string& s) const { return std::make_unique<T>(s); }
};

class TC
{
public:
    template<typename T>
    decltype(auto) Make(const std::string& s) const { return std::make_unique<T>(); }
};


class TypeRegistrationTest
{
	public:
		int Run()
		{
			std::cout<<"START TypeRegistration"<<std::endl;


			Factory<IElement> fmt;
			EC ec;
			auto reg = Registration<Factory<IElement>,EC,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>(&fmt,ec);

			auto qp = fmt("Sum","100");
			std::cout<<fmt<<std::endl;
			
			Factory<IToken> fmt2;
			TC tc;
			auto reg2 = Registration<Factory<IToken>,TC,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2, tc);

			std::cout<<"END TypeRegistration"<<std::endl;

			return 0;
		}
};

