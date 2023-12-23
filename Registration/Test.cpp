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


			auto fmt=std::make_shared<Factory<IElement>>();
			auto reg = Registration<Factory<IElement>,Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>(&(*fmt));

			auto qp = (*fmt)("Sum","100");
			std::cout<<fmt<<std::endl;
			
			Factory<IToken> fmt2;
			auto reg2 = Registration<Factory<IToken>,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);
			auto qp2 = fmt2("SumToken","100");
			std::cout<<*qp2<<std::endl;

			auto pfs = std::make_shared<CompositeFactory<IPredicateVisitor, Factory<IElement>>>(fmt);
     		pfs->Register("EQ",[](std::unique_ptr<IElement> e) { return std::make_unique<EqualVisitor>(std::move(e)); });
			auto regC = Registration<CompositeFactory<IPredicateVisitor, Factory<IElement>>,EqualVisitor>(&(*pfs));

			auto eqv = ((*pfs)("EqualVisitor", { "IBAN", "DE82660501011021592702"}));
		
			auto tf = TypeFactory<Factory<IElement>, Quantity<Sum>, IBAN, Date, BIC, ID<std::string>, Name, Index, Empty>();
			auto tfc = TypeFactory<CompositeFactory<IPredicateVisitor, Factory<IElement>>, EqualVisitor>(fmt);
	//		auto regT = Registration<TypeFactory<IToken>>(&tf);//,SumToken, IBANToken, DateToken, BICToken, EmptyToken, IDToken, WordToken>(&fmt2);

			std::cout<<"END TypeRegistration"<<std::endl;

			return 0;
		}
};

