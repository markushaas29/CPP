#include <iostream>
#include <cassert> 
#include <vector> 
#include <tuple> 
#include <memory> 
#include "Registration.hpp"
#include "../Common/DateTimes.hpp"
#include "../CSV/Elements.hpp"
#include "../Visitor/PredicateVisitor.hpp"
#include "../CSV/Token.hpp"
#include "../Quantity/Quantity.hpp"
#include "../ObjectFactory/Factory.hpp"

class TypeRegistrationTest
{
	public:
		int Run()
		{
			std::cout<<"START TypeRegistration"<<std::endl;


			Factory<IElement> fmt;
			auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>(&fmt);

			auto qp = fmt("Sum","100");
			std::cout<<fmt<<std::endl;
			
			Factory<IToken> fmt2;
			auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);
			auto qp2 = fmt2("SumToken","100");
			std::cout<<*qp2<<std::endl;

			auto fme =std::make_shared<Factory<IElement>>();
			auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fme);
     		pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });

			std::cout<<"END TypeRegistration"<<std::endl;

			return 0;
		}
};

